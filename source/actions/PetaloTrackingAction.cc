// ----------------------------------------------------------------------------
// petalosim | PetaloTrackingAction.cc
//
// This class is the default tracking action of the PETALO simulation.
// It stores in memory the trajectories of all particles, except optical photons
// and ionization electrons, with the relevant tracking information that will be
// saved to the output file.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetaloTrackingAction.h"

#include "nexus/Trajectory.h"
#include "nexus/TrajectoryMap.h"
#include "nexus/FactoryBase.h"

#include <NESTProc.hh>

#include <G4Track.hh>
#include <G4TrackingManager.hh>
#include <G4Trajectory.hh>
#include <G4ParticleDefinition.hh>
#include <G4OpticalPhoton.hh>



using namespace nexus;

REGISTER_CLASS(PetaloTrackingAction, G4UserTrackingAction)

PetaloTrackingAction::PetaloTrackingAction(): G4UserTrackingAction()
{
}



PetaloTrackingAction::~PetaloTrackingAction()
{
}



void PetaloTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  // Do nothing if the track is an optical photon
  if (track->GetDefinition() == G4OpticalPhoton::Definition() ||
      track->GetDefinition() == NEST::NESTThermalElectron::Definition()) {
      fpTrackingManager->SetStoreTrajectory(false);
      return;
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



void PetaloTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  // Do nothing if the track is an optical photon
  if (track->GetDefinition() == G4OpticalPhoton::Definition()) return;

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
