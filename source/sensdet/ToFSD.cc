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
                                box_geom_(0), sipm_cells_(0)
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

void ToFSD::Initialize(G4HCofThisEvent *HCE)
{
  // Create a new collection of PMT hits
  HC_ = new PetSensorHitsCollection(this->GetName(), this->GetCollectionName(0));

  G4int HCID = G4SDManager::GetSDMpointer()->
    GetCollectionID(this->GetName() + "/" + this->GetCollectionName(0));

  HCE->AddHitsCollection(HCID, HC_);
}

G4bool ToFSD::ProcessHits(G4Step *step, G4TouchableHistory *)
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

G4int ToFSD::FindID(const G4VTouchable *touchable)
{
  // This works only with FBK in PETit, and SiPM numbering is the following:
  // 100, 101, ..., 163
  // 200, 201, ..., 263
  // ...
  // 800, 801, ..., 863
  // SiPMs from 100 to 463 belong to "detection" plane,
  // the others to "coincidence" plane.

  G4int snsid = -1;
  if (box_geom_ == 1) {
    G4int motherid = touchable->GetCopyNumber(mother_depth_);

    if (sipm_cells_ == 1) {// each cell is an individual photosensor
      G4int pxlid         = touchable->GetCopyNumber(sensor_depth_);
      G4int grandmotherid = touchable->GetCopyNumber(grandmother_depth_);

      snsid = naming_order_ * grandmotherid + motherid; // this is the SiPM ID
      snsid = snsid * 10000 + pxlid; // this is the pixel ID
    } else {
      snsid = touchable->GetCopyNumber(sensor_depth_);
      snsid = naming_order_ * motherid + snsid;
    }

  } else {
    G4Exception("[TofSD]", "FindID()", FatalException,
                  "This branch works only for PETit geometries!");
  }
  return snsid;
}

void ToFSD::EndOfEvent(G4HCofThisEvent * /*HCE*/)
{
  //  int HCID = G4SDManager::GetSDMpointer()->
  //    GetCollectionID(this->GetCollectionName(0));
  //  // }
  // HCE->AddHitsCollection(HCID, _HC);
}
