// ----------------------------------------------------------------------------
// petalosim | PetAnalysisTrackingAction.cc
//
// This class produces a csv file with histogram information of the wavelengths
// and the time of production of the optical photons.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetAnalysisTrackingAction.h"

#include "nexus/Trajectory.h"
#include "nexus/TrajectoryMap.h"
#include "nexus/FactoryBase.h"

#include <G4Track.hh>
#include <G4TrackingManager.hh>
#include <G4Trajectory.hh>
#include <G4ParticleDefinition.hh>
#include <G4OpticalPhoton.hh>
#include <G4Electron.hh>
#include <G4GenericMessenger.hh>
#include <G4AnalysisManager.hh>


using namespace nexus;

REGISTER_CLASS(PetAnalysisTrackingAction, G4UserTrackingAction)

PetAnalysisTrackingAction::PetAnalysisTrackingAction(): G4UserTrackingAction()
{
}



PetAnalysisTrackingAction::~PetAnalysisTrackingAction()
{
}



void PetAnalysisTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  // if ( track->GetCreatorProcess())
  //   G4cout << track->GetCreatorProcess()->GetProcessName()  << G4endl;
  // Do nothing if the track is an optical photon

  if (track->GetDefinition() == G4OpticalPhoton::Definition()) {

    auto g4_analysis_man = G4AnalysisManager::Instance();

    if (track->GetCreatorProcess()->GetProcessName() == "Cerenkov") {
      //track->CalculateVelocityForOpticalPhoton()
      g4_analysis_man->FillH1(0, h_Planck*c_light/track->GetKineticEnergy()/nanometer);
    }
    else if (track->GetCreatorProcess()->GetProcessName() == "Scintillation") {
      g4_analysis_man->FillH1(1, h_Planck*c_light/track->GetKineticEnergy()/nanometer);
      g4_analysis_man->FillH1(2, track->GetGlobalTime()/picosecond);
    }
  }

  // Create a new trajectory associated to the track.
  // N.B. If the processesing of a track is interrupted to be resumed
  // later on (to process, for instance, its secondaries) more than
  // one trajectory associated to the track will be created, but
  // the event manager will merge them at some point.
  G4VTrajectory* trj = new Trajectory(track);

   // Set the trajectory in the tracking manager
  fpTrackingManager->SetStoreTrajectory(true);
  fpTrackingManager->SetTrajectory(trj);
 }



void PetAnalysisTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  Trajectory* trj = (Trajectory*) TrajectoryMap::Get(track->GetTrackID());

  // Do nothing if the track has no associated trajectory in the map
  if (!trj) return;

  // Record final time and position of the track
  trj->SetFinalPosition(track->GetPosition());
  trj->SetFinalTime(track->GetGlobalTime());
  trj->SetTrackLength(track->GetTrackLength());
  trj->SetFinalVolume(track->GetVolume()->GetName());
  trj->SetFinalMomentum(track->GetMomentum());

  // Record last process of the track
  G4String proc_name = track->GetStep()->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
  trj->SetFinalProcess(proc_name);
}
