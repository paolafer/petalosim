// ----------------------------------------------------------------------------
// petalosim | TeflonBlockHamamatsu.h
//
// Teflon block used with the Hamamatsu VUV tiles,
// with 4 SiPMs coupled to 1 hole and configurable length.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "TeflonBlockHamamatsu2x2.h"
#include "PetIonizationSD.h"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4Material.hh>
#include <G4UserLimits.hh>

TeflonBlockHamamatsu2x2::TeflonBlockHamamatsu2x2(): teflon_block_thick_(35.75 * mm),
                                                    hole_length_(5*mm),
                                                    max_step_size_(1.*mm)
{
}

TeflonBlockHamamatsu2x2::~TeflonBlockHamamatsu2x2()
{
}


void TeflonBlockHamamatsu2x2::Construct()
{
  G4double teflon_block_xy = 67 * mm;
  
  G4double teflon_offset_x = 3.64 * mm;
  G4double teflon_offset_y = 3.7  * mm;
  
  G4double teflon_central_offset_x = 3.23 * mm;
  G4double teflon_central_offset_y = 3.11 * mm;
  
  G4double dist_between_holes_xy = 1.75 * mm;

  G4double teflon_holes_xy    = 5.75*2 * mm + dist_between_holes_xy;
  G4double teflon_holes_depth = hole_length_;
  
  G4Box* teflon_block_solid =
    new G4Box("TEFLON_BLOCK", teflon_block_xy/2., teflon_block_xy/2., teflon_block_thick_/2.);
  
  
  G4Material *teflon = G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");
  teflon->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  
  G4LogicalVolume* teflon_block_logic =
    new G4LogicalVolume(teflon_block_solid, teflon, "TEFLON_BLOCK");
  this->SetLogicalVolume(teflon_block_logic);
  
  // Holes in the block which are filled with LXe and defined as LXe vols
  G4double dist_four_holes_xy = 2 * teflon_holes_xy + dist_between_holes_xy;
  
  G4Box* teflon_hole_solid =
    new G4Box("ACTIVE", teflon_holes_xy/2., teflon_holes_xy/2., teflon_holes_depth/2.);

  G4LogicalVolume* teflon_hole_logic =
    new G4LogicalVolume(teflon_hole_solid, mat_, "ACTIVE");
  
  // Set the ACTIVE volume as an ionization sensitive det
  teflon_hole_logic->SetSensitiveDetector(ionisd_);
  teflon_hole_logic->SetUserLimits(new G4UserLimits(max_step_size_));
  
  G4double holes_pos_z = -teflon_block_thick_/2. + teflon_holes_depth/2.;
  
  G4int copy_no = 0;
  
  for (G4int j = 0; j < 2; j++){ // Loop over the tiles in row
    G4double set_holes_y = teflon_block_xy/2. - teflon_offset_y - dist_four_holes_xy/2.
      - j*(teflon_central_offset_y + dist_four_holes_xy);
    for (G4int i = 0; i < 2; i++){ // Loop over the tiles in column
      G4double set_holes_x = -teflon_block_xy/2. + teflon_offset_x + dist_four_holes_xy/2.
        + i*(teflon_central_offset_x + dist_four_holes_xy);
      for (G4int l = 0; l < 2; l++){ // Loop over every 2 sensors in row
        G4double holes_pos_y = set_holes_y + (teflon_holes_xy/2. + dist_between_holes_xy/2.)
          - l*(teflon_holes_xy + dist_between_holes_xy);
        for (G4int k = 0; k < 2; k++){ // Loop over every 2 sensors in column
          G4double holes_pos_x = set_holes_x - (teflon_holes_xy/2. + dist_between_holes_xy/2.)
            + k*(teflon_holes_xy + dist_between_holes_xy);
          
          new G4PVPlacement(0, G4ThreeVector(holes_pos_x, holes_pos_y, holes_pos_z), teflon_hole_logic,
                            "ACTIVE", teflon_block_logic, false, copy_no, false);
          copy_no += 1;
        }
      }
    }
  }

}
