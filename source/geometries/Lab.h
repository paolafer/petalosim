// ----------------------------------------------------------------------------
// petalosim | Lab.h
//
// This class consists of two LXe cells placed opposite to each other.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef LAB_H
#define LAB_H

#include "nexus/GeometryBase.h"

class G4GenericMessenger;
class LXeCellTOF;
//class PetLYSOCell;

using namespace nexus;

class Lab: public GeometryBase
{
public:
  /// Constructor
  Lab();
  /// Destructor
  ~Lab();

  /// Return vertex within region <region> of the chamber
  virtual G4ThreeVector GenerateVertex(const G4String &region) const;

  virtual void Construct();

private:
  /// Messenger for the definition of control commands
  G4GenericMessenger* msg_;

  LXeCellTOF* module_;
};

#endif
