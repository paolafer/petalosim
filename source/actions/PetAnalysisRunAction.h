// ----------------------------------------------------------------------------
// petalosim | PetAnalysisRunAction.h
//
// This is the run action used for optical photon analyses.
// The G4 analysis manager is opened and closed and the histograms and n-tuples
// to be saved are declared.
// A message at the beginning and at the end of the simulation is also printed.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PET_ANALYSIS_RUN_ACTION_H
#define PET_ANALYSIS_RUN_ACTION_H

#include <G4UserRunAction.hh>

namespace nexus {

  class PetAnalysisRunAction: public G4UserRunAction
  {
  public:
    /// Constructor
    PetAnalysisRunAction();
    /// Destructor
    ~PetAnalysisRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

  };

}

#endif
