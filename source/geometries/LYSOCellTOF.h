// ----------------------------------------------------------------------------
// petalosim | LYSOCellTOF.h
//
// Basic cell filled with LYSO, coupled 1-to-1 with two SiPMs.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef LYSO_CELL_TOF
#define LYSO_CELL_TOF

#include "nexus/GeometryBase.h"

class G4GenericMessenger;

using namespace nexus;

class LYSOCellTOF: public GeometryBase
{
public:
  /// Constructor
  LYSOCellTOF();

  /// Destructor
  ~LYSOCellTOF();

  void Construct();

 private:
    /// Messenger for the definition of control commands
  G4GenericMessenger* msg_;

  G4double xy_size_, z_size_;
  G4double double_sipm_;

};

#endif
