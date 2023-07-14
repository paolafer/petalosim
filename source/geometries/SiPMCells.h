// ----------------------------------------------------------------------------
// petalosim | SiPMCell.h
//
// 6x6 mm2 VUV Hamamatsu SiPM geometry with individual cells.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef SIPM_CELL_H
#define SIPM_CELL_H

#include "nexus/GeometryBase.h"
//#include <G4ThreeVector.hh>

//class G4GenericMessenger;

using namespace nexus;

class SiPMCells : public GeometryBase
{
public:
  /// Constructor
  SiPMCells();
  /// Destructor
  ~SiPMCells();

  /// Invoke this method to build the volumes of the geometry
  void Construct();
};



#endif
