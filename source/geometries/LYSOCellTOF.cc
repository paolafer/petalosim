// ----------------------------------------------------------------------------
// petalosim | LYSOCellTOF.cc
//
// Basic cell filled with LYSO, coupled 1-to-1 with two SiPMs.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "LYSOCellTOF.h"
#include "SiPMLYSO.h"
#include "PetOpticalMaterialProperties.h"
#include "PetMaterialsList.h"

#include "nexus/IonizationSD.h"
#include "nexus/FactoryBase.h"

#include <G4GenericMessenger.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>
#include <G4OpticalSurface.hh>
#include <G4UserLimits.hh>
#include <G4LogicalSkinSurface.hh>

REGISTER_CLASS(LYSOCellTOF, GeometryBase)

LYSOCellTOF::LYSOCellTOF(): GeometryBase(),
                            xy_size_(5.75*mm),
                            z_size_(5.*mm),
                            double_sipm_(false)
{
  msg_ = new G4GenericMessenger(this, "/Geometry/LYSOCellTOF/",
                                "Control commands of geometry LYSOCellTOF.");

  // xy size
  G4GenericMessenger::Command &xysize_cmd =
    msg_->DeclareProperty("xy_size", xy_size_, "xy dimension");
  xysize_cmd.SetUnitCategory("Length");
  xysize_cmd.SetParameterName("xy_size", false);
  xysize_cmd.SetRange("xy_size>0.");

  // z size
  G4GenericMessenger::Command &zsize_cmd =
    msg_->DeclareProperty("z_size", z_size_, "z dimension");
  zsize_cmd.SetUnitCategory("Length");
  zsize_cmd.SetParameterName("z_size", false);
  zsize_cmd.SetRange("z_size>0.");

  msg_->DeclareProperty("double_sipm", double_sipm_,
    "True if there are two SIPMs per cell");
}

LYSOCellTOF::~LYSOCellTOF()
{
  delete msg_;
}

void LYSOCellTOF::Construct()
{

  SiPMLYSO sipm;
  sipm.SetSensorDepth(2);
  sipm.Construct();
  G4ThreeVector sipm_dim = sipm.GetDimensions();

  G4double cell_xy  = xy_size_ + 2*mm;
  G4double cell_z   = z_size_ + 2*sipm_dim.z();
  G4Box* cell_solid = new G4Box("CELL", cell_xy/2., cell_xy/2., cell_z/2.);

  SetDimensions(G4ThreeVector(cell_xy, cell_xy, cell_z));

  G4Material* teflon = G4NistManager::Instance()->FindOrBuildMaterial("G4_TEFLON");
  teflon->SetMaterialPropertiesTable(new G4MaterialPropertiesTable());
  G4LogicalVolume* cell_logic =
    new G4LogicalVolume(cell_solid, teflon, "CELL");
  this->SetLogicalVolume(cell_logic);

  // Optical surface for teflon
  G4OpticalSurface* teflon_optSurf =
    new G4OpticalSurface("TEFLON_OPSURF", unified, ground, dielectric_metal);

  teflon_optSurf->SetMaterialPropertiesTable(petopticalprops::PTFE());

  new G4LogicalSkinSurface("TEFLON_OPSURF", cell_logic, teflon_optSurf);



  G4Box* active_solid =
    new G4Box("ACTIVE", xy_size_/2., xy_size_/2., z_size_/2.);

  G4Material* LYSO = petmaterials::LYSO();
  LYSO->SetMaterialPropertiesTable(petopticalprops::LYSO());

  G4LogicalVolume* active_logic =
    new G4LogicalVolume(active_solid, LYSO, "ACTIVE");

  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.),
                   active_logic, "ACTIVE", cell_logic, false, 0, true);
  active_logic ->SetUserLimits(new G4UserLimits(1.*mm));

  // Set the ACTIVE volume as an ionization sensitive det
  IonizationSD* ionisd = new IonizationSD("/PETALO/ACTIVE");
  G4SDManager::GetSDMpointer()->AddNewDetector(ionisd);
  active_logic->SetSensitiveDetector(ionisd);

  G4LogicalVolume* sipm_logic = sipm.GetLogicalVolume();
   new G4PVPlacement(0, G4ThreeVector(0., 0., -cell_z/2. + sipm_dim.z()/2.),
                    sipm_logic, "SIPM", cell_logic, false, 0, true);

   if (double_sipm_) {
     G4RotationMatrix rot;
     rot.rotateY(pi);
     new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., 0., cell_z/2.-sipm_dim.z()/2.)),
                       sipm_logic, "SIPM", cell_logic, false, 1, true);
   }
}
