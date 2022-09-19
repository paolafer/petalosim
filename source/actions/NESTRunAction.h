// ----------------------------------------------------------------------------
// petalosim | NESTRunAction.h
//
// This action allows one to save NEST information about photons and
// thermal electrons.
// A message at the beginning and at the end of the simulation is printed.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef NEST_RUN_ACTION_H
#define NEST_RUN_ACTION_H

#include <G4UserRunAction.hh>


class NESTRunAction: public G4UserRunAction
{
public:
  /// Constructor
  NESTRunAction();
  /// Destructor
  ~NESTRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);
};

#endif
