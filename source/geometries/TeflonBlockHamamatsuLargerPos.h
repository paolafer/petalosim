// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsuLargerPos.h
//
// Teflon block used with the Hamamatsu VUV tiles, with 1 hole covering
// a little bit more than 1 SiPM and a half and configurable length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TEFLON_HAMA_LARGER_POS_H
#define TEFLON_HAMA_LARGER_POS_H

#include "nexus/GeometryBase.h"

class G4Material;
class PetIonizationSD;


using namespace nexus;

class TeflonBlockHamamatsuLargerPos: public GeometryBase
{
public:
  // Constructor
  TeflonBlockHamamatsuLargerPos();
  //Destructor
  ~TeflonBlockHamamatsuLargerPos();

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

inline void TeflonBlockHamamatsuLargerPos::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockHamamatsuLargerPos::SetIoniSD(PetIonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockHamamatsuLargerPos::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline void TeflonBlockHamamatsuLargerPos::SetHoleLength(G4double hole_length) {hole_length_ = hole_length;}
inline G4double TeflonBlockHamamatsuLargerPos::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
