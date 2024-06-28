// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu15x15.h
//
// Teflon block used with the Hamamatsu VUV tiles,
// with 1 hole covering 1 SiPM and a half and configurable length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TEFLON_HAMA_15x15_H
#define TEFLON_HAMA_15x15_H

#include "nexus/GeometryBase.h"

class G4Material;
class PetIonizationSD;


using namespace nexus;

class TeflonBlockHamamatsu15x15: public GeometryBase
{
public:
  // Constructor
  TeflonBlockHamamatsu15x15();
  //Destructor
  ~TeflonBlockHamamatsu15x15();

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

inline void TeflonBlockHamamatsu15x15::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockHamamatsu15x15::SetIoniSD(PetIonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockHamamatsu15x15::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline void TeflonBlockHamamatsu15x15::SetHoleLength(G4double hole_length) {hole_length_ = hole_length;}
inline G4double TeflonBlockHamamatsu15x15::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
