#include <QGSP_BERT.hh>
#include <G4VUserPhysicsList.hh>
#include <G4FastSimulationManagerProcess.hh>
#include <G4OpticalPhoton.hh>
#include <G4OpRayleigh.hh>
#include <G4ParticleDefinition.hh>
#include <G4ProcessManager.hh>
#include <G4Cerenkov.hh>
#include <G4OpBoundaryProcess.hh>
#include <RAT/GLG4OpAttenuation.hh>
#include <RAT/GLG4Scint.hh>
#include <RAT/GLG4SteppingAction.hh>
#include <RAT/PhysicsListFast.hh>

namespace RAT {

PhysicsListFast::PhysicsListFast() : QGSP_BERT() {}

PhysicsListFast::~PhysicsListFast() {}

void PhysicsListFast::ConstructParticle() {
  QGSP_BERT::ConstructParticle();
  G4OpticalPhoton::OpticalPhotonDefinition();
}

void PhysicsListFast::ConstructProcess() {
  AddParameterization();
  QGSP_BERT::ConstructProcess();
  ConstructOpticalProcesses();
}

void PhysicsListFast::ConstructOpticalProcesses() {
  // Cherenkov: default G4Cerenkov
  //
  // Request that Cerenkov photons be tracked first, before continuing
  // originating particle step.  Otherwise, we get too many secondaries!
  G4Cerenkov* cerenkovProcess = new G4Cerenkov();
  cerenkovProcess->SetTrackSecondariesFirst(true);
  cerenkovProcess->SetMaxNumPhotonsPerStep(4);

  // Attenuation: RAT's GLG4OpAttenuation
  //
  // GLG4OpAttenuation implements Rayleigh scattering.
  // G4OpRayleigh is not used for the following two reasons:
  //   1) It doesn't even try to work for anything other than water.
  //   2) It doesn't actually work for water, either.
  GLG4OpAttenuation* attenuationProcess = new GLG4OpAttenuation();

  // Scintillation: RAT's GLG4Scint
  //
  // Create three scintillation processes which depend on the mass.
  G4double protonMass = G4Proton::Proton()->GetPDGMass();
  G4double alphaMass = G4Alpha::Alpha()->GetPDGMass();
  GLG4Scint* defaultScintProcess = new GLG4Scint();
  GLG4Scint* nucleonScintProcess = new GLG4Scint("nucleon", 0.9 * protonMass);
  GLG4Scint* alphaScintProcess = new GLG4Scint("alpha", 0.9 * alphaMass);

  // Optical boundary processes: default G4
  G4OpBoundaryProcess* opBoundaryProcess = new G4OpBoundaryProcess();

  // Set verbosity
  if (verboseLevel > 0) {
    cerenkovProcess->DumpInfo();
    attenuationProcess->DumpInfo();
    defaultScintProcess->DumpInfo();
    nucleonScintProcess->DumpInfo();
    alphaScintProcess->DumpInfo();
    opBoundaryProcess->DumpInfo();
  }

  cerenkovProcess->SetVerboseLevel(verboseLevel-1);
  attenuationProcess->SetVerboseLevel(verboseLevel-1);
  defaultScintProcess->SetVerboseLevel(verboseLevel-1);
  nucleonScintProcess->SetVerboseLevel(verboseLevel-1);
  alphaScintProcess->SetVerboseLevel(verboseLevel-1);
  opBoundaryProcess->SetVerboseLevel(verboseLevel-1);
  //opBoundaryProcess->SetVerboseLevel(5);

  // Apply processes to all particles where applicable
#if (G4VERSION_NUMBER>1021)
  G4ParticleTable::G4PTblDicIterator* theParticleIterator =  G4ParticleTable::GetParticleTable()->GetIterator();
#endif
  theParticleIterator->reset();
  while((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (cerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(cerenkovProcess);
      pmanager->SetProcessOrdering(cerenkovProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      pmanager->AddDiscreteProcess(attenuationProcess);
      pmanager->AddDiscreteProcess(opBoundaryProcess);
      pmanager->AddDiscreteProcess( new  G4OpRayleigh );
    }
  }
}

void PhysicsListFast::AddParameterization() {
  G4FastSimulationManagerProcess* fastSimulationManagerProcess =
    new G4FastSimulationManagerProcess();
#if (G4VERSION_NUMBER>1021)
  G4ParticleTable::G4PTblDicIterator* theParticleIterator =  G4ParticleTable::GetParticleTable()->GetIterator();
#endif
  theParticleIterator->reset();
  while((*theParticleIterator)()) {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (particle->GetParticleName() == "opticalphoton") {
      pmanager->AddProcess(fastSimulationManagerProcess, -1, -1, 1);
    }
  }
}

}  // namespace RAT

