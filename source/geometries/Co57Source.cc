// ----------------------------------------------------------------------------
// petalosim | Co57Source.cc
//
// Co-57 calibration specific source with polyester support used in PETit,
// borrowed from DIPC.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "Co57Source.h"

#include "nexus/Visibilities.h"
#include "nexus/MaterialsList.h"

#include <G4Tubs.hh>
#include <G4Orb.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>

namespace nexus {

  using namespace CLHEP;

  Co57Source::Co57Source():
    DiskSource()
  {
    source_diam_   = 3.*mm;
    support_diam_  = 25.*mm;
    support_thick_ = 3.*mm;
  }

  Co57Source::~Co57Source()
  {

  }

  void Co57Source::Construct()
  {

    // Plastic support
    G4Tubs* support_solid =
      new G4Tubs("SUPPORT", 0., support_diam_/2., support_thick_/2., 0., twopi);

    G4Material* plastic =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_POLYETHYLENE");
    G4LogicalVolume* support_logic =
      new G4LogicalVolume(support_solid, plastic, "CO57_SOURCE_SUPPORT");

    this->SetLogicalVolume(support_logic);

    G4Tubs* source_solid =
      new G4Tubs("CO57_SOURCE", 0, source_diam_/2., 2.*mm/2., 0., twopi);
    G4Material* co_mat =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Co");
    G4LogicalVolume* source_logic =
      new G4LogicalVolume(source_solid, co_mat, "CO57_SOURCE");

    new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
		      source_logic, "CO57_SOURCE",
		      support_logic, false, 0, false);
    G4VisAttributes source_col = nexus::LightGreen();
    source_col.SetForceSolid(true);
    source_logic->SetVisAttributes(source_col);

    return;

  }
}
