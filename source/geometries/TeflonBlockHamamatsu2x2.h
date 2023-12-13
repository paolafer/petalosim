// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu2x2.h
//
// Teflon block used with the Hamamatsu VUV tiles,
// with 4 SiPMs coupled to 1 hole and 30 mm of length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TEFLON_HAMA_2x2_H
#define TEFLON_HAMA_2x2_H

#include "nexus/GeometryBase.h"

class G4Material;
namespace nexus
{
class IonizationSD;
}

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
  void SetIoniSD(IonizationSD* ionisd);
  void SetMaxStepSize(G4double step_size);

  G4double GetTeflonThickness() const;

 private:

  G4Material* mat_;
  IonizationSD* ionisd_;
  G4double teflon_block_thick_;
  G4double max_step_size_;


};

inline void TeflonBlockHamamatsu2x2::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockHamamatsu2x2::SetIoniSD(IonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockHamamatsu2x2::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline G4double TeflonBlockHamamatsu2x2::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
