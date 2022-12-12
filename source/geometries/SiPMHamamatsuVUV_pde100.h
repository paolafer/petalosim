// ----------------------------------------------------------------------------
// petalosim | SiPMHamamatsuVUV_pde100.cc
//
// 6x6 mm2 VUV Hamamatsu SiPM geometry with 100% PDE.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_HAMA_VUV_PDE100_H
#define SIPM_HAMA_VUV_PDE100_H

#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>

class G4GenericMessenger;

using namespace nexus;

class SiPMHamamatsuVUV_pde100 : public GeometryBase
{
public:
  /// Constructor
  SiPMHamamatsuVUV_pde100();
  /// Destructor
  ~SiPMHamamatsuVUV_pde100();

  /// Invoke this method to build the volumes of the geometry
  void Construct();

  void SetVisibility(G4bool vis);
  void SetTimeBinning(G4double time_binning);
  void SetSensorDepth(G4int sensor_depth);
  void SetMotherDepth(G4int mother_depth);
  void SetNamingOrder(G4int naming_order);
  void SetBoxGeom(G4int box_geom);

private:
  G4bool visibility_;

  G4double time_binning_;
  G4int sensor_depth_;
  G4int mother_depth_;
  G4int naming_order_;
  G4int box_geom_;
};

inline void SiPMHamamatsuVUV_pde100::SetVisibility(G4bool vis)
{
  visibility_ = vis;
}

inline void SiPMHamamatsuVUV_pde100::SetTimeBinning(G4double time_binning)
{
  time_binning_ = time_binning;
}

inline void SiPMHamamatsuVUV_pde100::SetSensorDepth(G4int sensor_depth)
{
  sensor_depth_ = sensor_depth;
}

inline void SiPMHamamatsuVUV_pde100::SetMotherDepth(G4int mother_depth)
{
  mother_depth_ = mother_depth;
}

inline void SiPMHamamatsuVUV_pde100::SetNamingOrder(G4int naming_order)
{
  naming_order_ = naming_order;
}

inline void SiPMHamamatsuVUV_pde100::SetBoxGeom(G4int box_geom)
{
  box_geom_ = box_geom;
}

#endif
