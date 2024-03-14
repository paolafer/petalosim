// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu4x4.h
//
// Teflon block used with the Hamamatsu VUV tiles,
// with 16 SiPMs coupled to 1 hole and 30 mm of length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "TeflonBlockHamamatsu4x4.h"
#include "PetIonizationSD.h"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4UserLimits.hh>

TeflonBlockHamamatsu4x4::TeflonBlockHamamatsu4x4(): teflon_block_thick_(35.75*mm),
                                                    max_step_size_(1.*mm)
{
}

TeflonBlockHamamatsu4x4::~TeflonBlockHamamatsu4x4()
{
}


void TeflonBlockHamamatsu4x4::Construct()
{
  G4double teflon_block_xy = 67 * mm;

  G4double teflon_offset_x = 3.64 * mm;
  G4double teflon_offset_y = 3.7  * mm;

  G4double teflon_central_offset_x = 3.23 * mm;
  G4double teflon_central_offset_y = 3.11 * mm;

  G4double dist_between_holes_xy = 1.75 * mm;

  G4double teflon_holes_xy    = 5.75 * 4 * mm + 3*dist_between_holes_xy;
  G4double teflon_holes_depth = 30 * mm;

  G4Box* teflon_block_solid =
    new G4Box("TEFLON_BLOCK", teflon_block_xy/2., teflon_block_xy/2., teflon_block_thick_/2.);


  G4Material* teflon = G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");
  teflon->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());

  G4LogicalVolume* teflon_block_logic =
    new G4LogicalVolume(teflon_block_solid, teflon, "TEFLON_BLOCK");
  this->SetLogicalVolume(teflon_block_logic);

  // Holes in the block which are filled with LXe and defined as LXe vols
  G4Box* teflon_hole_solid =
    new G4Box("ACTIVE", teflon_holes_xy/2., teflon_holes_xy/2.,
              teflon_holes_depth/2.);

  G4LogicalVolume* teflon_hole_logic =
    new G4LogicalVolume(teflon_hole_solid, mat_, "ACTIVE");

  // Set the ACTIVE volume as an ionization sensitive det
  teflon_hole_logic->SetSensitiveDetector(ionisd_);
  teflon_hole_logic->SetUserLimits(new G4UserLimits(max_step_size_));

  G4double holes_pos_z = -teflon_block_thick_/2. + teflon_holes_depth/2.;

  G4int copy_no = 0;

  for (G4int j = 0; j < 2; j++) { // Loop over the tiles in row
    G4double holes_pos_y =
      teflon_block_xy/2. - teflon_offset_y - teflon_holes_xy/2. -
      j*(teflon_central_offset_y + teflon_holes_xy);
    for (G4int i = 0; i < 2; i++) { // Loop over the tiles in column
      G4double holes_pos_x =
        -teflon_block_xy/2. + teflon_offset_x + teflon_holes_xy/2. +
        i*(teflon_central_offset_x + teflon_holes_xy);
      
      new G4PVPlacement(0, G4ThreeVector(holes_pos_x, holes_pos_y, holes_pos_z),
                        teflon_hole_logic, "ACTIVE", teflon_block_logic,
                        false, copy_no, false);
      copy_no += 1;
    }
  }
  
}
