// ----------------------------------------------------------------------------
// petalosim | PetaloPhysics.cc
//
// This class registers any new physics process defined in petalosim.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetaloPhysics.h"
#include "WavelengthShifting.h"
#include "PositronAnnihilation.h"
#include "PetaloPersistencyManager.h"

#include <NESTProc.hh>
#include <VDetector.hh>
#include <PetaloDetector.hh>
#include <PetitDetector.hh>

#include <G4Cerenkov.hh>
#include <G4Scintillation.hh>
#include <G4GenericMessenger.hh>
#include <G4OpticalPhoton.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4ProcessManager.hh>
#include <G4ProcessTable.hh>
#include <G4StepLimiter.hh>
#include <G4FastSimulationManagerProcess.hh>
#include <G4PhysicsConstructorFactory.hh>

/// Macro that allows the use of this physics constructor
/// with the generic physics list
G4_DECLARE_PHYSCONSTR_FACTORY(PetaloPhysics);

PetaloPhysics::PetaloPhysics() : G4VPhysicsConstructor("PetaloPhysics"), max_beta_(0.1),
                                 risetime_(false), noCompt_(false),
                                 nest_(false), prod_th_el_(false),
                                 petalo_detector_("FullRing")
{
  msg_ = new G4GenericMessenger(this, "/PhysicsList/Petalo/",
                                "Control commands of the nexus physics list.");

  msg_->DeclareProperty("max_beta_change", max_beta_,
                        "Maximum relative change of the beta value per step.");

  msg_->DeclareProperty("scintRiseTime", risetime_,
                        "Must be true if LYSO is used");

  msg_->DeclareProperty("offCompt", noCompt_,
                        "If true, switches off Compton Scattering.");

  msg_->DeclareProperty("nest", nest_,
                        "If true, NEST is used for scintillation.");

  msg_->DeclareProperty("thermal_electrons", prod_th_el_,
                        "If true, NEST thermal electrons are produced.");

  msg_->DeclareProperty("petalo_detector", petalo_detector_,
                        "Detector geometry chosen.");
}

PetaloPhysics::~PetaloPhysics()
{
  delete msg_;
  delete wls_;
}

void PetaloPhysics::ConstructParticle()
{
  G4OpticalPhoton::Definition();
  NEST::NESTThermalElectron::Definition();
}

void PetaloPhysics::ConstructProcess()
{
  G4ProcessManager *pmanager = 0;

  // Add our own wavelength shifting process for the optical photon
  pmanager = G4OpticalPhoton::Definition()->GetProcessManager();
  if (!pmanager)
  {
    G4Exception("ConstructProcess()", "[PetaloPhysics]", FatalException,
                "G4OpticalPhoton without a process manager.");
  }

  wls_ = new WavelengthShifting();
  pmanager->AddDiscreteProcess(wls_);

  pmanager = G4Positron::Definition()->GetProcessManager();

  // Remove Geant4 annihilation process
  G4VProcess* eplusAnnihilation =
    G4ProcessTable::GetProcessTable()->FindProcess("annihil", G4Positron::Definition());
  pmanager->RemoveProcess(eplusAnnihilation);
  // Add our custom-made process
  pos_annihil_ = new PositronAnnihilation();
  pmanager->AddDiscreteProcess(pos_annihil_);
  pmanager->SetProcessOrderingToFirst(pos_annihil_, idxAtRest);

  // Play with Cerenkov parameters
  pmanager = G4Electron::Definition()->GetProcessManager();
  G4Cerenkov* theCerProcess =
        (G4Cerenkov*)G4ProcessTable::GetProcessTable()->FindProcess("Cerenkov",
                                                                    G4Electron::Definition());
  theCerProcess->SetMaxBetaChangePerStep(max_beta_);
  G4cout << "Max beta change for step = " << theCerProcess->GetMaxBetaChangePerStep() / CLHEP::perCent << " %" << G4endl;

  // Add rise time to scintillation
  if (risetime_)
  {
    pmanager = G4Electron::Definition()->GetProcessManager();
    G4Scintillation *theScintillationProcess =
        (G4Scintillation *)G4ProcessTable::GetProcessTable()->FindProcess("Scintillation",
                                                                          G4Electron::Definition());
    theScintillationProcess->SetFiniteRiseTime(true);
  }

  if (noCompt_)
  {
    pmanager = G4Gamma::Definition()->GetProcessManager();
    G4VProcess *cs =
      G4ProcessTable::GetProcessTable()->FindProcess("compt", G4Gamma::Definition());
    pmanager->RemoveProcess(cs);
  }


  if (nest_) {
    if (petalo_detector_ == "FullRing"){
      petalo_ = new PetaloDetector();
    } else if (petalo_detector_ == "Petit") {
      petalo_ = new PetitDetector();
    } else {
      G4Exception("[PetaloPhysics]", "ConstructProcess()", FatalException,
                  "Unknown PETALO detector!");
    }

    G4double e_field = petalo_->FitEF(0, 0, 0);
    PetaloPersistencyManager* pm =
      dynamic_cast<PetaloPersistencyManager*>(G4VPersistencyManager::GetPersistencyManager());
    pm->SetElectricField(e_field);
    NEST::NESTcalc* petaloCalc = new NEST::NESTcalc(petalo_);
    NEST::NESTProc* theNESTScintillationProcess =
      new NEST::NESTProc("S1", fElectromagnetic, petaloCalc, petalo_);
    theNESTScintillationProcess->SetDetailedSecondaries(true); // this is to use the full scintillation spectrum of LXe.
    theNESTScintillationProcess->SetStackElectrons(prod_th_el_); //false if only light is collected

    auto aParticleIterator = GetParticleIterator();
    aParticleIterator->reset();
    while ((*aParticleIterator)()) {
      G4ParticleDefinition* particle = aParticleIterator->value();
      if (theNESTScintillationProcess->IsApplicable(*particle)) {
        pmanager = particle->GetProcessManager();
        pmanager->AddProcess(theNESTScintillationProcess, ordDefault+1, ordInActive, ordDefault+1);
      }
    }
  }

}
