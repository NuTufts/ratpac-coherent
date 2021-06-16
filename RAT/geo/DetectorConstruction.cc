#include <string>
#include <sstream>
#include <cstdlib>
#include <TVector3.h>
#include <G4GeometryManager.hh>
#include <G4PhysicalVolumeStore.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4VPhysicalVolume.hh>
#include <G4SolidStore.hh>
#include <G4SDManager.hh>
#include <G4Material.hh>
#include <G4MaterialTable.hh>
#include <BWVetGenericChamber.hh>
#include <RAT/geo/DetectorConstruction.hh>
#include <RAT/physics/PhotonThinning.hh>
#include <RAT/DB/DB.hh>
#include <RAT/base/Log.hh>
#include <RAT/geo/GeoBuilder.hh>
#include <RAT/geo/Materials.hh>
#include <RAT/geo/GLG4SimpleOpDetSD.hh>
#include <RAT/geo/GeoFactory.hh>

#include <RAT/geo/DetectorFactory.hh>
#include <RAT/geo/WatchmanDetectorFactory.hh>
#include <RAT/geo/TheiaDetectorFactory.hh>

using namespace std;

namespace RAT {

DetectorConstruction* DetectorConstruction::sDetectorConstruction = NULL;

DetectorConstruction::DetectorConstruction() {
    DetectorFactory::Register("Watchman",new WatchmanDetectorFactory());
    DetectorFactory::Register("Theia",new TheiaDetectorFactory());
}

G4VPhysicalVolume* DetectorConstruction::Construct() {
  // Load the DETECTOR table
  DB* db = DB::Get();
  DBLinkPtr ldetector = db->GetLink("DETECTOR");

  string glg4data = "";
  string experiment = "";
  if (getenv("GLG4DATA") != NULL) {
    glg4data = string(getenv("GLG4DATA")) + "/";
  }

  // Load experiment RATDB files before doing anything else
  try {
    experiment = ldetector->GetS("experiment");
    info << "Loading experiment-specific RATDB files for: "
         << experiment << newline;
    db->LoadAll(glg4data + experiment);
  }
  catch (DBNotFoundError& e) {
    info << "No experiment-specific tables loaded." << newline;
  }

  try { 
    string detector_factory = ldetector->GetS("detector_factory");
    info << "Loading detector factory " << detector_factory << newline;
    DetectorFactory::DefineWithFactory(detector_factory,ldetector);
  } catch (DBNotFoundError &e) {
    try {
      string geo_file = ldetector->GetS("geo_file");
      info << "Loading detector geometry from " << geo_file << newline;
      if (db->Load(geo_file) == 0) {
        Log::Die("DetectorConstruction: Could not open detector geometry");
      }
    } catch (DBNotFoundError &_e) {
        Log::Die("DetectorConstruction: Could not open geo_file or detector_factory");
    }
  }

  // Delete the old detector if we are constructing a new one
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  std::cout << "[DETECTOR CONSTRUCTION]" << std::endl;

  GeoBuilder geo;
  fWorldPhys = geo.ConstructGDML(); // must come before so it doesn't produce duplicate Material Info

  info << "Constructing detector materials...\n";
  ConstructMaterials();

  // Add sensitive volumes here (only veto for now)
  G4SDManager* sdman = G4SDManager::GetSDMpointer();
  G4VSensitiveDetector *veto 
    =  new BWVetGenericChamber("/mydet/veto/genericchamber");  
  sdman->AddNewDetector(veto);

  // Setup photon thinning parameters
  PhotonThinning::Init();

  G4VPhysicalVolume* constructedWorld = geo.ConstructAll("GEO",fWorldPhys);
  if ( constructedWorld!=NULL )
    fWorldPhys = constructedWorld;

  if ( geo.GetBuilderSource()==GeoBuilder::GDMLFILE ) {
    SetupGDMLSD();
  }

  info << "Dump Surface Info...\n";
  G4LogicalBorderSurface::DumpInfo();
  G4LogicalSkinSurface::DumpInfo();


  const G4MaterialTable* mat_table = G4Material::GetMaterialTable();
  for ( G4MaterialTable::const_iterator imat= mat_table->begin(); imat!=mat_table->end(); imat++ ) {
    std::cout << "============================================================" << std::endl;
    std::cout << "Material: " << (*imat)->GetName() << std::endl;
    if ( (*imat)->GetMaterialPropertiesTable() )
      (*imat)->GetMaterialPropertiesTable()->DumpTable();
    else
      std::cout << "No property table." << std::endl;
  }

  return fWorldPhys;
}


void DetectorConstruction::ConstructMaterials() {
  Materials::ConstructMaterials();
  Materials::LoadOpticalSurfaces();
}


DetectorConstruction* DetectorConstruction::GetDetectorConstruction() {
  if (!sDetectorConstruction) {
    sDetectorConstruction = new DetectorConstruction();
  }
  return sDetectorConstruction;
}

void DetectorConstruction::SetupGDMLSD() {
  DBLinkGroup lgeo = DB::Get()->GetLinkGroup("GEO");

  // find GDML table
  DBLinkGroup::iterator i_table;
  for (i_table = lgeo.begin(); i_table != lgeo.end(); ++i_table) {
    string name = i_table->first;
    DBLinkPtr table = i_table->second;
    string gdmlfilename;
    try {
      gdmlfilename = table->GetS("gdml_file");
    }
    catch (DBNotFoundError &e) {
      continue;
    }
    info << "GeoBuilder used GDML file as source of geometry." << newline;
    G4PhysicalVolumeStore* pvolumes = G4PhysicalVolumeStore::GetInstance();

    // ------------------------------------------------------------
    // OPTICAL SENSORS
    // ------------------------------------------------------------
    std::vector<std::string> sdname_v;
    try {
      sdname_v = table->GetSArray("opdet_sdnames");
    }
    catch (DBNotFoundError &e) {
      try {
	std::string sdname = table->GetS("opdet_sdnames");
        sdname_v.push_back( sdname );
      }
      catch (DBNotFoundError &e2) {
	// do nothing
	info << "[WARNING] Did not find 'opdet_lv_name'. Proceeding without OpDetSD assignment." << newline;
        continue;
      }
    }

    // LOOP OVER THE DIFFERENT SD TYPES
    for ( auto const& sdname : sdname_v ) {

      info << "Configure SD group [" << sdname << "]" << newline;
      DBLinkGroup lsdconfig = DB::Get()->GetLinkGroup("SDCONFIG:"+sdname);
      
      // get the DB block
      DBLinkGroup::iterator j_table;
      for (j_table = lsdconfig.begin(); j_table != lsdconfig.end(); ++j_table) {
        DBLinkPtr ptable_j = j_table->second;
        try {
          std::string name_j = ptable_j->GetS("simplesdname");
          info << name_j << newline;
          if ( name_j==sdname ) {
            info << "SDCONFIG for " << name_j << " found" << newline;            
            break;
          }
        }
        catch (...) {
          Log::Die( "Missing 'simplesdname' entry in SDCONFIG block.");
        }
      }
      if ( j_table==lsdconfig.end() ) {
        Log::Die( "DID NOT find SD config block.");
      }
      
      // get name of logical volumes which will be sensitive
      DBLinkPtr ptable_sd = j_table->second;
      std::vector<std::string> opdet_lv_names;
      try {
        opdet_lv_names = ptable_sd->GetSArray("lv_names");
      }
      catch (DBNotFoundError &e) {
        try {
          std::string opdet_lv_name = ptable_sd->GetS("lv_names");
          opdet_lv_names.push_back( opdet_lv_name );
        }
        catch (DBNotFoundError &e2) {
          // do nothing
          info << "[WARNING] Did not find 'opdet_lv_name'. Proceeding without OpDetSD assignment." << newline;
        }
      }
      
      info << "Creating GLG4SimpleOpDetSD. Sensitive Logical Volumes include those whose names='";    
      for ( std::vector<std::string>::iterator it=opdet_lv_names.begin(); it!=opdet_lv_names.end(); it++ )
        info << *it << " ";
      info << "' and assigning it to SD[" << sdname << "]" << newline;
    
      // make sensitive detector
      G4SDManager* sdman = G4SDManager::GetSDMpointer();
      GLG4SimpleOpDetSD* opdetsd = new GLG4SimpleOpDetSD( sdname );
      sdman->AddNewDetector(opdetsd);

      // assign SD to logical volumes. We also make a list of physical volumes
      G4LogicalVolumeStore* lvolumes = G4LogicalVolumeStore::GetInstance();
      std::map<std::string,G4LogicalVolume*> sd_opdet_lv_v;
      int nopdets = 0;
      for ( std::vector<std::string>::iterator it=opdet_lv_names.begin(); it!=opdet_lv_names.end(); it++ ) {
        G4LogicalVolume* lv = lvolumes->GetVolume( *it );
        if (lv) {
          lv->SetSensitiveDetector( opdetsd );
          sd_opdet_lv_v[ *it ] = lv;
          nopdets++;
          //std::cout << "Assigning as OpDetSD: " << lv->GetName() << std::endl;
        }
      }
      info << "Number of logical volumes found for GLG4SimpleOpDetSD[" << sdname << "]: " << nopdets << newline;
      
    //   // find the physical volumes for these opdets
    //   std::map< std::string, G4VPhysicalVolume*> sd_opdet_pv_v;
    //   for ( std::vector<std::string>::iterator it=opdet_lv_names.begin(); it!=opdet_lv_names.end(); it++ ) {
    //     G4LogicalVolume* sd_lv = sd_opdet_lv_v[ *it ];
    //     for ( G4PhysicalVolumeStore::iterator it_pv=pvolumes->begin(); it_pv!=pvolumes->end(); it_pv++) {      
    //       G4VPhysicalVolume* volume = (*it_pv);
    //       G4LogicalVolume* lv = volume->GetLogicalVolume();
    //       if ( lv==sd_lv ) {
    //         sd_opdet_pv_v[*it] = volume;
    //         break;
    //       }
    //     }
    //   }
    //   if ( sd_opdet_pv_v.size()!=sd_opdet_lv_v.size() ) {
    //     Log::Die( "Did not find a physical volume for each logical SD volume." );      
    //   }
      
    //   // get list of strings which we will use to define optical detector channels.
    //   // they will have to contain in their daughter volumes, a sensitive detector
    //   std::vector<std::string> opchannel_pv_names;
    //   try {
    //     opchannel_pv_names = table->GetSArray("opchannel_pv_name");
    //   }
    //   catch (DBNotFoundError &e) {
    //     try {
    //       std::string opchannel_pv_name = table->GetS("opchannel_pv_name");
    //       opchannel_pv_names.push_back( opchannel_pv_name );
    //     }
    //     catch (DBNotFoundError &e2) {
    //       info << "[WARNING] Did not find 'opchannel_pv_name'." << newline;
    //     }
    //   }
    //   info << "Number of strings to search for GLG4SimpleOpDetSD Optical Channels: " << opchannel_pv_names.size() << newline;
      
    //   int nopchannels = 0;
    //   for ( G4PhysicalVolumeStore::iterator it=pvolumes->begin(); it!=pvolumes->end(); it++) {

    //   G4VPhysicalVolume* volume = (*it);
    //   //info << "parsing pv=" << volume->GetName() << " for opdet name" << newline;
      
    //   bool found = false;
    //   std::string opchan_name;
    //   for ( std::vector<std::string>::iterator it_pvname=opchannel_pv_names.begin(); it_pvname!=opchannel_pv_names.end(); it_pvname++ ) {
    //     if ( volume->GetName().contains( *it_pvname ) ) {
    //       opchan_name = *it_pvname;
    //       found = true;
    //       break;
    //     }
    //   }
      
    //   if ( !found ) 
    //     continue;
    //   //info << "  matched pv opchannel stem name: " << opchan_name << newline;
      
    //   // check if this pv contains one of the SD physical volumes
    //   std::string opdet_sd_name;
    //   bool contains_sd_lv = false;
    //   for ( auto it=sd_opdet_pv_v.begin(); it!=sd_opdet_pv_v.end(); it++ ) {
    //     if ( it->second->GetLogicalVolume()->IsAncestor( volume )
    //          || volume->GetLogicalVolume()->IsAncestor( it->second ) ) {
    //       opdet_sd_name = it->first;
    //       contains_sd_lv = true;
    //       break;
    //     }
    //   }
      
    //   if (!contains_sd_lv)
    //     continue;
    //   //info << " contains sd volume: " << opdet_sd_name << newline;
      
    //   int channelid = 0;
    //   size_t numstart = 0;
    //   size_t numend = 0;
    //   std::string pvname = volume->GetName();
    //   try {
    //     numstart = pvname.find_first_of("1234567890");
    //     numend = pvname.find_first_not_of("1234567890",numstart+1);
    //     info << "  numstart=" << numstart << " numend=" << numend << newline; 
    //     if (numstart==std::string::npos)
    //       continue;
    //     channelid = atoi( pvname.substr(numstart, numend-numstart).c_str() );
    //   }
    //   catch (std::exception& e) {
    //     std::stringstream errmsg;
    //     errmsg << "Error parsing OpDet physical volume name '" << pvname << "' for channel ID. Error: " << e.what() << std::endl;
    //     Log::Die( errmsg.str() );
    //   }
          
    //   opdetsd->AddOpDetChannel( channelid, volume );
    //   nopchannels += 1;
    //   info << "Found Optical Channel instance. PVname=" << volume->GetName() << " ChannelID=" << channelid << " CopyNo=" << volume->GetCopyNo() << newline;
      
    //   }//loop over physical volumes
    // info << "Found " << nopchannels << " Optical Channels." << newline;

    }//end of sdname loop      
  } //loop over geo links

  assert(false);
}

void DetectorConstruction::SetupGDMLSurfaces() {
  DBLinkGroup lgeo = DB::Get()->GetLinkGroup("GEO");

  // find GDML table
  DBLinkGroup::iterator i_table;
  for (i_table = lgeo.begin(); i_table != lgeo.end(); ++i_table) {
    string name = i_table->first;
    DBLinkPtr table = i_table->second;
    string type;

    // check for GDML entry
    string gdmlfilename;
    try {
      gdmlfilename = table->GetS("gdml_file");
    }
    catch (DBNotFoundError &e) {
    }
    if ( gdmlfilename!="" )
      continue;

    try {
      type = table->GetS("type");
    } catch (DBNotFoundError &e) {
      Log::Die("GeoBuilder error: volume " + name + " has no type");
    }

    if (type!="border")
      continue;

    try {
      info << "Make border: " << newline;
      GeoFactory::ConstructWithFactory(type, table);
    } catch (GeoFactoryNotFoundError &e) {
      Log::Die("GeoBuilder error: Cannot find factory for volume type "  + type);
    }
    
  }
}


}
