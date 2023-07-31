// ----------------------------------------------------------------------------
// petalosim | Ba133Source.h
//
// Ba-133 calibration specific source with plastic support used in PETit.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef BA133_SOURCE_H
#define BA133_SOURCE_H

#include "DiskSource.h"

class Ba133Source: public DiskSource {
public:
  /// Constructor
  Ba133Source();
  
  /// Destructor
  ~Ba133Source();
  
  void Construct();

};

#endif
