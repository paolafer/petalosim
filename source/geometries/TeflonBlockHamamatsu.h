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
namespace nexus
{
class IonizationSD;
}

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
  void SetIoniSD(IonizationSD* ionisd);
  void SetMaxStepSize(G4double step_size);

  G4double GetTeflonThickness() const;

 private:

  G4Material* mat_;
  IonizationSD* ionisd_;
  G4double teflon_block_thick_;
  G4double max_step_size_;


};

inline void TeflonBlockHamamatsu::SetHoleMaterial(G4Material* mat) {mat_ = mat;}
inline void TeflonBlockHamamatsu::SetIoniSD(IonizationSD* ionisd) {ionisd_ = ionisd;}
inline void TeflonBlockHamamatsu::SetMaxStepSize(G4double step_size) {max_step_size_ = step_size;}
inline G4double TeflonBlockHamamatsu::GetTeflonThickness() const {return teflon_block_thick_;}

#endif
