// ----------------------------------------------------------------------------
// petalosim | TileHamamatsuVUV.h
//
// Hamamatsu VUV sensor tile geometry.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TILE_HAMAMATSU_VUV_H
#define TILE_HAMAMATSU_VUV_H

#include "TileGeometryBase.h"

#include <G4ThreeVector.hh>

using namespace nexus;

class TileHamamatsuVUV : public TileGeometryBase
{
public:
  /// Constructor
  TileHamamatsuVUV();
  /// Destructor
  ~TileHamamatsuVUV();

  /// Invoke this method to build the volumes of the geometry
  void Construct();

private:
  // Tile dimensions
  G4double tile_x_, tile_y_, tile_z_;

  // SiPM pitch
  G4double sipm_pitch_;

  // Rows and columns of SiPMs
  G4int n_rows_, n_columns_;

  // Thickness of the LXe and quartz in front of the sensors
  G4double lxe_thick_;
  G4double quartz_rindex_;
  G4double quartz_thick_;
};

#endif
