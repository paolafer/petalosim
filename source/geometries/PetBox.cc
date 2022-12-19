// ----------------------------------------------------------------------------
// petalosim | PetBox.cc
//
// This class implements the geometry of a symmetric box of LXe.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetBox.h"
#include "TileHamamatsuVUVNoQuartz.h"
#include "TileHamamatsuBlue.h"
#include "TileFBK.h"
#include "PetMaterialsList.h"
#include "PetOpticalMaterialProperties.h"
#include "Na22Source.h"

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

REGISTER_CLASS(PetBox, GeometryBase)

PetBox::PetBox() : GeometryBase(),
                   visibility_(0),
                   reflectivity_(0),
                   tile_vis_(1),
                   tile_refl_(0.),
                   sipm_pde_(0.2),
                   source_pos_{},
                   tile_type_d_("HamamatsuVUV"),
                   tile_type_c_("HamamatsuVUV"),
                   single_tile_coinc_plane_(0),
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
                   dist_fbk_(21.05 * mm),
                   panel_sipm_xy_size_(66. * mm),
                   dist_sipms_panel_sipms_(0.3 * mm),
                   wls_depth_(0.001 * mm),
                   add_teflon_block_(0),
                   max_step_size_(1. * mm)

{
  // Messenger
  msg_ = new G4GenericMessenger(this, "/Geometry/PetBox/",
                                "Control commands of geometry PetBox.");
  msg_->DeclareProperty("visibility", visibility_, "Visibility");
  msg_->DeclareProperty("surf_reflectivity", reflectivity_, "Reflectivity of the panels");
  msg_->DeclareProperty("tile_vis", tile_vis_, "Visibility of tiles");
  msg_->DeclareProperty("tile_refl", tile_refl_, "Reflectivity of SiPM boards");
  msg_->DeclareProperty("sipm_pde", sipm_pde_, "SiPM photodetection efficiency");

  msg_->DeclarePropertyWithUnit("specific_vertex", "mm",  source_pos_,
                                "Set generation vertex.");

  msg_->DeclareProperty("tile_type_d", tile_type_d_,
                        "Type of the tile in the detection plane");
  msg_->DeclareProperty("tile_type_c", tile_type_c_,
                        "Type of the tile in the coincidence plane");

  msg_->DeclareProperty("single_tile_coinc_plane", single_tile_coinc_plane_,
                        "If 1, one tile centered in coinc plane");

  G4GenericMessenger::Command &time_cmd =
      msg_->DeclareProperty("sipm_time_binning", time_binning_,
                            "Time binning for the sensor");
  time_cmd.SetUnitCategory("Time");
  time_cmd.SetParameterName("sipm_time_binning", false);
  time_cmd.SetRange("sipm_time_binning>0.");

  msg_->DeclareProperty("add_teflon_block", add_teflon_block_,
    "Boolean to add a teflon block that reduces the xenon volume");
}

PetBox::~PetBox()
{
}

void PetBox::Construct()
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

void PetBox::BuildBox()
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
      new G4Box("ACTIVE_OUT", LXe_size/2., LXe_size/2., LXe_size/2.);

  G4Material *LXe = G4NistManager::Instance()->FindOrBuildMaterial("G4_lXe");
  // LXe->SetMaterialPropertiesTable(opticalprops::LXe());
  active_logic_ =
      new G4LogicalVolume(active_solid, LXe, "ACTIVE_OUT");

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
                    active_logic_, "ACTIVE_OUT", box_logic, false, 0, false);

  // Set the ACTIVE volume as an ionization sensitive det
   IonizationSD *ionisd = new IonizationSD("/PETALO/ACTIVE");
   G4SDManager::GetSDMpointer()->AddNewDetector(ionisd);

  // active_logic_->SetSensitiveDetector(ionisd);
  // active_logic_->SetUserLimits(new G4UserLimits(max_step_size_));

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

  if (tile_type_d_ == "HamamatsuVUV") {
    tile_ = new TileHamamatsuVUVNoQuartz();
    dist_dice_flange_ = dist_ham_vuv_;
  } else if (tile_type_d_ == "HamamatsuBlue") {
    tile_ = new TileHamamatsuBlue();
    dist_dice_flange_ = dist_ham_blue_;
  } else if (tile_type_d_ == "FBK") {
    tile_ = new TileFBK();
    tile_->SetPDE(sipm_pde_);
    dist_dice_flange_ = dist_fbk_;
  } else {
    G4Exception("[PetBox]", "BuildBox()", FatalException,
                "Unknown tile type for the detection plane!");
  }

  tile_->SetBoxGeom(1);
  // Construct the tile for the distances in the active
  tile_->SetTileVisibility(tile_vis_);
  tile_->SetTileReflectivity(tile_refl_);
  tile_->SetTimeBinning(time_binning_);

  tile_->Construct();
  tile_thickn_ = tile_->GetDimensions().z();


  if (tile_type_d_ != tile_type_c_) {
    // TILE TYPE COINCIDENCE PLANE
    if (tile_type_c_ == "HamamatsuVUV") {
      tile2_ = new TileHamamatsuVUVNoQuartz();
      dist_dice_flange2_ = dist_ham_vuv_;
    } else if (tile_type_c_ == "HamamatsuBlue") {
      tile2_ = new TileHamamatsuBlue();
      dist_dice_flange2_ = dist_ham_blue_;
    } else if (tile_type_c_ == "FBK") {
      tile2_ = new TileFBK();
      tile2_->SetPDE(sipm_pde_);
      dist_dice_flange2_ = dist_fbk_;
    } else {
      G4Exception("[PetBox]", "BuildBox()", FatalException,
                  "Unknown tile type for the coincidence plane!");
    }

    tile2_->SetBoxGeom(1);
    tile2_->SetTileVisibility(tile_vis_);
    tile2_->SetTileReflectivity(tile_refl_);
    tile2_->SetTimeBinning(time_binning_);

    tile2_->Construct();
    tile2_thickn_ = tile2_->GetDimensions().z();
  }

  if (add_teflon_block_==0){

    // PYREX PANELS BETWEEN THE INTERNAL HAT AND THE ACTIVE REGIONS /////

    G4Box *entry_panel_solid =
        new G4Box("ENTRY_PANEL", entry_panel_x_size_/2., entry_panel_y_size_/2., panel_thickness_/2.);

    G4Material *pyrex = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pyrex_Glass");
    pyrex->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
    //pyrex->SetMaterialPropertiesTable(petopticalprops::Pyrex_vidrasa());

    G4LogicalVolume *entry_panel_logic =
        new G4LogicalVolume(entry_panel_solid, pyrex, "ENTRY_PANEL");

    G4double entry_panel_ypos = -box_size_/2. + box_thickness_ +
                                dist_entry_panel_ground_ + entry_panel_y_size_/2.;
    G4double entry_panel_zpos = ih_z_size_/2. + dist_ihat_entry_panel_ + panel_thickness_/2.;


    new G4PVPlacement(0, G4ThreeVector(0., entry_panel_ypos, -entry_panel_zpos),
                      entry_panel_logic, "ENTRY_PANEL", active_logic_, false, 1, false);

    new G4PVPlacement(0, G4ThreeVector(0., entry_panel_ypos, entry_panel_zpos),
                      entry_panel_logic, "ENTRY_PANEL", active_logic_, false, 2, false);

    // PYREX PANELS SURROUNDING THE SIPM DICE BOARDS /////////////

    // Horizontal lateral panels
    G4Box *h_l_panel_solid =
        new G4Box("LAT_PANEL", lat_panel_len_/2., panel_thickness_/2., h_l_panel_z_size_/2.);

    G4LogicalVolume *h_l_panel_logic =
        new G4LogicalVolume(h_l_panel_solid, pyrex, "LAT_PANEL");

    G4double h_l_panel_ypos_bot = -box_size_/2. + box_thickness_ +
                                  h_l_panel_y_pos_ + panel_thickness_/2.;
    G4double h_l_panel_ypos_top = h_l_panel_ypos_bot + panel_thickness_ +
                                  dist_lat_panels_;
    G4double h_l_panel_zpos = entry_panel_zpos + panel_thickness_/2. +
                              dist_entry_panel_h_panel_ + h_l_panel_z_size_/2.;

    new G4PVPlacement(0, G4ThreeVector(0., h_l_panel_ypos_bot, -h_l_panel_zpos),
                      h_l_panel_logic, "LAT_PANEL", active_logic_, false, 1, false);

    new G4PVPlacement(0, G4ThreeVector(0., h_l_panel_ypos_bot, h_l_panel_zpos),
                      h_l_panel_logic, "LAT_PANEL", active_logic_, false, 2, false);

    new G4PVPlacement(0, G4ThreeVector(0., h_l_panel_ypos_top, -h_l_panel_zpos),
                      h_l_panel_logic, "LAT_PANEL", active_logic_, false, 3, false);

    new G4PVPlacement(0, G4ThreeVector(0., h_l_panel_ypos_top, h_l_panel_zpos),
                      h_l_panel_logic, "LAT_PANEL", active_logic_, false, 4, false);

    // Vertical lateral panels
    G4Box *v_l_panel_solid =
        new G4Box("LAT_PANEL", panel_thickness_/2., lat_panel_len_/2., v_l_panel_z_size_/2.);

    G4LogicalVolume *v_l_panel_logic =
        new G4LogicalVolume(v_l_panel_solid, pyrex, "LAT_PANEL");

    G4double v_l_panel_xpos = dist_lat_panels_/2. + panel_thickness_/2.;
    G4double v_l_panel_ypos = h_l_panel_ypos_bot + dist_lat_panels_/2. +
                              panel_thickness_/2.;
    G4double v_l_panel_zpos = entry_panel_zpos + panel_thickness_/2. +
                              dist_entry_panel_v_panel_ + v_l_panel_z_size_/2.;

    new G4PVPlacement(0, G4ThreeVector(-v_l_panel_xpos, v_l_panel_ypos, -v_l_panel_zpos),
                      v_l_panel_logic, "LAT_PANEL", active_logic_, false, 1, false);

    new G4PVPlacement(0, G4ThreeVector(-v_l_panel_xpos, v_l_panel_ypos, v_l_panel_zpos),
                      v_l_panel_logic, "LAT_PANEL", active_logic_, false, 2, false);

    new G4PVPlacement(0, G4ThreeVector(v_l_panel_xpos, v_l_panel_ypos, -v_l_panel_zpos),
                      v_l_panel_logic, "LAT_PANEL", active_logic_, false, 3, false);

    new G4PVPlacement(0, G4ThreeVector(v_l_panel_xpos, v_l_panel_ypos, v_l_panel_zpos),
                      v_l_panel_logic, "LAT_PANEL", active_logic_, false, 4, false);

    // Optical surface for the panels
    G4OpticalSurface *panel_opsur = new G4OpticalSurface("OP_PANEL");
    panel_opsur->SetType(dielectric_metal);
    panel_opsur->SetModel(unified);
    panel_opsur->SetFinish(ground);
    panel_opsur->SetSigmaAlpha(0.1);
    panel_opsur->SetMaterialPropertiesTable(petopticalprops::ReflectantSurface(reflectivity_));
    new G4LogicalSkinSurface("OP_PANEL", entry_panel_logic, panel_opsur);
    new G4LogicalSkinSurface("OP_PANEL_H", h_l_panel_logic, panel_opsur);
    new G4LogicalSkinSurface("OP_PANEL_V", v_l_panel_logic, panel_opsur);

    if (visibility_){
      G4VisAttributes panel_col = nexus::Red();
      entry_panel_logic->SetVisAttributes(panel_col);
      h_l_panel_logic->SetVisAttributes(panel_col);
      v_l_panel_logic->SetVisAttributes(panel_col);
    }

    // Panel in front of the sensors just for the Hamamatsu Blue SiPMs

    G4Material *pyrex2 = G4NistManager::Instance()->FindOrBuildMaterial("G4_Pyrex_Glass");
    pyrex2->SetMaterialPropertiesTable(petopticalprops::Pyrex_vidrasa());

    G4Box *panel_sipms_solid =
      new G4Box("PANEL_SiPMs", panel_sipm_xy_size_/2., panel_sipm_xy_size_/2.,
                (panel_thickness_ + wls_depth_)/2.);

    G4LogicalVolume *panel_sipms_logic =
      new G4LogicalVolume(panel_sipms_solid, pyrex2, "PANEL_SiPMs");

    // WAVELENGTH SHIFTER LAYER ON THE PANEL /////////////////////////////////
    G4Box *wls_solid =
      new G4Box("WLS", panel_sipm_xy_size_/2., panel_sipm_xy_size_/2., wls_depth_/2);

    G4Material *wls = materials::TPB();
    wls->SetMaterialPropertiesTable(petopticalprops::TPB());

    G4LogicalVolume *wls_logic =
      new G4LogicalVolume(wls_solid, wls, "WLS");

    new G4PVPlacement(0, G4ThreeVector(0., 0., panel_thickness_/2.), wls_logic,
                        "WLS", panel_sipms_logic, false, 0, false);

    // Optical surface for WLS
    G4OpticalSurface *wls_optSurf = new G4OpticalSurface("WLS_OPSURF",
                                                         glisur, ground,
                                                         dielectric_dielectric, .01);
    new G4LogicalSkinSurface("WLS_OPSURF", wls_logic, wls_optSurf);

    // PLACEMENT OF THE PANEL
    if (tile_type_d_ == "HamamatsuBlue") {
      G4double panel_sipms_zpos = box_size_/2. - box_thickness_ - dist_dice_flange_ -
                tile_thickn_ - dist_sipms_panel_sipms_ - (panel_thickness_ + wls_depth_)/2.;
      new G4PVPlacement(0, G4ThreeVector(0., 0., -panel_sipms_zpos),
                        panel_sipms_logic, "PANEL_SiPMs", active_logic_, false, 1, false);

      if (visibility_){
        G4VisAttributes panel_col = nexus::Red();
        panel_sipms_logic->SetVisAttributes(panel_col);
        G4VisAttributes wls_col = nexus::LightBlue();
        wls_logic->SetVisAttributes(wls_col);
      }
    }

    if (tile_type_c_ == "HamamatsuBlue") {
      G4RotationMatrix rot_panel;
      rot_panel.rotateY(pi);
      G4double panel_sipms_zpos2;
      if (tile_type_d_ != tile_type_c_) {
        panel_sipms_zpos2 = box_size_/2. - box_thickness_ - dist_dice_flange2_
              - tile2_thickn_ - dist_sipms_panel_sipms_ - (panel_thickness_+wls_depth_)/2.;
      } else {
        panel_sipms_zpos2 = box_size_/2. - box_thickness_ - dist_dice_flange_ - tile_thickn_
              - dist_sipms_panel_sipms_ - (panel_thickness_+wls_depth_)/2.;
      }
      new G4PVPlacement(G4Transform3D(rot_panel, G4ThreeVector(0., 0., panel_sipms_zpos2)),
                          panel_sipms_logic, "PANEL_SiPMs", active_logic_, false, 2, false);

      if (visibility_) {
        G4VisAttributes panel_col = nexus::Red();
        panel_sipms_logic->SetVisAttributes(panel_col);
        G4VisAttributes wls_col = nexus::LightBlue();
        wls_logic->SetVisAttributes(wls_col);
      }
    }
  } else {

    if (tile_type_d_ != tile_type_c_) {
      G4Exception("[PetBox]", "BuildBox()", FatalException,
                  "Teflon Block only allowed when tiles in both planes are the same!");
    } else {
      if (tile_type_d_ == "FBK" or tile_type_d_ == "HamamatsuBlue"){
        G4Exception("[PetBox]", "BuildBox()", FatalException,
                    "Teflon Block only allowed when tiles in both planes are the HamamatsuVUV!");
      }
    }

    // TEFLON BLOCK TO REDUCE XENON VOL  /////////////////////////

    G4double teflon_block_xy    = 67    * mm;
    G4double teflon_block_thick = 35.75 * mm;

    G4double teflon_offset_x = 3.64 * mm;
    G4double teflon_offset_y = 3.7  * mm;

    G4double teflon_central_offset_x = 3.23 * mm;
    G4double teflon_central_offset_y = 3.11 * mm;

    G4double teflon_holes_xy    = 5.75 * mm;
    G4double teflon_holes_depth = 5    * mm;

    G4double dist_between_holes_xy = 1.75 * mm;

    G4Box *teflon_block_solid =
      new G4Box("TEFLON_BLOCK", teflon_block_xy/2., teflon_block_xy/2., teflon_block_thick/2.);

    G4double block_z_pos = ih_z_size_/2. + teflon_block_thick/2.; // + 0.55 * mm;

    G4Material *teflon = G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");
    teflon->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());

    G4LogicalVolume *teflon_block_logic =
       new G4LogicalVolume(teflon_block_solid, teflon, "TEFLON_BLOCK");

    // Holes in the block which are filled with LXe and defined as LXe vols
    G4double dist_four_holes = 4* teflon_holes_xy + 3*dist_between_holes_xy;

    G4Box *teflon_hole_solid =
      new G4Box("ACTIVE", teflon_holes_xy/2., teflon_holes_xy/2., teflon_holes_depth/2.);

    G4Material *LXe_true = G4NistManager::Instance()->FindOrBuildMaterial("G4_lXe");
    LXe_true->SetMaterialPropertiesTable(opticalprops::LXe());
    G4LogicalVolume *teflon_hole_logic =
       new G4LogicalVolume(teflon_hole_solid, LXe_true, "ACTIVE");

    // Set the ACTIVE volume as an ionization sensitive det
    teflon_hole_logic->SetSensitiveDetector(ionisd);
    teflon_hole_logic->SetUserLimits(new G4UserLimits(max_step_size_));

    G4double holes_pos_z = -teflon_block_thick/2. + teflon_holes_depth/2.;

    G4int copy_no = 0;

    for (G4int j = 0; j < 2; j++){ // Loop over the tiles in row
      G4double set_holes_y = teflon_block_xy/2. - teflon_offset_y - dist_four_holes/2.
                             - j*(teflon_central_offset_y + dist_four_holes);
      for (G4int i = 0; i < 2; i++){ // Loop over the tiles in column
        G4double set_holes_x = -teflon_block_xy/2. + teflon_offset_x + dist_four_holes/2.
                               + i*(teflon_central_offset_x + dist_four_holes);
        for (G4int l = 0; l < 4; l++){ // Loop over the sensors in row
          G4double holes_pos_y = set_holes_y + 3*(teflon_holes_xy/2. + dist_between_holes_xy/2.)
                                - l*(teflon_holes_xy + dist_between_holes_xy);
          for (G4int k = 0; k < 4; k++){ // Loop over the sensors in column
            G4double holes_pos_x = set_holes_x - 3*(teflon_holes_xy/2. + dist_between_holes_xy/2.)
                                    + k*(teflon_holes_xy + dist_between_holes_xy);

            new G4PVPlacement(0, G4ThreeVector(holes_pos_x, holes_pos_y, holes_pos_z), teflon_hole_logic,
                              "ACTIVE", teflon_block_logic, false, copy_no, false);
            copy_no += 1;
          }
        }
      }
    }

    new G4PVPlacement(0, G4ThreeVector(0., 0., -block_z_pos), teflon_block_logic,
                      "TEFLON_BLOCK", active_logic_, false, 0, false);

    G4RotationMatrix rot_teflon;
    rot_teflon.rotateY(pi);
      new G4PVPlacement(G4Transform3D(rot_teflon, G4ThreeVector(0., 0., block_z_pos)), teflon_block_logic,
                        "TEFLON_BLOCK", active_logic_, false, 1, false);


    // Optical surface for teflon
    G4OpticalSurface* teflon_optSurf =
      new G4OpticalSurface("TEFLON_OPSURF", unified, ground, dielectric_metal);

    teflon_optSurf->SetMaterialPropertiesTable(petopticalprops::PTFE());

      new G4LogicalSkinSurface("TEFLON_OPSURF", teflon_block_logic, teflon_optSurf);


    if (visibility_) {
      G4VisAttributes block_col = nexus::LightBlue();
      teflon_block_logic->SetVisAttributes(block_col);
    }
  }


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
  }
  else
  {
    box_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
    active_logic_->SetVisAttributes(G4VisAttributes::GetInvisible());
  }
}

void PetBox::BuildSensors()
{
  // SiPMs /////////////////////////////////////////////////////

  G4double tile_size_x = tile_->GetDimensions().x();
  G4double tile_size_y = tile_->GetDimensions().y();
  full_row_size_ = n_tile_columns_ * tile_size_x;
  full_col_size_ = n_tile_rows_ * tile_size_y;

  G4LogicalVolume *tile_logic = tile_->GetLogicalVolume();

  G4String vol_name;
  G4int copy_no = 0;

  G4double z_pos = -box_size_/2. + box_thickness_ + dist_dice_flange_ + tile_thickn_/2. - 1.2*mm;
  for (G4int j = 0; j < n_tile_rows_; j++)
  {
    G4double y_pos = full_col_size_/2. - tile_size_y/2. - j*tile_size_y;
    for (G4int i = 0; i < n_tile_columns_; i++)
    {
      G4double x_pos = -full_row_size_/2. + tile_size_x/2. + i*tile_size_x;
      vol_name = "TILE_" + std::to_string(copy_no);

      new G4PVPlacement(0, G4ThreeVector(x_pos, y_pos, z_pos), tile_logic,
                        vol_name, active_logic_, false, copy_no, false);
      copy_no += 1;
    }
  }

  G4RotationMatrix rot;
  rot.rotateY(pi);

  if (tile_type_d_ != tile_type_c_) {

    G4LogicalVolume* tile2_logic = tile2_->GetLogicalVolume();

    G4double z_pos2 = -box_size_/2. + box_thickness_ + dist_dice_flange2_ + tile2_thickn_/2.;

    if (single_tile_coinc_plane_) {

      /// SINGLE TILE CENTERED IN X AND Y (COINCIDENCE PLANE)
      vol_name = "TILE_" + std::to_string(copy_no);
      G4double x_pos = 0.;
      G4double y_pos = 0.;

      new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(x_pos, y_pos, -z_pos2)),
                        tile2_logic, vol_name, active_logic_, false, copy_no, false);
    } else {

      /// 4 TILES
      for (G4int j=0; j<n_tile_rows_; j++) {
        G4double y_pos = full_col_size_/2. - tile_size_y/2. - j*tile_size_y;
        for (G4int i=0; i<n_tile_columns_; i++) {
          G4double x_pos = full_row_size_/2. - tile_size_x/2. - i*tile_size_x;
          vol_name = "TILE_" + std::to_string(copy_no);

          new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(x_pos, y_pos, -z_pos2)),
                            tile2_logic, vol_name, active_logic_, false, copy_no, false);
          copy_no += 1;
        }
      }
    }
  } else {
    if (single_tile_coinc_plane_) {

      /// SINGLE TILE CENTERED IN X AND Y (COINCIDENCE PLANE)
      vol_name = "TILE_" + std::to_string(copy_no);
      G4double x_pos = 0.;
      G4double y_pos = 0.;

      new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(x_pos, y_pos, -z_pos)),
                        tile_logic, vol_name, active_logic_, false, copy_no, false);
    } else {

      /// 4 TILES
      for (G4int j=0; j<n_tile_rows_; j++) {
        G4double y_pos = full_col_size_/2. - tile_size_y/2. - j*tile_size_y;
        for (G4int i=0; i<n_tile_columns_; i++) {
          G4double x_pos = full_row_size_/2. - tile_size_x/2. - i*tile_size_x;
          vol_name = "TILE_" + std::to_string(copy_no);

          new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(x_pos, y_pos, -z_pos)),
                            tile_logic, vol_name, active_logic_, false, copy_no, false);
          copy_no += 1;
        }
      }
    }
  }


}

G4ThreeVector PetBox::GenerateVertex(const G4String &region) const
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
      G4Exception("[PetBox]", "GenerateVertex()", FatalException,
                  "Unknown vertex generation region!");
    }
  return vertex;
}
