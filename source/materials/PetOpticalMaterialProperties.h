// ----------------------------------------------------------------------------
// petalosim | PetOpticalMaterialProperties.h
//
// Optical properties of relevant materials.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef P_OPTICAL_MATERIAL_PROPERTIES_H
#define P_OPTICAL_MATERIAL_PROPERTIES_H

#include <globals.hh>

#include <CLHEP/Units/PhysicalConstants.h>

class G4MaterialPropertiesTable;

using namespace CLHEP;

/// This is a stateless class where all methods are static functions.

namespace petopticalprops
{
  G4MaterialPropertiesTable* Epoxy();
  G4MaterialPropertiesTable* GlassEpoxy();
  G4MaterialPropertiesTable* EpoxyFixedRefr(G4double n);
  G4MaterialPropertiesTable* EpoxyLXeRefr();

  G4MaterialPropertiesTable* FakeGenericMaterial(G4double quartz_rindex = 1.6);

  G4MaterialPropertiesTable* LXe(G4double pressure);
  G4MaterialPropertiesTable* LXe_nconst();

  G4MaterialPropertiesTable* Pyrex_vidrasa();
  G4MaterialPropertiesTable* PTFE();

  G4MaterialPropertiesTable* TPB(G4double decay_time = 2.2 * nanosecond);
  G4MaterialPropertiesTable* TPB_LXe(G4double decay_time = 2.2 * nanosecond);
  G4MaterialPropertiesTable* TPB_LXe_nconst(G4double decay_time = 2.2 * nanosecond);

  G4MaterialPropertiesTable* LYSO();
  G4MaterialPropertiesTable* LYSO_nconst();
  G4MaterialPropertiesTable* ReflectantSurface(G4double reflectivity = 0.95);

  //constexpr G4double optPhotMinE_ = 1. * eV;
  //constexpr G4double optPhotMaxE_ = 8.21 * eV;
  //constexpr G4double noAbsLength_ = 1.e8 * m;

  constexpr G4double lyso_minE_ = 1.9630 * eV;
  constexpr G4double lyso_maxE_ = 3.3429 * eV;
}

#endif
