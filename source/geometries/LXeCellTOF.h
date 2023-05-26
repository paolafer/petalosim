// ----------------------------------------------------------------------------
// petalosim | LXeCellTOF.h
//
// Basic cell filled with LXe, coupled 1-to-1 with two SiPMs.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef LXE_CELL_TOF
#define LXE_CELL_TOF

#include "nexus/GeometryBase.h"

class G4GenericMessenger;

using namespace nexus;

class LXeCellTOF: public GeometryBase
{
public:
  /// Constructor
  LXeCellTOF();

  /// Destructor
  ~LXeCellTOF();

  void Construct();

 private:
    /// Messenger for the definition of control commands
  G4GenericMessenger* msg_;

  G4double xy_size_, z_size_;
  G4double double_sipm_;

};

#endif
