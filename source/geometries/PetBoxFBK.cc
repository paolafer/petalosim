// ----------------------------------------------------------------------------
// petalosim | PetBoxFBK.cc
//
// This class implements the geometry of a box of LXe with two FBK
// SiPM planes.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetBoxFBK.h"
#include "TileHamamatsuVUV.h"
#include "TileFBK.h"
#include "PetMaterialsList.h"
#include "PetOpticalMaterialProperties.h"
#include "Na22Source.h"
#include "TeflonBlockHamamatsu.h"
#include "TeflonBlockFBK.h"

#include "nexus/Visibilities.h"
#include "nexus/IonizationSD.h"
#include "nexus/FactoryBase.h"
#include "nexus/MaterialsList.h"
#include "nexus/OpticalMaterialProperties.h"
#include "nexus/SpherePointSampler.h"

#include <G4GenericMessenger.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Material.hh>
#include <G4SDManager.hh>
#include <G4UserLimits.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4SubtractionSolid.hh>

using namespace nexus;

REGISTER_CLASS(PetBoxFBK, GeometryBase)

PetBoxFBK::PetBoxFBK() : GeometryBase(),
                   visibility_(0),
                   reflectivity_(0),
                   tile_vis_(1),
                   tile_refl_(0.),
                   sipm_pde_(0.3),
                   source_pos_{},
                   box_size_(194.4 * mm),
                   box_thickness_(2. * cm),
                   ih_x_size_(6. * cm),
                   ih_y_size_(12. * cm),
                   ih_z_size_(4. * cm),
                   ih_thick_wall_(3. * mm),
                   ih_thick_roof_(6. * mm),
                   source_tube_thick_wall_(1. * mm),
                   source_tube_int_radius_(1.4 * cm),
                   dist_source_roof_(10. * mm),
                   source_tube_thick_roof_(5. * mm),
                   n_tile_rows_(2),
                   n_tile_columns_(2),
                   dist_lat_panels_(69. * mm),
                   dist_ihat_entry_panel_(5.25 * mm), //z distance between the external surface of the hat and the internal surface of the entry panel
                   panel_thickness_(1.75 * mm),
                   entry_panel_x_size_(77.5 * mm),
                   entry_panel_y_size_(120 * mm),
                   dist_entry_panel_ground_(12 * mm),
                   dist_entry_panel_h_panel_(6.2 * mm), //z distance between the internal surface of the entry panel and the edge of the horizontal lateral panel
                   dist_entry_panel_v_panel_(1.5 * mm),  //z distance between the internal surface of the entry panel and the edge of the vertical lateral panel
                   lat_panel_len_(66.5 * mm),
                   h_l_panel_z_size_(42. * mm),
                   h_l_panel_y_pos_(40.95 * mm),
                   v_l_panel_z_size_(46.7 * mm),
                   dist_ham_vuv_(18.6 * mm),
                   dist_ham_blue_(19.35 * mm),
//dist_fbk_(21.05 * mm),
                   dist_fbk_(19.6 * mm),
                   panel_sipm_xy_size_(66. * mm),
                   dist_sipms_panel_sipms_(0.3 * mm),
                   wls_depth_(0.001 * mm),
                   add_teflon_block_(0),
                   max_step_size_(1. * mm),
                   pressure_(1 * bar),
                   sipm_cells_(false)

{
  // Messenger
  msg_ = new G4GenericMessenger(this, "/Geometry/PetBoxFBK/",
                                "Control commands of geometry PetBoxFBK.");
  msg_->DeclareProperty("visibility", visibility_, "Visibility");
  msg_->DeclareProperty("surf_reflectivity", reflectivity_, "Reflectivity of the panels");
  msg_->DeclareProperty("tile_vis", tile_vis_, "Visibility of tiles");
  msg_->DeclareProperty("tile_refl", tile_refl_, "Reflectivity of SiPM boards");
  msg_->DeclareProperty("sipm_pde", sipm_pde_, "SiPM photodetection efficiency");

  msg_->DeclarePropertyWithUnit("specific_vertex", "mm",  source_pos_,
                                "Set generation vertex.");

  msg_->DeclareProperty("add_teflon_block", add_teflon_block_,
    "Boolean to add a teflon block that reduces the xenon volume");
  msg_->DeclareProperty("sipm_cells", sipm_cells_, "True if each cell of SiPMs is simulated");

  G4GenericMessenger::Command &press_cmd =
    msg_->DeclareProperty("pressure", pressure_,
                          "Pressure of LXe");
  press_cmd.SetUnitCategory("Pressure");
  press_cmd.SetParameterName("pressure", false);
  press_cmd.SetRange("pressure>0.");


}

PetBoxFBK::~PetBoxFBK()
{
}

void PetBoxFBK::Construct()
{
  // LAB. Volume of air surrounding the detector ///////////////
  G4double lab_size = 1. * m;
  G4Box *lab_solid = new G4Box("LAB", lab_size/2., lab_size/2., lab_size/2.);

  G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  lab_logic_ = new G4LogicalVolume(lab_solid, air, "LAB");
  lab_logic_->SetVisAttributes(G4VisAttributes::GetInvisible());
  this->SetLogicalVolume(lab_logic_);

  BuildBox();
  BuildSensors();
}

void PetBoxFBK::BuildBox()
{

  // BOX ///////////////////////////////////////////////////////
  G4Box *box_solid =
      new G4Box("BOX", box_size_/2., box_size_/2., box_size_/2.);

  G4Material *aluminum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  aluminum->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  G4LogicalVolume *box_logic =
      new G4LogicalVolume(box_solid, aluminum, "BOX");

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), box_logic,
                    "BOX", lab_logic_, false, 0, false);

  // LXe as ACTIVE /////////////////////////////////////////////

  G4double LXe_size = box_size_ - 2. * box_thickness_;
  G4Box *active_solid =
      new G4Box("LXe", LXe_size/2., LXe_size/2., LXe_size/2.);

  G4Material *LXe = G4NistManager::Instance()->FindOrBuildMaterial("G4_lXe");
  LXe->SetMaterialPropertiesTable(petopticalprops::LXe(pressure_));
  //LXe->SetMaterialPropertiesTable(opticalprops::LXe());
  active_logic_ =
      new G4LogicalVolume(active_solid, LXe, "ACTIVE");

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
                    active_logic_, "ACTIVE", box_logic, false, 0, false);

  // Set the ACTIVE volume as an ionization sensitive det
  IonizationSD *ionisd = new IonizationSD("/PETALO/ACTIVE");
  G4SDManager::GetSDMpointer()->AddNewDetector(ionisd);

  active_logic_->SetSensitiveDetector(ionisd);
  active_logic_->SetUserLimits(new G4UserLimits(max_step_size_));

  // Aluminum cylinder /////////////////////////////////////////
  G4double aluminum_cyl_rad = 40. * mm;
  G4double aluminum_cyl_len = 19. * mm;
  G4Tubs *aluminum_cyl_solid =
      new G4Tubs("ALUMINUM_CYL", 0, aluminum_cyl_rad, aluminum_cyl_len/2., 0, twopi);

  G4LogicalVolume *aluminum_cyl_logic =
      new G4LogicalVolume(aluminum_cyl_solid, aluminum, "ALUMINUM_CYL");

  G4RotationMatrix rot;
  rot.rotateX(pi / 2.);
  G4double aluminum_cyl_ypos = box_size_/2. - box_thickness_ - aluminum_cyl_len/2.;

  new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., aluminum_cyl_ypos, 0.)),
                    aluminum_cyl_logic, "ALUMINUM_CYL", active_logic_, false, 0, false);


  // INTERNAL HAT //////////////////////////////////////////////
  G4Box *internal_hat_solid =
      new G4Box("INTERNAL_HAT", ih_x_size_/2., ih_y_size_/2., ih_z_size_/2.);

  G4LogicalVolume *internal_hat_logic =
      new G4LogicalVolume(internal_hat_solid, aluminum, "INTERNAL_HAT");

  new G4PVPlacement(0, G4ThreeVector(0., (-box_size_/2. + box_thickness_ + ih_y_size_/2.), 0.),
                    internal_hat_logic, "INTERNAL_HAT", active_logic_, false, 0, false);

  G4double vacuum_hat_xsize = ih_x_size_ - 2. * ih_thick_wall_;
  G4double vacuum_hat_ysize = ih_y_size_ - ih_thick_roof_;
  G4double vacuum_hat_zsize = ih_z_size_ - 2. * ih_thick_wall_;
  G4Box *vacuum_hat_solid =
      new G4Box("VACUUM_HAT", vacuum_hat_xsize/2., vacuum_hat_ysize/2., vacuum_hat_zsize/2.);

  G4Material *vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  vacuum->SetMaterialPropertiesTable(opticalprops::Vacuum());
  G4LogicalVolume *vacuum_hat_logic =
      new G4LogicalVolume(vacuum_hat_solid, vacuum, "VACUUM_HAT");

  new G4PVPlacement(0, G4ThreeVector(0., -ih_thick_roof_/2., 0.), vacuum_hat_logic,
                    "VACUUM_HAT", internal_hat_logic, false, 0, false);

  // SOURCE TUBE ///////////////////////////////////////////////
  G4double source_tube_ext_radius = source_tube_int_radius_ + source_tube_thick_wall_;
  G4double source_tube_length = ih_y_size_ - ih_thick_roof_ - dist_source_roof_;
  G4Tubs *source_tube_solid =
      new G4Tubs("SOURCE_TUBE", 0, source_tube_ext_radius, source_tube_length/2., 0, twopi);

  G4Material *carbon_fiber = petmaterials::CarbonFiber();
  carbon_fiber->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  G4LogicalVolume *source_tube_logic =
      new G4LogicalVolume(source_tube_solid, carbon_fiber, "SOURCE_TUBE");

  G4double source_tube_ypos = source_tube_length/2. - (ih_y_size_ - ih_thick_roof_)/2.;
  new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., source_tube_ypos, 0.)),
                    source_tube_logic, "SOURCE_TUBE", vacuum_hat_logic, false, 0, false);

  G4double air_source_tube_len = source_tube_length - source_tube_thick_roof_;
  G4Tubs *air_source_tube_solid =
      new G4Tubs("AIR_SOURCE_TUBE", 0, source_tube_int_radius_, air_source_tube_len/2., 0, twopi);

  G4Material *air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
  air->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  G4LogicalVolume *air_source_tube_logic =
      new G4LogicalVolume(air_source_tube_solid, air, "AIR_SOURCE_TUBE");

  new G4PVPlacement(0, G4ThreeVector(0., 0., source_tube_thick_roof_/2.), air_source_tube_logic,
                    "AIR_SOURCE_TUBE", source_tube_logic, false, 0, false);

  // ENCAPSULATED SOURCE ///////////////////////////////////////

  Na22Source na22 = Na22Source();
  na22.Construct();
  G4LogicalVolume* na22_logic = na22.GetLogicalVolume();
  G4double source_offset_y = -0.9 * mm;
  G4double na22_pos = - box_size_/2 + box_thickness_ + air_source_tube_len / 2. - source_offset_y;

  new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., 0., na22_pos)), na22_logic,
                    "NA22_SOURCE_SUPPORT", air_source_tube_logic, false, 0, false);

  source_gen_ = new SpherePointSampler(0, na22.GetSourceDiameter()/2,
                                       G4ThreeVector(0, source_offset_y, 0.));

  // SOURCE TUBE INSIDE BOX ////////////////////////////////////
  G4Tubs *source_tube_inside_box_solid =
      new G4Tubs("SOURCE_TUBE", 0, source_tube_ext_radius, box_thickness_/2., 0, twopi);

  G4LogicalVolume *source_tube_inside_box_logic =
      new G4LogicalVolume(source_tube_inside_box_solid, carbon_fiber, "SOURCE_TUBE");

  new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., (-box_size_ + box_thickness_)/2., 0.)),
                    source_tube_inside_box_logic, "SOURCE_TUBE", box_logic, false, 0, false);

  G4Tubs *air_source_tube_inside_box_solid =
      new G4Tubs("AIR_SOURCE_TUBE", 0, source_tube_int_radius_, box_thickness_/2., 0, twopi);

  G4LogicalVolume *air_source_tube_inside_box_logic =
      new G4LogicalVolume(air_source_tube_inside_box_solid, air, "AIR_SOURCE_TUBE");

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), air_source_tube_inside_box_logic,
                    "AIR_SOURCE_TUBE", source_tube_inside_box_logic, false, 0, false);


  // TILES /////////////////////////////////////////////////////
  tile_ = new TileFBK();
  tile_->SetSiPMCells(sipm_cells_);
  tile_->SetPDE(sipm_pde_);
  tile_->SetBoxGeom(1);
  tile_->SetTileVisibility(tile_vis_);
  tile_->SetTileReflectivity(tile_refl_);

  tile_->Construct();
  tile_thickn_ = tile_->GetDimensions().z();
  dist_dice_flange_ = dist_fbk_;

  /// Teflon block ///

  TeflonBlockFBK* teflon_block_fbk = new TeflonBlockFBK();
  teflon_block_fbk->SetHoleMaterial(LXe);
  teflon_block_fbk->SetIoniSD(ionisd);
  teflon_block_fbk->SetMaxStepSize(max_step_size_);
  teflon_block_fbk->Construct();
  G4LogicalVolume* teflon_block_fbk_logic = teflon_block_fbk->GetLogicalVolume();

    
  G4double teflon_block_fbk_thick = teflon_block_fbk->GetTeflonThickness();
  G4double block_z_pos = ih_z_size_/2. + 0.25*mm + teflon_block_fbk_thick/2.;
  new G4PVPlacement(0, G4ThreeVector(0., 0., -block_z_pos), teflon_block_fbk_logic,
                    "TEFLON_BLOCK_FBK", active_logic_, false, 0, false);
  
  G4RotationMatrix rot_teflon;
  rot_teflon.rotateY(pi);
  new G4PVPlacement(G4Transform3D(rot_teflon, G4ThreeVector(0., 0., block_z_pos)),
                    teflon_block_fbk_logic,
                    "TEFLON_BLOCK_FBK", active_logic_, false, 1, false);

  end_of_teflon_z_ = block_z_pos + teflon_block_fbk_thick/2.;


  // Optical surface for teflon
  G4OpticalSurface* teflon_optSurf =
    new G4OpticalSurface("TEFLON_OPSURF", unified, ground, dielectric_metal);
  
  teflon_optSurf->SetMaterialPropertiesTable(petopticalprops::PTFE());
  
  new G4LogicalSkinSurface("TEFLON_FBK_OPSURF", teflon_block_fbk_logic, teflon_optSurf);

  // Visibilities
  if (visibility_) {
    G4VisAttributes box_col = nexus::White();
    box_logic->SetVisAttributes(box_col);
    G4VisAttributes al_cyl_col = nexus::DarkGrey();
    //al_cyl_col.SetForceSolid(true);
    aluminum_cyl_logic->SetVisAttributes(al_cyl_col);
    G4VisAttributes active_col = nexus::Blue();
    active_logic_->SetVisAttributes(active_col);
    G4VisAttributes ih_col = nexus::Yellow();
    internal_hat_logic->SetVisAttributes(ih_col);
    G4VisAttributes vacuum_col = nexus::Lilla();
    vacuum_hat_logic->SetVisAttributes(vacuum_col);
    G4VisAttributes source_tube_col = nexus::Red();
    //source_tube_col.SetForceSolid(true);
    source_tube_logic->SetVisAttributes(source_tube_col);
    G4VisAttributes air_source_tube_col = nexus::DarkGrey();
    // air_source_tube_col.SetForceSolid(true);
    air_source_tube_logic->SetVisAttributes(air_source_tube_col);
    G4VisAttributes air_source_tube_inside_box_col = nexus::White();
    //air_source_tube_inside_box_col.SetForceSolid(true);
    source_tube_inside_box_logic->SetVisAttributes(air_source_tube_inside_box_col);
    G4VisAttributes block_col = nexus::LightBlue();
    //block_col.SetForceSolid(true);
    teflon_block_fbk_logic->SetVisAttributes(block_col);
  } else {
    box_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
    active_logic_->SetVisAttributes(G4VisAttributes::GetInvisible());
  }
}

void PetBoxFBK::BuildSensors()
{
  /// "Detection" plane ///

  G4LogicalVolume* tile_logic = tile_->GetLogicalVolume();
  G4double tile_size_x = tile_->GetDimensions().x();
  G4double tile_size_y = tile_->GetDimensions().y();
  G4double full_row_size = n_tile_columns_ * tile_size_x;
  G4double full_col_size = n_tile_rows_ * tile_size_y;

  
  G4String vol_name;
  G4int copy_no = 1;
  //G4double z_pos = -box_size_/2. + box_thickness_ + dist_dice_flange_ + tile_thickn_/2.;
  G4double z_pos = end_of_teflon_z_ + 0.5*mm + tile_thickn_/2.;

  for (G4int j=0; j<n_tile_rows_; j++) {
    G4double y_pos = full_col_size/2. - tile_size_y/2. - j*tile_size_y;
    for (G4int i=0; i<n_tile_columns_; i++) {
      G4double x_pos = full_row_size/2. - tile_size_x/2. - i*tile_size_x;
      vol_name = "TILE_" + std::to_string(copy_no);
      new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, -z_pos),
                        tile_logic, vol_name, active_logic_, false, copy_no, false);
      copy_no += 1;
    }
  }
  
  /// "Coincidence" plane ///

  G4RotationMatrix rot;
  rot.rotateY(pi);
  
  for (G4int j=0; j<n_tile_rows_; j++) {
    G4double y_pos = full_col_size/2. - tile_size_y/2. - j*tile_size_y;
    for (G4int i=0; i<n_tile_columns_; i++) {
      G4double x_pos = full_row_size/2. - tile_size_x/2. - i*tile_size_x;
      vol_name = "TILE_" + std::to_string(copy_no);
      
      new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(x_pos, y_pos, z_pos)),
                        tile_logic, vol_name, active_logic_, false, copy_no, false);
      copy_no += 1;
    }
  }

}

G4ThreeVector PetBoxFBK::GenerateVertex(const G4String &region) const
{
  G4ThreeVector vertex(0., 0., 0.);

  if (region == "CENTER")
    {
      return vertex;
    }
  else if (region == "AD_HOC")
    {
      vertex = source_pos_;
    }
  else if (region == "SOURCE")
    {
      vertex = source_gen_->GenerateVertex("VOLUME");
    }
  else
    {
      G4Exception("[PetBoxFBK]", "GenerateVertex()", FatalException,
                  "Unknown vertex generation region!");
    }
  return vertex;
}
