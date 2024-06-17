// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu.h
//
// Teflon block used with the Hamamatsu VUV tiles.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TEFLON_HAMA_H
#define TEFLON_HAMA_H

#include "nexus/GeometryBase.h"

class G4Material;

class PetIonizationSD;

using namespace nexus;

class TeflonBlockHamamatsu: public GeometryBase
{
public:
  // Constructor
  TeflonBlockHamamatsu();
  //Destructor
  ~TeflonBlockHamamatsu();

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

inline void TeflonBlockHamamatsu::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockHamamatsu::SetIoniSD(PetIonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockHamamatsu::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline void TeflonBlockHamamatsu::SetHoleLength(G4double hole_length) {hole_length_ = hole_length;}
inline G4double TeflonBlockHamamatsu::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
