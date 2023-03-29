// ----------------------------------------------------------------------------
// petalosim | Pixel.h
//
// Each one of the cells of a SiPM.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PIXEL_H
#define PIXEL_H

#include "nexus/GeometryBase.h"

using namespace nexus;

class Pixel : public GeometryBase
{
 public:
  /// Constructor
  Pixel();
  /// Destructor
  ~Pixel();
  
  void Construct();

};

#endif
