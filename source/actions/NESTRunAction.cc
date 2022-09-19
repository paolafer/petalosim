// ----------------------------------------------------------------------------
// nexus | NESTRunAction.cc
//
// This action allows one to save NEST information about photons and
// thermal electrons.
// A message at the beginning and at the end of the simulation is printed.
//
// The NEXT Collaboration
// ----------------------------------------------------------------------------

#include "NESTRunAction.h"
#include "FactoryBase.h"

#include <NESTProc.hh>

#include <G4Run.hh>
#include <G4ProcessTable.hh>

using namespace nexus;

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
      auto analysisTriggerFunction = std::bind(&nEXOAnalysis::StoreNESTLineages, this, std::placeholders::_1);
      nestProc->SetAnalysisTrigger(analysisTriggerFunction);
    }
  }
}


void NESTRunAction::EndOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " end." << G4endl;
}
