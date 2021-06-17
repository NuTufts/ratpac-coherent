#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
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

      /// get the channel id offset
      int channel_id_offset = 0;
      try {
        channel_id_offset = ptable_sd->GetI("copynooffset");
      }
      catch (...) {
        channel_id_offset = 0;
      }

      // now get the list of mother pv we'll use to define the channel number
      std::vector<std::string> copyno_pvname_v;
      std::vector<int> copyno_stride_v;
      try {
        copyno_pvname_v = ptable_sd->GetSArray("copynopvnames");
        copyno_stride_v = ptable_sd->GetIArray("copynostrides");
      }
      catch (...) {          
      }      
      if ( copyno_pvname_v.size()!=copyno_stride_v.size() ) {
        Log::Die("copynopvnames and copynostrides had different number of entries");
      }
      
      // make sensitive detector
      G4SDManager* sdman = G4SDManager::GetSDMpointer();
      GLG4SimpleOpDetSD* opdetsd = new GLG4SimpleOpDetSD( sdname, copyno_pvname_v );
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
      
      // find the physical volumes for each of the logical volumes
      std::map< std::string, G4VPhysicalVolume*> sd_opdet_pv_v;
      for ( std::vector<std::string>::iterator it=opdet_lv_names.begin(); it!=opdet_lv_names.end(); it++ ) {
        G4LogicalVolume* sd_lv = sd_opdet_lv_v[ *it ];
        for ( G4PhysicalVolumeStore::iterator it_pv=pvolumes->begin(); it_pv!=pvolumes->end(); it_pv++) {      
          G4VPhysicalVolume* volume = (*it_pv);
          G4LogicalVolume* lv = volume->GetLogicalVolume();
          if ( lv==sd_lv ) {
            sd_opdet_pv_v[*it] = volume;
            break;
          }
        }
      }
      if ( sd_opdet_pv_v.size()!=sd_opdet_lv_v.size() ) {
        Log::Die( "Did not find a physical volume for each logical SD volume." );      
      }
      
      // get list of strings which we will use to define optical detector channels.
      // they will have to contain in their daughter volumes, a sensitive detector
      std::vector<std::string> opchannel_pv_names;
      try {
        opchannel_pv_names = ptable_sd->GetSArray("pv_names");
      }
      catch (DBNotFoundError &e) {
        try {
          std::string opchannel_pv_name = ptable_sd->GetS("pv_names");
          opchannel_pv_names.push_back( opchannel_pv_name );
        }
        catch (DBNotFoundError &e2) {
          Log::Die("[WARNING] Did not find 'pv_names' parameter in configuration block.");
        }
      }
      info << "Number of strings to search for GLG4SimpleOpDetSD Optical Channels: " << opchannel_pv_names.size() << newline;
      
      // Now we have the job of finding the PV for the sensitive volumes and assigning each a channel ID
      // In order to support a heirarchical description of the geometry, we need to calculate
      // a channel ID based on the chain of mother volumes an instance of a sensitive detector is found in
      // this is specified by the user through:
      // copynopvname, copynostrides, copynooffsets
      // the first two keys in the configuation block are arrays. they define strides.          
      std::vector< std::vector< G4VPhysicalVolume* > > upstream_pv_vv( copyno_pvname_v.size() );      
      if ( copyno_pvname_v.size()>0 ) {
        // now we need to search for all physical volumes with these names
        int ipv = 0;
        for ( auto const& upstream_pvname : copyno_pvname_v ) {
          for ( G4PhysicalVolumeStore::iterator it_pv2=pvolumes->begin(); it_pv2!=pvolumes->end(); it_pv2++) {
            if ( (*it_pv2)->GetName().contains( upstream_pvname ) ) {
              upstream_pv_vv[ipv].push_back( *it_pv2 );
            }
          }
          info << "SD[" << sdname << "] num of PV containing '" << upstream_pvname << "': " << upstream_pv_vv[ipv].size() << newline;
          ipv++;
        }        
      }

      int idepth = 0;
      std::vector<int> index_v( upstream_pv_vv.size(), 0 );
      std::vector< std::vector< G4VPhysicalVolume* > > combo_vv;      
      _recursive_assign_channelid( idepth,
                                   index_v,
                                   upstream_pv_vv,
                                   combo_vv );
      info << "SD[" << sdname << "] formed " << combo_vv.size() << " combinations" << newline;

      // now we check each one and calculate a channel id
      int nopchannels = 0;
      for ( auto& combo_v : combo_vv ) {
        bool isgood = true;
        for (int ipv=0; ipv<(int)upstream_pv_vv.size(); ipv++) {
          auto& pv1 = combo_v[ipv];
          if ( ipv+1<(int)upstream_pv_vv.size() ) {
            // is the next volume a daugher?
            auto& pv2= combo_v[ipv+1];
            if ( !pv1->GetLogicalVolume()->IsAncestor( pv2 ) ) {
              isgood = false;
              info << pv1->GetName() << " is not ancestor of " << pv2->GetName() << newline;
            }
          }

          bool contains_sd_lv = false;
          for ( auto it=sd_opdet_pv_v.begin(); it!=sd_opdet_pv_v.end(); it++ ) {
            if ( pv1->GetLogicalVolume()->IsAncestor( it->second )
                 || pv1->GetLogicalVolume()==sd_opdet_lv_v[it->first] ){
              contains_sd_lv = true;
              break;
            }
          }          
          if ( !contains_sd_lv ) {
            info << pv1->GetName() << " does not contain an OpDet PV " << newline;
            isgood = false;
          }
          if (!isgood) break;          
        }

        if (isgood) {
          // passes hierarchy test. make channel id and register in opdetsd
          // info << "combo passed: (";
          // for (int i=0; i<(int)combo_v.size(); i++) {
          //   info << combo_v[i];
          //   if ( i+1<(int)combo_v.size() )
          //     info << ", ";
          // }
          // info << ")" << newline;
          
          int channelid = channel_id_offset;
          int stride = 1;
          std::stringstream zcalc;
          zcalc << "ID = " << channel_id_offset << "(offset) ";
          std::vector<int> copyno_v(upstream_pv_vv.size(),0);
          for (int ipv=(int)upstream_pv_vv.size()-1; ipv>=0; ipv--) {
            int copyno = combo_v[ipv]->GetCopyNo();
            channelid += stride*copyno;
            zcalc << " + " << stride << "*" << copyno;
            stride *= copyno_stride_v[ipv];            
            copyno_v[ipv] = copyno;
          }
          zcalc << " = " << channelid;
          //info << zcalc.str() << newline;
          
          // info << "Assign copyno index (";
          // for (int i=0; i<(int)copyno_v.size(); i++) {
          //   info << copyno_v[i];
          //   if ( i+1<(int)copyno_v.size() )
          //     info << ",";
          // }
          // info << ") to channelid " << channelid << newline;
          
          opdetsd->AddOpDetChannel( channelid, copyno_v, combo_v.back() );
          nopchannels += 1;          
        }
      }
                                   
      info << "SD[" << sdname << "] Defined " << nopchannels << " Optical Channels." << newline;

    }//end of sdname loop      
  } //loop over geo links  
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

 void DetectorConstruction::_recursive_assign_channelid( int& idepth,
                                                         std::vector<int>& index_v,
                                                         std::vector< std::vector< G4VPhysicalVolume* > >& upstream_pv_vv,
                                                         std::vector< std::vector< G4VPhysicalVolume* > >& combo_vv )
 {

   // info << "depth=" << idepth << " index=(";
   // for (int i=0; i<(int)upstream_pv_vv.size(); i++)
   //   info << index_v[i] << ",";
   // info << ")" << newline;
   
   if ( idepth==(int)upstream_pv_vv.size() ) {
     // at leaf
     // formed a complete combo
     std::vector< G4VPhysicalVolume* > combo_v( index_v.size(), nullptr );
     for ( size_t i=0; i<index_v.size(); i++) {
       combo_v[i] = upstream_pv_vv[i][index_v[i]];
     }
     // info << " make combo: ";
     // for (int i=0; i<(int)upstream_pv_vv.size(); i++)
     //   info << index_v[i] << ",";
     // info << ")" << newline;     
     combo_vv.push_back( combo_v );
   }
   else if (idepth<upstream_pv_vv.size()) {
     // non-leaf
     // go through daughters
     for (int i=0; i<(int)upstream_pv_vv[idepth].size(); i++) {
       index_v[idepth] = i;
       idepth++;
       _recursive_assign_channelid( idepth, index_v, upstream_pv_vv, combo_vv );
     }
   }
   idepth--;
   //std::cin.get();
   return;
 }


}
