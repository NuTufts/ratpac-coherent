#include <RAT/geo/GeoTubeFactory.hh>
#include <G4Tubs.hh>
#include <CLHEP/Units/SystemOfUnits.h>
#include <CLHEP/Units/PhysicalConstants.h>

using namespace std;

namespace RAT {

G4VSolid *GeoTubeFactory::ConstructSolid(DBLinkPtr table)
{
  string volume_name = table->GetIndex();
  G4double r_max = table->GetD("r_max") * CLHEP::mm;
  G4double size_z = table->GetD("size_z") * CLHEP::mm;

  // Optional parameters
  G4double r_min = 0.0;
  try { r_min = table->GetD("r_min") * CLHEP::mm; } 
  catch (DBNotFoundError &e) { };
  G4double phi_start = 0.0;
  try { phi_start = table->GetD("phi_start") * CLHEP::deg; } 
  catch (DBNotFoundError &e) { };
  G4double phi_delta = CLHEP::twopi;
  try { phi_delta = table->GetD("phi_delta") * CLHEP::deg; } 
  catch (DBNotFoundError &e) { };


  return new G4Tubs(volume_name,
		    r_min, r_max, size_z,
		    phi_start, phi_delta);
}

} // namespace RAT
