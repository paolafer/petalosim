// ----------------------------------------------------------------------------
// petalosim | NESTRunAction.cc
//
// This action allows one to save NEST information about photons and
// thermal electrons.
// A message at the beginning and at the end of the simulation is printed.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "NESTRunAction.h"
//#include "PetaloUtils.h"
#include "PetaloPersistencyManager.h"
#include "nexus/FactoryBase.h"

#include <NESTProc.hh>

#include <G4Run.hh>
#include <G4ProcessTable.hh>


REGISTER_CLASS(NESTRunAction, G4UserRunAction)

NESTRunAction::NESTRunAction(): G4UserRunAction()
{
}



NESTRunAction::~NESTRunAction()
{
}



void NESTRunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

  G4ProcessTable* theProcessTable = G4ProcessTable::GetProcessTable();
  auto NESTProcesses = theProcessTable->FindProcesses();
  for (size_t i = 0; i < NESTProcesses->size(); i++) {
    NEST::NESTProc* nestProc = dynamic_cast<NEST::NESTProc*> ((*NESTProcesses)[i]);
    if (nestProc) {
      PetaloPersistencyManager* pm =
      dynamic_cast<PetaloPersistencyManager *>(G4VPersistencyManager::GetPersistencyManager());
      auto analysisTriggerFunction = std::bind(&PetaloPersistencyManager::StoreNESTLineages,
                                               pm, std::placeholders::_1);
      nestProc->SetAnalysisTrigger(analysisTriggerFunction);
    }
  }
}


void NESTRunAction::EndOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " end." << G4endl;
}
