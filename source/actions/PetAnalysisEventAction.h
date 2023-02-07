// ----------------------------------------------------------------------------
// petalosim | PetAnalysisEventAction.h
//
// This class is based on PetaloEventAction and modified to produce
// a csv file with the number of scintillation photons event by event.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef P_ANALYSIS_EVENT_ACTION_H
#define P_ANALYSIS_EVENT_ACTION_H

#include <G4UserEventAction.hh>
#include <globals.hh>

class G4Event;
class G4GenericMessenger;

class PetAnalysisEventAction : public G4UserEventAction
{
public:
  /// Constructor
  PetAnalysisEventAction();
  /// Destructor
  ~PetAnalysisEventAction();

  /// Hook at the beginning of the event loop
  void BeginOfEventAction(const G4Event *);
  /// Hook at the end of the event loop
  void EndOfEventAction(const G4Event *);

private:
  G4GenericMessenger* msg_;
  G4int nevt_, nupdate_;
  G4double min_energy_;
  G4double max_energy_;

};

#endif
