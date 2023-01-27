// ----------------------------------------------------------------------------
// petalosim | PETSensorHit.h
//
// This class describes the charge detected by a photosensor.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PET_SNS_HIT_H
#define PET_SNS_HIT_H

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>


namespace nexus {

  class PetSensorHit: public G4VHit
  {
  public:
    /// Default constructor
    PetSensorHit();
    /// Constructor providing the detector ID and position
    PetSensorHit(G4int ID, const G4ThreeVector& position, G4double bin_size);
    /// Copy-constructor
    PetSensorHit(const PetSensorHit&);
    /// Destructor
    ~PetSensorHit();

    /// Assignement operator
    const PetSensorHit& operator=(const PetSensorHit&);
    /// Equality operator
    G4int operator==(const PetSensorHit&) const;

    /// Memory allocation
    void* operator new(size_t);
    /// Memory deallocation
     void operator delete(void*);

    /// Returns the detector ID code
    G4int GetSnsID() const;
    /// Sets the detector ID code
    void  SetSnsID(G4int);

    /// Returns the position of the detector
    G4ThreeVector GetPosition() const;
    /// Sets the position of the detector
    void SetPosition(const G4ThreeVector&);

    /// Returns the bin size of the histogram
    G4double GetBinSize() const;
    /// Sets the bin size of the histogram. This can only be done
    /// while the histogram is empty (rebinning is not supported).
    void SetBinSize(G4double);

    /// Adds counts to a given time bin
    void Fill(G4double time, G4int counts=1);
    void AddPhoton(G4double time, G4int track_id);

    const std::map<G4double, G4int>& GetHistogram() const;
    const std::map<G4double, G4int>& GetPhotonMap() const;

  private:
    G4int sns_id_;           ///< Detector ID number
    G4double bin_size_;      ///< Size of time bin
    G4ThreeVector position_; ///< Detector position

    /// Sparse histogram with number of photons detected per time bin
    std::map<G4double, G4int> histogram_;

    /// Map with time and track id of detected photons
    std::map<G4double, G4int> phot_;
  };

} // namespace nexus


typedef G4THitsCollection<nexus::PetSensorHit> PetSensorHitsCollection;
extern G4Allocator<nexus::PetSensorHit> PetSensorHitAllocator;


// INLINE DEFINITIONS ////////////////////////////////////////////////

namespace nexus {

  inline void* PetSensorHit::operator new(size_t)
  { return ((void*) PetSensorHitAllocator.MallocSingle()); }

  inline void PetSensorHit::operator delete(void* hit)
  { PetSensorHitAllocator.FreeSingle((PetSensorHit*) hit); }

  inline G4int PetSensorHit::GetSnsID() const { return sns_id_; }
  inline void PetSensorHit::SetSnsID(G4int id) { sns_id_ = id; }

  inline G4double PetSensorHit::GetBinSize() const { return bin_size_; }

  inline G4ThreeVector PetSensorHit::GetPosition() const { return position_; }
  inline void PetSensorHit::SetPosition(const G4ThreeVector& p) { position_ = p; }

  inline const std::map<G4double, G4int>& PetSensorHit::GetHistogram() const
  { return histogram_; }

  inline const std::map<G4double, G4int>& PetSensorHit::GetPhotonMap() const
  { return phot_; }

} // namespace nexus

#endif
