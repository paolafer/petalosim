// ----------------------------------------------------------------------------
// petalosim | PetSensorHit.cc
//
// This class describes the charge detected by a photosensor.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetSensorHit.h"


using namespace nexus;


G4Allocator<PetSensorHit> PetSensorHitAllocator;



PetSensorHit::PetSensorHit():
  G4VHit(), sns_id_(-1.), bin_size_(0.)
{
}



PetSensorHit::PetSensorHit(G4int id, const G4ThreeVector& position, G4double bin_size):
  G4VHit(), sns_id_(id),  bin_size_(bin_size), position_(position)
{
}



PetSensorHit::~PetSensorHit()
{
}



PetSensorHit::PetSensorHit(const PetSensorHit& other): G4VHit()
{
  *this = other;
}



const PetSensorHit& PetSensorHit::operator=(const PetSensorHit& other)
{
  sns_id_    = other.sns_id_;
  bin_size_  = other.bin_size_;
  position_  = other.position_;
  histogram_ = other.histogram_;

  return *this;
}



G4int PetSensorHit::operator==(const PetSensorHit& other) const
{
  return (this==&other) ? 1 : 0;
}



void PetSensorHit::SetBinSize(G4double bin_size)
{
  if (histogram_.size() == 0) {
    bin_size_ = bin_size;
  }
  else {
    G4String msg = "A PetSensorHit cannot be rebinned once it has been filled.";
    G4Exception("[PetSensorHit]", "SetBinSize()", JustWarning, msg);
  }
}



void PetSensorHit::Fill(G4double time, G4int counts)
{
  G4double time_bin = floor(time/bin_size_) * bin_size_;
  histogram_[time_bin] += counts;
}


void PetSensorHit::AddPhoton(G4double time, G4int track_id)
{
  phot_[time] = track_id;
}

