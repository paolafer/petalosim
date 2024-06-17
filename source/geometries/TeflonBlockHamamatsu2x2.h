// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu2x2.h
//
// Teflon block used with the Hamamatsu VUV tiles,
// with 4 SiPMs coupled to 1 hole and configurable length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TEFLON_HAMA_2x2_H
#define TEFLON_HAMA_2x2_H

#include "nexus/GeometryBase.h"

class G4Material;
class PetIonizationSD;


using namespace nexus;

class TeflonBlockHamamatsu2x2: public GeometryBase
{
public:
  // Constructor
  TeflonBlockHamamatsu2x2();
  //Destructor
  ~TeflonBlockHamamatsu2x2();

  void Construct();

  void SetHoleMaterial(G4Material* mat);
  void SetIoniSD(PetIonizationSD* ionisd);
  void SetMaxStepSize(G4double step_size);
  void SetHoleLength(G4double hole_length);

  G4double GetTeflonThickness() const;

 private:

  G4Material* mat_;
  PetIonizationSD* ionisd_;
  G4double teflon_block_thick_, hole_length_;
  G4double max_step_size_;


};

inline void TeflonBlockHamamatsu2x2::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockHamamatsu2x2::SetIoniSD(PetIonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockHamamatsu2x2::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline void TeflonBlockHamamatsu2x2::SetHoleLength(G4double hole_length) {hole_length_ = hole_length;}
inline G4double TeflonBlockHamamatsu2x2::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
