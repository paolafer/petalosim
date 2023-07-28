// ----------------------------------------------------------------------------
// petalosim | SiPMFBKVUVCell.h
//
// 3x3 mm2 VUV FBK SiPM geometry with individual cells.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_FBK_VUV_CELL_H
#define SIPM_FBK_VUV_CELL_H

#include "nexus/GeometryBase.h"

using namespace nexus;

class SiPMFBKVUVCells : public GeometryBase
{
public:
  /// Constructor
  SiPMFBKVUVCells();
  /// Destructor
  ~SiPMFBKVUVCells();

  /// Invoke this method to build the volumes of the geometry
  void Construct();

  void SetPDE(G4double eff);

private:
    G4double eff_;
};

inline void SiPMFBKVUVCells::SetPDE(G4double eff)
{
  eff_ = eff;
}


#endif
