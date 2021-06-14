#include <RAT/geo/GeoSkinSurfaceFactory.hh>
#include <G4Material.hh>
#include <RAT/geo/Materials.hh>
#include <G4LogicalVolumeStore.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4VisAttributes.hh>
#include <sstream>
#include <iostream>

using namespace std;

namespace RAT {
  
// G4LogicalSkinSurface( const G4String& name,
// 		      G4LogicalVolume* vol,
// 		      G4SurfaceProperty* surfaceProperty );

G4VPhysicalVolume *GeoSkinSurfaceFactory::Construct(DBLinkPtr table) {
  //detail << "GeoSkinSurfaceFactory: Constructing skin surface: " << table->GetIndex() << newline;
  std::cout << "GeoSkinSurfaceFactory: Constructing skin surface: " << table->GetIndex() << std::endl;

  string skin_name = table->GetIndex();
  string lv_name;
  
  try {
    lv_name = table->GetS("logicalvolume");
  } catch (DBNotFoundError &e) {
    Log::Die("Missing 'logicalvolume' table item");
  };

  G4LogicalVolume* LV = G4LogicalVolumeStore::GetInstance()->GetVolume( lv_name );
  if ( LV==NULL ) {
    Log::Die("GeoSkinSurfaceFactory: Error building "+skin_name+", logical volume '"+lv_name+"' does not exist");
  }
  
  string surface_name = table->GetS("surface");
  if (Materials::optical_surface.count(surface_name) == 0)
    Log::Die("GeoSkinSurfaceFactory: Error building "+skin_name+", surface "+ surface_name + " does not exist");
    
  new G4LogicalSkinSurface("skinsurf_"+lv_name, LV, Materials::optical_surface[surface_name]);
 
  return 0;
}

} // namespace RAT

