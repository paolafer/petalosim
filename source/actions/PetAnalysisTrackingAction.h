// ----------------------------------------------------------------------------
// petalosim | PetAnalysisTrackingAction.h
//
// This class produces csv files with histogram information of the wavelengths 
// and the time of production of the optical photons.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef P_ANALYSIS_TRACKING_ACTION_H
#define P_ANALYSIS_TRACKING_ACTION_H

#include "nexus/GeometryBase.h"
#include <G4UserTrackingAction.hh>

class G4Track;
class G4GenericMessenger;

using namespace nexus;

class PetAnalysisTrackingAction : public G4UserTrackingAction
{
public:
  /// Constructor
  PetAnalysisTrackingAction();
  /// Destructor
  virtual ~PetAnalysisTrackingAction();

  virtual void PreUserTrackingAction(const G4Track *);
  virtual void PostUserTrackingAction(const G4Track *);

};

#endif
