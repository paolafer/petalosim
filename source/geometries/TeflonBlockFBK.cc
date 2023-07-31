// ----------------------------------------------------------------------------
// petalosim | TeflonBlockFBK.h
//
// Teflon block used with the FBK VUV tiles.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "TeflonBlockFBK.h"

#include "nexus/IonizationSD.h"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4UserLimits.hh>

TeflonBlockFBK::TeflonBlockFBK(): teflon_block_thick_(33.30 * mm),
                                  max_step_size_(1.*mm)
{
}

TeflonBlockFBK::~TeflonBlockFBK()
{
}

void TeflonBlockFBK::Construct()
{
  G4double teflon_block_xy = 67 * mm;
  G4Box* teflon_block_solid =
    new G4Box("TEFLON_BLOCK", teflon_block_xy/2., teflon_block_xy/2., teflon_block_thick_/2.);
  
  G4Material *teflon = G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");
  teflon->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  
  G4LogicalVolume* teflon_block_logic =
    new G4LogicalVolume(teflon_block_solid, teflon, "TEFLON_BLOCK");
  this->SetLogicalVolume(teflon_block_logic);

  G4double teflon_holes_xy    = 5.75 * mm;
  G4double teflon_holes_depth = 5 * mm;
  G4Box* teflon_hole_solid =
    new G4Box("ACTIVE", teflon_holes_xy/2., teflon_holes_xy/2., teflon_holes_depth/2.);

  G4LogicalVolume* teflon_hole_logic =
    new G4LogicalVolume(teflon_hole_solid, mat_, "ACTIVE");
  
  // Set the ACTIVE volume as an ionization sensitive det
  teflon_hole_logic->SetSensitiveDetector(ionisd_);
  teflon_hole_logic->SetUserLimits(new G4UserLimits(max_step_size_));
  
  G4double holes_pos_z = -teflon_block_thick_/2. + teflon_holes_depth/2.;

  G4double offset_x = 0.95 * mm;
  G4double offset_y = 0.55 * mm;

  G4double tile_xy = 29.*mm;
  G4double sipm_pitch = 3.5*mm;

  G4int copy_no = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      G4double hole_pos_x = -tile_xy / 2. + offset_x + 3  * mm + 2 * j * sipm_pitch;
      G4double hole_pos_y =  tile_xy / 2. - offset_y - 3.4* mm - 2 * i * sipm_pitch;
      new G4PVPlacement(0, G4ThreeVector(hole_pos_x, hole_pos_y, holes_pos_z),
                        teflon_hole_logic, "ACTIVE", teflon_block_logic, false, copy_no, false);
      copy_no += 1;
    }
  }

}
