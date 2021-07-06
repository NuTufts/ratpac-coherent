#ifndef __CENNS_PRIMARY_GENERATOR_ACTION_HH__
#define __CENNS_PRIMARY_GENERATOR_ACTION_HH__

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ThreeVector.hh"
#include "G4VPhysicalVolume.hh"

//#include "TROOT.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"

#include "globals.hh"

#include "marley/JSON.hh"

//forward declarations
class G4ParticleGun;
class G4Event;
class CENNSPrimaryGeneratorMessenger;
class G4ParticleTable;

namespace marley {
  class JSONConfig;
  class Generator;
}

class CENNSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  CENNSPrimaryGeneratorAction();
  virtual ~CENNSPrimaryGeneratorAction();

  void GeneratePrimaries(G4Event *event);
  void SetGenerator(G4String);
  void SetNuFlavor(G4String);
  void GenerateCEvNS(G4Event* event);
  void GenerateCosmics(G4Event* event);
  void Generate83mKr(G4Event* event);
  void GenerateMarley(G4Event* event);
  void GenerateFromJSON(G4Event* event);
  
  G4double PullNuEnergy(G4String nuFlavor);
  G4double SampleNuEnergy();
  G4double SampleNuTime();
  void GetPositionInLAr(G4ThreeVector& genpos, G4double xmax, G4double ymax, G4double zmax, bool cylindrical_vol=false );
  void SetParticleMomentum(G4ThreeVector& momentum, G4double ar_angle, G4double phi);
  void GetCosmicPosition(G4ThreeVector& genPos);
  void SetXCosmicPos(G4double xpos);
  void SetYCosmicPos(G4double ypos);
  void SetZCosmicPos(G4double zpos);
  void SetRadCosmic(G4double radCosmic);
  void SetMarleyConfig(std::string marley_cfg) { _MarleyJSON_filepath=marley_cfg; };
  void SetJSONeventFile(std::string json_file) { _json_event_file=json_file; };  

private:
  G4GeneralParticleSource* fParticleGun;    // For normal work need using gps
  CENNSPrimaryGeneratorMessenger* fGunMessenger;
  G4String fGenerator;
  G4String fNuFlavor;
  G4double fXCosmic;
  G4double fYCosmic;
  G4double fZCosmic;
  G4double fRadCosmic;
  G4ParticleGun* fPartGenerator;

  // variables to avoid loading file multiple times
  TFile* _recoilfile;
  TH2D*  _energydist;
  TH1D*  _energyspect;

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
#endif /* __CENNS_PRIMARY_GENERATOR_ACTION_HH__ */
