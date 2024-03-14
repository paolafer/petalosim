// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu4x4.h
//
// Teflon block used with the Hamamatsu VUV tiles,
// with 16 SiPMs coupled to 1 hole and 30 mm of length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TEFLON_HAMA_4x4_H
#define TEFLON_HAMA_4x4_H

#include "nexus/GeometryBase.h"

class G4Material;
class PetIonizationSD;

using namespace nexus;

class TeflonBlockHamamatsu4x4: public GeometryBase
{
public:
  // Constructor
  TeflonBlockHamamatsu4x4();
  //Destructor
  ~TeflonBlockHamamatsu4x4();

  void Construct();

  void SetHoleMaterial(G4Material* mat);
  void SetIoniSD(PetIonizationSD* ionisd);
  void SetMaxStepSize(G4double step_size);

  G4double GetTeflonThickness() const;

 private:

  G4Material* mat_;
  PetIonizationSD* ionisd_;
  G4double teflon_block_thick_;
  G4double max_step_size_;


};

inline void TeflonBlockHamamatsu4x4::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockHamamatsu4x4::SetIoniSD(PetIonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockHamamatsu4x4::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline G4double TeflonBlockHamamatsu4x4::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
