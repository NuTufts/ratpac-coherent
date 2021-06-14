#include <RAT/geo/GeoBuilder.hh>
#include <RAT/base/Log.hh>
#include <RAT/geo/DetectorConstruction.hh>

#include <G4SDManager.hh>

#include <RAT/geo/GeoBoxFactory.hh>
#include <RAT/geo/GeoTubeFactory.hh>
#include <RAT/geo/GeoTorusFactory.hh>
#include <RAT/geo/GeoSphereFactory.hh>
#include <RAT/geo/GeoReflectorFactory.hh>
#include <RAT/geo/GeoReflectorWaveguideFactory.hh>
#include <RAT/geo/GeoPMTArrayFactory.hh>
#include <RAT/geo/GeoPMTCoverageFactory.hh>
#include <RAT/geo/GeoWaterBoxArrayFactory.hh>
#include <RAT/geo/GeoBubbleFactory.hh>
#include <RAT/geo/GeoPerfTubeFactory.hh>
#include <RAT/geo/GeoPerfSphereFactory.hh>
#include <RAT/geo/GeoRevArrayFactory.hh>
#include <RAT/geo/GeoTubeArrayFactory.hh>
#include <RAT/geo/GeoRevolutionFactory.hh>
#include <RAT/geo/GeoLensFactory.hh>
#include <RAT/geo/GeoConvexLensFactory.hh>
#include <RAT/geo/GeoPolygonFactory.hh>
#include <RAT/base/Factory.hh>
#include <RAT/geo/ConeWaveguideFactory.hh>
#include <RAT/geo/GeoRevolutionChimneyFactory.hh>
#include <RAT/geo/GeoSurfaceFactory.hh>
#include <RAT/geo/GeoSkinSurfaceFactory.hh>
#include <RAT/geo/GeoTubeIntersectionFactory.hh>
#include <RAT/geo/GeoPerfBoxFactory.hh>
#include <RAT/geo/GeoCutTubeFactory.hh>
#include <RAT/geo/GeoWatchmanShieldFactory.hh>

using namespace std;

namespace RAT {

GeoBuilder::GeoBuilder() 
{
  // Register all the standard volumes
  new GeoBoxFactory();
  new GeoTubeFactory();
  new GeoTorusFactory();
  new GeoSphereFactory();
  new GeoReflectorFactory();
  new GeoReflectorWaveguideFactory();
  new GeoPMTArrayFactory();
  new GeoPMTCoverageFactory();
  new GeoWaterBoxArrayFactory();
  new GeoBubbleFactory();
  new GeoPerfTubeFactory();
  new GeoPerfSphereFactory();
  new GeoRevArrayFactory();
  new GeoTubeArrayFactory();
  new GeoRevolutionFactory();
  new GeoRevolutionChimneyFactory();
  new GeoLensFactory();
  new GeoPolygonFactory();
  new GeoSurfaceFactory();
  new GeoSkinSurfaceFactory();
  new GeoConvexLensFactory();
  new GeoTubeIntersectionFactory();
  new GeoPerfBoxFactory();
  new GeoCutTubeFactory();
  new GeoWatchmanShieldFactory();

  // Register standard waveguides
  GlobalFactory<WaveguideFactory>::Register("cone",
      new Alloc<WaveguideFactory, ConeWaveguideFactory>);

  geo_source = RATGEOTABLES;
}

  G4VPhysicalVolume *GeoBuilder::ConstructAll(std::string geo_tablename, G4VPhysicalVolume* prebuiltWorld) 
  {
  // Get all geometry tables that have been loaded
    DBLinkGroup geo = DB::Get()->GetLinkGroup(geo_tablename);
    G4VPhysicalVolume *world = prebuiltWorld;

  // Plan: Scan list repeatedly, looking for volume with mother that
  //       already has been constructed.  This should reduce the size
  //       of geo by one each iteration if mother dependency is not
  //       circular.  (No Oedipus complexes allowed)  The case
  //       of circular dependency is detected and the program is aborted.
  //
  //       Also fail if you ever find a volume without an already built
  //       mother AND without a yet-to-be built mother.
  // tmw: 3/21/2015. Adding the ability to load GDML instead. Different GEO.
  //       gdmlfile: "filename.gdml"
  debug << "GeoBuilder: Starting ConstructAll()\n";

  while (geo.size() > 0) {

    DBLinkGroup::iterator i_table;
    for (i_table = geo.begin(); i_table != geo.end(); ++i_table) {
      string name = i_table->first;
      debug << "GeoBuilder: Checking " << name << newline;

      DBLinkPtr table = i_table->second;

      try {
      	// we don't use this here. see DetectorConstruction.cc
	std::string gdmlfilename = table->GetS("gdml_file");
	geo.erase(i_table);
	break;
      }
      catch (DBNotFoundError &e) {
       	// do nothing. keep going.
      } 

      string mother;
      string type;
      try {
        mother = table->GetS("mother");
      } catch (DBNotFoundError &e) {
        Log::Die("GeoBuilder error: volume " + name + " has no mother");
      }
      try {
        type = table->GetS("type");
      } catch (DBNotFoundError &e) {
        Log::Die("GeoBuilder error: volume " + name + " has no type");
      }
      
      // Skip disabled volumes
      int enabled = 1;
      try { enabled = table->GetI("enable"); } 
      catch (DBNotFoundError &e) { };

      if (!enabled) {
        debug << "GeoBuilder: Removing " << name << " (disabled) from geo list.\n";
        geo.erase(i_table);
        break;
      }

      if (type == "border"){
	// Border Surface Definition
        string volume1, volume2;
        try {
          volume1 = table->GetS("volume1");
        } catch (DBNotFoundError &e) {
        Log::Die("GeoBuilder error: border " + name + " has no volume1");
        }
        try {
          volume2 = table->GetS("volume2");
        } catch (DBNotFoundError &e) {
        Log::Die("GeoBuilder error: border " + name + " has no volume2");
        }
        G4VPhysicalVolume* LogVol1 = GeoFactory::FindPhysMother(volume1);
        G4VPhysicalVolume* LogVol2 = GeoFactory::FindPhysMother(volume2);

        if (LogVol1 != 0 && LogVol2 != 0) {
          try {
	    GeoFactory::ConstructWithFactory(type, table);
          } catch (GeoFactoryNotFoundError &e) {
          Log::Die("GeoBuilder error: Cannot find factory for volume type "  + type);
          }
          debug << "GeoBuilder: Removing " << name << " from geo list.\n";
          geo.erase(i_table);
          break;
        } else 
          if ((LogVol1==0 && geo.count(volume1)==0) || (LogVol2==0 && geo.count(volume2)==0)) {
              // No mother yet to be built
              Log::Die("GeoBuilder error: Cannot find "+volume1+" or "+volume2+" for " + name);
            }
      }
      else if ( type=="skin" ) {
	// Check if volume defined already before processing.
	// to d
	// make surface
	std::cout << "GeoBuilder: Skin Surface " << std::endl;
	GeoFactory::ConstructWithFactory(type, table);
	geo.erase(i_table); // finish
	break;
      }
      else{
        if (mother == "" || GeoFactory::FindMother(mother) != 0) { // Found volume to build
  
          try {
            if (mother == "")
              world = GeoFactory::ConstructWithFactory(type, table); // save world volume
	    else
              GeoFactory::ConstructWithFactory(type, table);
          } catch (GeoFactoryNotFoundError &e) {
            Log::Die("GeoBuilder error: Cannot find factory for volume type "  + type);
          }
  
          debug << "GeoBuilder: Removing " << name << " from geo list.\n";
          geo.erase(i_table);
          break;
        } else if (geo.count(mother) == 0) { // No mother yet to be built
	  Log::Die("GeoBuilder error: Cannot find mother volume " + mother
		   + " for " + name);
	}
      }//else
      
    } // end for loop looking for next volume to build

    // Circular dependency check
    if (i_table == geo.end()) {
      string err("GeoBuilder error: Circular volume dependency encountered!\n");
      for (i_table = geo.begin(); i_table != geo.end(); i_table++) {
        string name = i_table->first;
        DBLinkPtr table = i_table->second;
        err += "  " + name + " depends on " + table->GetS("mother") + "\n";
      }
      Log::Die(err);
    }

  } // end loop when all volumes built

  if (world == 0)
    Log::Die("GeoBuilder error: No world volume defined");

  return world;
}

  G4VPhysicalVolume* GeoBuilder::ConstructGDML(std::string geo_tablename) {

    std::cout << "GeoBuilder::ConstructGDML(" << geo_tablename << ")" << std::endl;

    // Get all geometry tables that have been loaded
    DBLinkGroup geo = DB::Get()->GetLinkGroup(geo_tablename);
    G4VPhysicalVolume *world = 0;

    DBLinkGroup::iterator i_table;
    for (i_table = geo.begin(); i_table != geo.end(); ++i_table) {
      string name = i_table->first;
      DBLinkPtr table = i_table->second;
      
      // look for GDML entry
      string gdmlfilename;

      // if found one, we use the GDML parser and skip the rest of this loop!
      try {
	gdmlfilename = table->GetS("gdml_file");
      }
      catch ( DBNotFoundError &e) {
	std::cout << "Could not find GDML database in this entry." << std::endl;
	continue;
	//return world;	
      }

      std::cout << "Parsing GDML File: " << gdmlfilename << std::endl;
      geo_source = GDMLFILE;
      // we need the glg4data and expriment to get right folder
      string glg4data = "";
      if (getenv("GLG4DATA") != NULL) {
	glg4data = string(getenv("GLG4DATA")) + "/";
      }
      string experiment = DB::Get()->GetLink("DETECTOR")->GetS("experiment");
      // parse the file
      gdml_parser.Read( glg4data+"/"+experiment+"/"+gdmlfilename );
      // return the world volume
      world = gdml_parser.GetWorldVolume();
      // remove from table
      geo.erase(i_table);
      break;
    }
    return world;
  }// end of construct GDML

} // namespace RAT
