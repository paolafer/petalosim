// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu15x15.h
//
// Teflon block used with the Hamamatsu VUV tiles,
// with 1 hole covering 1 SiPM and a half and configurable length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "TeflonBlockHamamatsu15x15.h"
#include "PetIonizationSD.h"

#include "nexus/Visibilities.h"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4UserLimits.hh>
#include <G4VisAttributes.hh>

TeflonBlockHamamatsu15x15::TeflonBlockHamamatsu15x15(): teflon_block_thick_(35.75 * mm),
                                                    hole_length_(5*mm),
                                                    max_step_size_(1.*mm)
{
}

TeflonBlockHamamatsu15x15::~TeflonBlockHamamatsu15x15()
{
}


void TeflonBlockHamamatsu15x15::Construct()
{
  G4double teflon_block_xy = 67 * mm;
 
  G4double dist_between_holes_x = 1.75 * mm;
  G4double dist_between_holes_y = 1.35 * mm;

  G4double tile_off_x = 1.225 * mm;
  G4double tile_off_y = 1.175 * mm;
  G4double dist_between_sipms_x = 1.65 * mm;
  G4double dist_between_sipms_y = 1.55 * mm;

  G4double teflon_holes_x_dim = 5.95*1.5 * mm + dist_between_sipms_x - dist_between_holes_x/2.;
  G4double teflon_holes_y_dim = 5.85*1.5 * mm + dist_between_sipms_y - dist_between_holes_y/2.;
  // G4double teflon_holes_xy     = 5.75*1.5 * mm + dist_between_holes_xy;
  
  G4Box* teflon_block_solid =
    new G4Box("TEFLON_BLOCK", teflon_block_xy/2., teflon_block_xy/2., teflon_block_thick_/2.);
  
  G4Material *teflon = G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");
  teflon->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  
  G4LogicalVolume* teflon_block_logic =
    new G4LogicalVolume(teflon_block_solid, teflon, "TEFLON_BLOCK");
  this->SetLogicalVolume(teflon_block_logic);
  // Holes in the block which are filled with LXe and defined as LXe vols
  //G4double dist_four_holes_xy = 2 * teflon_holes_xy + dist_between_holes_xy;
  
  G4Box* teflon_hole_solid =
    new G4Box("ACTIVE", teflon_holes_x_dim/2., teflon_holes_y_dim/2., hole_length_/2.);

  G4LogicalVolume* teflon_hole_logic =
    new G4LogicalVolume(teflon_hole_solid, mat_, "ACTIVE");
  
  // Set the ACTIVE volume as an ionization sensitive det
  teflon_hole_logic->SetSensitiveDetector(ionisd_);
  teflon_hole_logic->SetUserLimits(new G4UserLimits(max_step_size_));
  G4VisAttributes block_col = nexus::LightBlue();
  block_col.SetForceSolid(true);
  teflon_hole_logic->SetVisAttributes(block_col);

  G4double holes_pos_z = -teflon_block_thick_/2. + hole_length_/2.;
  
  G4int copy_no = 0;
  
  // Tile 1
  for (G4int i=0; i<2; i++) {
    G4double holes_pos_x = -tile_off_x - teflon_holes_x_dim/2. - i*(teflon_holes_x_dim + dist_between_sipms_x);
    for (G4int j=0; j<2; j++) {
    G4double holes_pos_y = tile_off_y + teflon_holes_y_dim/2. + j*(teflon_holes_y_dim + dist_between_sipms_y);
    new G4PVPlacement(0, G4ThreeVector(holes_pos_x, holes_pos_y, holes_pos_z),
                      teflon_hole_logic, "ACTIVE", teflon_block_logic,
                      false, copy_no, true);
    }
  }

  // Tile 2
  for (G4int i=0; i<2; i++) {
    G4double holes_pos_x = tile_off_x + teflon_holes_x_dim/2. + i*(teflon_holes_x_dim + dist_between_sipms_x);
    for (G4int j=0; j<2; j++) {
    G4double holes_pos_y = tile_off_y + teflon_holes_y_dim/2. + j*(teflon_holes_y_dim + dist_between_sipms_y);
    new G4PVPlacement(0, G4ThreeVector(holes_pos_x, holes_pos_y, holes_pos_z),
                      teflon_hole_logic, "ACTIVE", teflon_block_logic,
                      false, copy_no, true);
    }
  }

   // Tile 3
  for (G4int i=0; i<2; i++) {
    G4double holes_pos_x = tile_off_x + teflon_holes_x_dim/2. + i*(teflon_holes_x_dim + dist_between_sipms_x);
    for (G4int j=0; j<2; j++) {
    G4double holes_pos_y = -tile_off_y - teflon_holes_y_dim/2. - j*(teflon_holes_y_dim + dist_between_sipms_y);
    new G4PVPlacement(0, G4ThreeVector(holes_pos_x, holes_pos_y, holes_pos_z),
                      teflon_hole_logic, "ACTIVE", teflon_block_logic,
                      false, copy_no, true);
    }
  }

    // Tile 4
  for (G4int i=0; i<2; i++) {
    G4double holes_pos_x = -tile_off_x - teflon_holes_x_dim/2. - i*(teflon_holes_x_dim + dist_between_sipms_x);
    for (G4int j=0; j<2; j++) {
    G4double holes_pos_y = -tile_off_y - teflon_holes_y_dim/2. - j*(teflon_holes_y_dim + dist_between_sipms_y);
    new G4PVPlacement(0, G4ThreeVector(holes_pos_x, holes_pos_y, holes_pos_z),
                      teflon_hole_logic, "ACTIVE", teflon_block_logic,
                      false, copy_no, true);
    }
  }
}
