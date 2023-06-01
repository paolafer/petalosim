// ----------------------------------------------------------------------------
// petalosim | SiPMLYSO.cc
//
// 6x6 mm2 SiPMs, same as VUV Hamamatsu SiPM except for PDE.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_LYSO_H
#define SIPM_LYSO_H

#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>

class G4GenericMessenger;

using namespace nexus;

class SiPMLYSO : public GeometryBase
{
public:
  /// Constructor
  SiPMLYSO();
  /// Destructor
  ~SiPMLYSO();

  /// Invoke this method to build the volumes of the geometry
  void Construct();

  void SetVisibility(G4bool vis);
  void SetSensorDepth(G4int sensor_depth);
  void SetMotherDepth(G4int mother_depth);
  void SetNamingOrder(G4int naming_order);
  void SetBoxGeom(G4int box_geom);

private:
  G4bool visibility_;

  G4int sensor_depth_;
  G4int mother_depth_;
  G4int naming_order_;
  G4int box_geom_;
};

inline void SiPMLYSO::SetVisibility(G4bool vis)
{
  visibility_ = vis;
}

inline void SiPMLYSO::SetSensorDepth(G4int sensor_depth)
{
  sensor_depth_ = sensor_depth;
}

inline void SiPMLYSO::SetMotherDepth(G4int mother_depth)
{
  mother_depth_ = mother_depth;
}

inline void SiPMLYSO::SetNamingOrder(G4int naming_order)
{
  naming_order_ = naming_order;
}

inline void SiPMLYSO::SetBoxGeom(G4int box_geom)
{
  box_geom_ = box_geom;
}

#endif
