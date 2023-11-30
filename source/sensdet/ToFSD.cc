// ----------------------------------------------------------------------------
// petalosim | ToFSD.cc
//
// This class is the sensitive detector used for PETALO.
// The first time a photoelectron is detected by a particular sensor,
// a PetSensorHit instance is created with two tasks:
// storing the total charge response and the times of the individual photons.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "ToFSD.h"

#include <G4OpticalPhoton.hh>
#include <G4SDManager.hh>
#include <G4ProcessManager.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4RunManager.hh>

using namespace CLHEP;

ToFSD::ToFSD(G4String sdname) : G4VSensitiveDetector(sdname),
                                naming_order_(0), sensor_depth_(0),
                                mother_depth_(0),
                                box_conf_(def), sipm_cells_(false)
{
  // Register the name of the collection of hits
  collectionName.insert(GetCollectionUniqueName());
}

ToFSD::~ToFSD()
{
}

G4String ToFSD::GetCollectionUniqueName()
{
  return "PetSensorHitsCollection";
}

void ToFSD::Initialize(G4HCofThisEvent* HCE)
{
  // Create a new collection of PMT hits
  HC_ = new PetSensorHitsCollection(this->GetName(), this->GetCollectionName(0));

  G4int HCID = G4SDManager::GetSDMpointer()->
    GetCollectionID(this->GetName() + "/" + this->GetCollectionName(0));

  HCE->AddHitsCollection(HCID, HC_);
}

G4bool ToFSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  // Check whether the track is an optical photon
  G4ParticleDefinition *pdef = step->GetTrack()->GetDefinition();
  if (pdef != G4OpticalPhoton::Definition())
    return false;

  const G4VTouchable *touchable =
    step->GetPostStepPoint()->GetTouchable();

  G4int sns_id = FindID(touchable);

  PetSensorHit* hit = 0;
  for (size_t i = 0; i < HC_->entries(); i++)
    {
      if ((*HC_)[i]->GetSnsID() == sns_id)
        {
          hit = (*HC_)[i];
          break;
        }
    }

  // If no hit associated to this sensor exists already,
  // create it and set main properties
  if (!hit)
    {
      hit = new PetSensorHit();
      hit->SetSnsID(sns_id);
      hit->SetPosition(touchable->GetTranslation());
      HC_->insert(hit);
    }

  hit->counts_ += 1;
  G4double time = step->GetPostStepPoint()->GetGlobalTime();
  hit->AddPhoton(time, step->GetTrack()->GetTrackID());

  return true;
}

G4int ToFSD::FindID(const G4VTouchable* touchable)
{
  // This is valid for full-body PET and for PETit with FBK-only
  G4int snsid = touchable->GetCopyNumber(sensor_depth_);
  if (naming_order_ != 0)
  {
    G4int motherid = touchable->GetCopyNumber(mother_depth_);
    snsid = naming_order_ * motherid + snsid;
  }

  if (sipm_cells_) {
    G4int pxlid         = touchable->GetCopyNumber(sensor_depth_);
    G4int motherid      = touchable->GetCopyNumber(mother_depth_);
    G4int grandmotherid = touchable->GetCopyNumber(grandmother_depth_);

    snsid = naming_order_ * grandmotherid + motherid; // this is the SiPM ID
    snsid = snsid * 10000 + pxlid; // this is the pixel ID
  }

  if (box_conf_ == hama)
  { // Hamamatsu 2x2 and FBK centered
    std::vector<G4int> init_ids({0, 4, 40, 44, 100, 104, 140, 144});
    G4int motherid = touchable->GetCopyNumber(mother_depth_);
    if (sipm_cells_) {
      G4int pxlid         = touchable->GetCopyNumber(sensor_depth_);
      G4int grandmotherid = touchable->GetCopyNumber(grandmother_depth_);
      G4int first_id = (init_ids)[grandmotherid];
      snsid = first_id + motherid; // this is the SiPM ID
      snsid = snsid * 100000 + pxlid; // this is the ID of each microcell
    } else {
      G4int first_id = (init_ids)[motherid];
      snsid = first_id + snsid;
    }
  }

  return snsid;
}

void ToFSD::EndOfEvent(G4HCofThisEvent* /*HCE*/)
{
  //  int HCID = G4SDManager::GetSDMpointer()->
  //    GetCollectionID(this->GetCollectionName(0));
  //  // }
  // HCE->AddHitsCollection(HCID, _HC);
}
