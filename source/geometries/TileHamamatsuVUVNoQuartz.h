// ----------------------------------------------------------------------------
// petalosim | TileHamamatsuVUVNoQuartz.h
//
// Hamamatsu VUV sensor tile geometry.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TILE_HAMAMATSU_VUV_NO_Q_H
#define TILE_HAMAMATSU_VUV_NO_Q_H

#include "TileGeometryBase.h"

#include <G4ThreeVector.hh>

class G4GenericMessenger;

class SiPMHamamatsuVUV_pde100;

using namespace nexus;

class TileHamamatsuVUVNoQuartz : public TileGeometryBase
{
public:
  /// Constructor
  TileHamamatsuVUVNoQuartz();
  /// Destructor
  ~TileHamamatsuVUVNoQuartz();

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

  SiPMHamamatsuVUV_pde100 *sipm_;
};

#endif
