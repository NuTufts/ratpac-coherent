#ifndef __RAT_VERTEXGEN_MARLEY_HH__
#define __RAT_VERTEXGEN_MARLEY_HH__

#include "GLG4VertexGen.hh"

//#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "globals.hh"

#include "marley/JSON.hh"

//forward declarations
namespace marley {
  class JSONConfig;
  class Generator;
}


namespace RAT {
  
class VertexGen_Marley : public GLG4VertexGen
{

public:

  VertexGen_Marley(const char *arg_dbname="marley");
  virtual ~VertexGen_Marley();

  virtual void GeneratePrimaryVertex( G4Event *argEvent,
				      G4ThreeVector &dx,
				      G4double dt);
  // generates a primary vertex with given particle type, direction, energy,
  // and consistent polarization.
  
  virtual void SetState( G4String newValues );
  // format: dir_x dir_y dir_z
  // If dir_x==dir_y==dir_z==0, the directions are isotropic.
  
  virtual G4String GetState();
  // returns current state formatted as above
  
private:

  // Marley Event Generator
  std::string         _MarleyJSON_filepath;
  marley::JSON*       _MarleyJSON;
  marley::JSONConfig* _MarleyConfig;
  marley::Generator*  _MarleyGen;
  G4long              _MarleyNumGen;

  // JSON file event generator
  std::string         _json_event_file;
  G4long              _json_nevents_gen;
  
};

}

#endif /* __RAT_VERTEXGEN_MARLEY_HH__ */
