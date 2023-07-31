// ----------------------------------------------------------------------------
// petalosim | TeflonBlockFBK.h
//
// Teflon block used with the FBK tiles.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TEFLON_FBK_H
#define TEFLON_FBK_H

#include "nexus/GeometryBase.h"

class G4Material;
namespace nexus
{
class IonizationSD;
}

using namespace nexus;

class TeflonBlockFBK: public GeometryBase
{
public:
  // Constructor
  TeflonBlockFBK();
  //Destructor
  ~TeflonBlockFBK();

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

inline void TeflonBlockFBK::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockFBK::SetIoniSD(IonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockFBK::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline G4double TeflonBlockFBK::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
