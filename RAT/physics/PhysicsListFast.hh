/**
 * @class RAT::PhysicsListFast
 * @brief Defines the physics processes active in the simulation
 *
 * @author A. Mastbaum <mastbaum@hep.upenn.edu>
 *
 * @detail This physics list extends the Shielding list included with Geant4
 * to add optical processes and custom processes overridden in RAT.
 */

#ifndef __RAT_PhysicsListFast__
#define __RAT_PhysicsListFast__

#include <G4VUserPhysicsList.hh>
#include <QGSP_BERT.hh>

namespace RAT {

class PhysicsListFast : public QGSP_BERT {
public:
  PhysicsListFast();

  ~PhysicsListFast();

  // Instantiate desired Particles
  void ConstructParticle();

  // Instantiate desired Processes
  void ConstructProcess();

private:
  // Construct and register optical processes
  void ConstructOpticalProcesses();

  // Register opticalphotons with the PMT G4FastSimulationManagerProcess
  void AddParameterization();
};

}  // namespace RAT

#endif  // __RAT_PhysicsListFast__

