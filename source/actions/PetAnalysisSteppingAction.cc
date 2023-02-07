// ----------------------------------------------------------------------------
// petalosim | PetAnalysisSteppingAction.cc
//
// This class allows the user to print the total number of photons detected by
// all kinds of photosensors at the end of the run.
// It produces csv files with histogram information of photon velocities
// and wavelengths.
// It also shows examples of information that can be accessed at the stepping
// level, useful for debugging.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetAnalysisSteppingAction.h"

#include "nexus/FactoryBase.h"

#include <G4Step.hh>
#include <G4SteppingManager.hh>
#include <G4ProcessManager.hh>
#include <G4OpticalPhoton.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4Electron.hh>
#include <G4GenericMessenger.hh>
#include <G4AnalysisManager.hh>

namespace nexus {}
using namespace nexus;
using namespace CLHEP;

REGISTER_CLASS(PetAnalysisSteppingAction, G4UserSteppingAction)

PetAnalysisSteppingAction::PetAnalysisSteppingAction(): G4UserSteppingAction()
{
  detected = 0;
  not_det = 0;
}



PetAnalysisSteppingAction::~PetAnalysisSteppingAction()
{
  G4cout << "Detected photons = " << detected << G4endl;
  G4cout << "Non detected photons = " << not_det << G4endl;

  G4double total_counts = 0;
  detectorCounts::iterator it = my_counts.begin();
  while (it != my_counts.end()) {
    G4cout << "Detector " << it->first << ": " << it->second << " counts" << G4endl;
    total_counts += it->second;
    it ++;
  }
  G4cout << "TOTAL COUNTS: " << total_counts << G4endl;
}



void PetAnalysisSteppingAction::UserSteppingAction(const G4Step* step)
{
  G4ParticleDefinition* pdef = step->GetTrack()->GetDefinition();

  //  Check whether the track is an optical photon
  if (pdef != G4OpticalPhoton::Definition()) return;


  G4StepPoint* point1 = step->GetPreStepPoint();
  G4StepPoint* point2 = step->GetPostStepPoint();
  G4TouchableHandle touch1 = point1->GetTouchableHandle();
  G4TouchableHandle touch2 = point2->GetTouchableHandle();
  G4String vol1name = touch1->GetVolume()->GetName();
  G4String vol2name = touch2->GetVolume()->GetName();
  //G4Track* track = step->GetTrack();

  //G4String proc_name = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  //G4int copy_no = step->GetPostStepPoint()->GetTouchable()->GetReplicaNumber(1);

  // Retrieve the pointer to the optical boundary process.
  // We do this only once per run defining our local pointer as static.
  static G4OpBoundaryProcess* boundary = 0;

  if (!boundary) { // the pointer is not defined yet
    // Get the list of processes defined for the optical photon
    // and loop through it to find the optical boundary process.
    G4ProcessVector* pv = pdef->GetProcessManager()->GetProcessList();
    for (size_t i=0; i<pv->size(); i++) {
      if ((*pv)[i]->GetProcessName() == "OpBoundary") {
	boundary = (G4OpBoundaryProcess*) (*pv)[i];
	break;
      }
    }
  }

  if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {

    // if boundary->GetStatus() == 2 in SiPMpet refraction takes place
    if (boundary->GetStatus() == Detection) {
	detected = detected + 1;
	double distance =
	  std::pow(point2->GetPosition().getX() - point1->GetPosition().getX(), 2) +
	  std::pow(point2->GetPosition().getY() - point1->GetPosition().getY(), 2)  + std::pow(point2->GetPosition().getZ() - point1->GetPosition().getZ(), 2) ;
	distance = std::sqrt(distance);
	G4double lambda = h_Planck*c_light/step->GetTrack()->GetKineticEnergy()/nanometer;

        auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->FillH1(4, lambda);

	G4String detector_name = step->GetPostStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
	//G4cout << "##### Sensitive Volume: " << detector_name << G4endl;

	detectorCounts::iterator it = my_counts.find(detector_name);
	if (it != my_counts.end()) my_counts[it->first] += 1;
	else my_counts[detector_name] = 1;
      } else {
	not_det = not_det + 1;
      }

  }

  return;
}
