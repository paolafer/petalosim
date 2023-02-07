// ----------------------------------------------------------------------------
// petalosim | PetAnalysisSteppingAction.h
//
// This class allows the user to print the total number of photons detected by
// all kinds of photosensors at the end of the run.
// It produces csv files with histogram information of photon velocities
// and wavelengths.
// It also shows examples of information that can be accessed at the stepping
// level, useful for debugging.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef P_ANALYSIS_STEPPING_ACTION_H
#define P_ANALYSIS_STEPPING_ACTION_H

#include <G4UserSteppingAction.hh>
#include <G4GenericMessenger.hh>
#include <globals.hh>

#include <vector>

class G4Step;
class G4GenericMessenger;

class PetAnalysisSteppingAction : public G4UserSteppingAction
{
public:
  /// Constructor
  PetAnalysisSteppingAction();
  /// Destructor
  ~PetAnalysisSteppingAction();

  virtual void UserSteppingAction(const G4Step *);

private:
  G4int detected;
  G4int not_det;

  typedef std::map<G4String, int> detectorCounts;
  detectorCounts my_counts;
};

#endif
