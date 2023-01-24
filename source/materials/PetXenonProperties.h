// ----------------------------------------------------------------------------
// petalosim | PetXenonProperties.h
//
// Some useful functions to calculate relevant physical properties of xenon.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PET_XENON_PROPERTIES_H
#define PET_XENON_PROPERTIES_H

#include <globals.hh>
#include <vector>

std::pair<G4int, G4int> MakeLXeDensityDataTable(std::vector<std::vector<G4double>>& data);

G4double GetLXeDensity(G4double temperature, G4double pressure);


#endif
