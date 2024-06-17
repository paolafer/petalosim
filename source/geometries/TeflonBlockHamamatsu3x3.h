// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu3x3.h
//
// Teflon block used with the Hamamatsu VUV tiles,
// with 9 SiPMs coupled to 1 hole and configurable length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TEFLON_HAMA_3x3_H
#define TEFLON_HAMA_3x3_H

#include "nexus/GeometryBase.h"

class G4Material;
class PetIonizationSD;


using namespace nexus;

class TeflonBlockHamamatsu3x3: public GeometryBase
{
public:
  // Constructor
  TeflonBlockHamamatsu3x3();
  //Destructor
  ~TeflonBlockHamamatsu3x3();

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

inline void TeflonBlockHamamatsu3x3::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockHamamatsu3x3::SetIoniSD(PetIonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockHamamatsu3x3::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline void TeflonBlockHamamatsu3x3::SetHoleLength(G4double hole_length) {hole_length_ = hole_length;}
inline G4double TeflonBlockHamamatsu3x3::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
