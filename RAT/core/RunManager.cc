#include <RAT/core/RunManager.hh>
#include <RAT/base/ProcBlock.hh>
#include <RAT/core/Gsim.hh>
#include <RAT/physics/PhysicsList.hh>
#include <RAT/physics/PhysicsListFast.hh>
#include <G4RunManager.hh>

#ifdef G4VIS_USE
#include <GLG4VisManager.hh>
#endif

namespace RAT {

RunManager::RunManager() {
  mainBlock = NULL;
  Init();
}

RunManager::RunManager(ProcBlock* theMainBlock) {
  mainBlock = theMainBlock;
  Init();
}

void RunManager::Init() {
  theRunManager = new G4RunManager;  // Manages GEANT4 simulation process
  
  // Particle transport and interactions.  Note that this has to be
  // created outside of Gsim, since the physics list must be
  // initialized before the user tracking action.
  //theRunManager->SetUserInitialization(new PhysicsList());
  theRunManager->SetUserInitialization(new PhysicsListFast());

  // Create the  simulation manager.
  ratGsim = new Gsim(mainBlock);
  
  // Visualization, only if you choose to have it!
#ifdef G4VIS_USE
  theVisManager = new GLG4VisManager();
  theVisManager -> Initialize();
#endif
}

RunManager::~RunManager() {
  delete ratGsim;
#ifdef G4VIS_USE
  delete theVisManager;
#endif
}

}  // namespace RAT

