// ----------------------------------------------------------------------------
// petalosim | Co57Source.h
//
// Co-57 calibration specific source with polyester support used in PETit,
// borrowed from DIPC.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef CO57_SOURCE_H
#define CO57_SOURCE_H

#include "DiskSource.h"

namespace nexus {

  class Co57Source: public DiskSource {
  public:
    /// Constructor
    Co57Source();

    /// Destructor
    ~Co57Source();

    void Construct();

  };
}
#endif
