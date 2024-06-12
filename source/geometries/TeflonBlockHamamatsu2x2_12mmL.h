// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu2x2_12mmL.h
//
// Teflon block used with the Hamamatsu VUV tiles,
// with 4 SiPMs coupled to 1 hole and 12 mm of length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TEFLON_HAMA_2x2_12mmL_H
#define TEFLON_HAMA_2x2_12mmL_H

#include "nexus/GeometryBase.h"

class G4Material;
class PetIonizationSD;


using namespace nexus;

class TeflonBlockHamamatsu2x2_12mmL: public GeometryBase
{
public:
  // Constructor
  TeflonBlockHamamatsu2x2_12mmL();
  //Destructor
  ~TeflonBlockHamamatsu2x2_12mmL();

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

inline void TeflonBlockHamamatsu2x2_12mmL::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockHamamatsu2x2_12mmL::SetIoniSD(PetIonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockHamamatsu2x2_12mmL::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline G4double TeflonBlockHamamatsu2x2_12mmL::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
