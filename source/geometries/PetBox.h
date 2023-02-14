// ----------------------------------------------------------------------------
// petalosim | PetBox.h
//
// This class implements the geometry of a symmetric box of LXe.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef PET_BOX_H
#define PET_BOX_H

#include "nexus/GeometryBase.h"
#include "TileGeometryBase.h"

#include <vector>
#include <random>

class G4GenericMessenger;
class G4LogicalVolume;
class G4VPhysicalVolume;

class TileHamamatsuVUV;
class TileHamamatsuBlue;
class TileFBK;

namespace nexus
{
  class SpherePointSampler;
  class BoxPointSampler;

}

using namespace nexus;

class PetBox : public GeometryBase
{

public:
  // Constructor
  PetBox();
  //Destructor
  ~PetBox();

  /// Generate a vertex within a given region of the geometry
  G4ThreeVector GenerateVertex(const G4String &region) const;

  TileGeometryBase *tile_;
  TileGeometryBase *tile2_;

private:
  void Construct();
  void BuildBox();
  void BuildSensors();

  G4LogicalVolume *lab_logic_;
  G4LogicalVolume *active_logic_;

  G4bool visibility_;
  G4double reflectivity_;
  G4bool tile_vis_;
  G4double tile_refl_;
  G4double sipm_pde_;

  G4ThreeVector source_pos_;

  G4String tile_type_d_, tile_type_c_;
  G4bool single_tile_coinc_plane_;

  G4double box_size_, box_thickness_;

  G4double ih_x_size_, ih_y_size_, ih_z_size_;
  G4double ih_thick_wall_, ih_thick_roof_;
  G4double source_tube_thick_wall_, source_tube_int_radius_;
  G4double dist_source_roof_, source_tube_thick_roof_;

  G4double n_tile_rows_, n_tile_columns_;
  G4double tile_thickn_, tile2_thickn_, full_row_size_, full_col_size_;

  G4double dist_lat_panels_;

  G4double dist_ihat_entry_panel_, panel_thickness_;
  G4double entry_panel_x_size_, entry_panel_y_size_;
  G4double dist_entry_panel_ground_;

  G4double dist_entry_panel_h_panel_, dist_entry_panel_v_panel_;

  G4double lat_panel_len_;
  G4double h_l_panel_z_size_, h_l_panel_y_pos_;
  G4double v_l_panel_z_size_;

  G4double dist_dice_flange_, dist_dice_flange2_;
  G4double dist_ham_vuv_, dist_ham_blue_, dist_fbk_;
  G4double panel_sipm_xy_size_, dist_sipms_panel_sipms_;
  G4double wls_depth_;

  G4bool add_teflon_block_;

  G4double max_step_size_;

  G4double hole_depth_;

  G4double pressure_;

  /// Messenger for the definition of control commands
  G4GenericMessenger* msg_;

  // Vertex generators
  SpherePointSampler* source_gen_;
  BoxPointSampler* teflon_hole_gen_;

  std::vector<G4ThreeVector> hole_positions_;
  G4double vtx_dim_z_, vtx_z_displ_;
};

#endif
