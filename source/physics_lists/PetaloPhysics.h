// ----------------------------------------------------------------------------
// petalosim | PetaloPhysics.h
//
// This class registers any new physics process defined in petalosim.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PETALO_PHYSICS_H
#define PETALO_PHYSICS_H

#include <G4VPhysicsConstructor.hh>
#include <VDetector.hh>

class G4GenericMessenger;
class WavelengthShifting;
class PositronAnnihilation;

class PetaloPhysics : public G4VPhysicsConstructor
{
public:
  /// Constructor
  PetaloPhysics();
  /// Destructor
  ~PetaloPhysics();

  /// Construct all required particles (Geant4 mandatory method)
  virtual void ConstructParticle();
  /// Construct all required physics processes (Geant4 mandatory method)
  virtual void ConstructProcess();

  VDetector *petalo_;

private:
  G4double max_beta_; ///< Max change for beta in Cerenkov

  G4bool risetime_; ///< Rise time for LYSO

  G4bool noCompt_; ///< Switch on/off Compton scattering

  G4bool nest_; ///< Switch on/off NEST

  G4bool prod_th_el_; ///< If true, NEST thermal electrons are produced

  G4String petalo_detector_;

  G4GenericMessenger *msg_;
  WavelengthShifting *wls_;
  PositronAnnihilation* pos_annihil_;
};

#endif
