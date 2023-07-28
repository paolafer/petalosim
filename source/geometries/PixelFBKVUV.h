// ----------------------------------------------------------------------------
// petalosim | PixelFBKVUV.h
//
// Each one of the cells of an FBK SiPM.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PIXEL_FBK_VUV_H
#define PIXEL_FBK_VUV_H

#include "nexus/GeometryBase.h"

using namespace nexus;

class PixelFBKVUV : public GeometryBase
{
 public:
  /// Constructor
  PixelFBKVUV();
  /// Destructor
  ~PixelFBKVUV();
  
  void Construct();

  void SetPDE(G4double eff);

 private:
    G4double eff_;
};

inline void PixelFBKVUV::SetPDE(G4double eff)
{
  eff_ = eff;
}

#endif
