// ----------------------------------------------------------------------------
// petalosim | PixelFBKVUV.cc
//
// Each one of the cells of an FBK SiPM.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PixelFBKVUV.h"
#include "ToFSD.h"

#include <G4Box.hh>
#include <G4NistManager.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4SDManager.hh>
#include <G4VisAttributes.hh>

using namespace nexus;
using namespace CLHEP;

PixelFBKVUV::PixelFBKVUV() : GeometryBase(), eff_(0.3)
{
}


PixelFBKVUV::~PixelFBKVUV()
{
}

void PixelFBKVUV::Construct()
{
  G4double cell_size   = 35 * micrometer;
  G4double cell_thickn = 10 * micrometer;

  SetDimensions(G4ThreeVector(cell_size, cell_size, cell_thickn));

  G4Box* active_cell_solid =
      new G4Box("PHOTODIODES", cell_size/2., cell_size/2., cell_thickn/2);

  G4Material* silicon =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");

  G4LogicalVolume* active_cell_logic =
    new G4LogicalVolume(active_cell_solid, silicon, "PHOTODIODES");

  this->SetLogicalVolume(active_cell_logic);

  const G4int entries = 12;

  G4double energies[entries] = {1.5 * eV, 6.19919 * eV, 6.35814 * eV,
                                6.52546 * eV, 6.70182 * eV, 6.88799 * eV,
                                7.08479 * eV, 7.29316 * eV, 7.51417 * eV,
                                7.74898 * eV, 7.99895 * eV, 8.26558 * eV};
  G4double reflectivity[entries] = {0., 0., 0.,
                                    0., 0., 0.,
                                    0., 0., 0.,
                                    0., 0., 0.};
  G4double efficiency[entries] = {eff_, eff_, eff_,
                                  eff_, eff_, eff_,
                                  eff_, eff_, eff_,
                                  eff_, eff_, eff_};

  G4MaterialPropertiesTable* sipm_mt = new G4MaterialPropertiesTable();
  sipm_mt->AddProperty("EFFICIENCY", energies, efficiency, entries);
  sipm_mt->AddProperty("REFLECTIVITY", energies, reflectivity, entries);

  G4OpticalSurface* sipm_opsurf =
      new G4OpticalSurface("SIPM_OPSURF", unified, polished, dielectric_metal);
  sipm_opsurf->SetMaterialPropertiesTable(sipm_mt);

  new G4LogicalSkinSurface("SIPM_OPSURF", active_cell_logic, sipm_opsurf);

  // SENSITIVE DETECTOR ////////////////////////////////////////////

  G4String sdname = "/SIPM/SiPMHmtsuVUV";
  G4SDManager* sdmgr = G4SDManager::GetSDMpointer();

  if (!sdmgr->FindSensitiveDetector(sdname, false))
  {
    ToFSD* sipmsd = new ToFSD(sdname);

    sipmsd->SetDetectorVolumeDepth(0);
    sipmsd->SetMotherVolumeDepth(1);
    sipmsd->SetGrandMotherVolumeDepth(2);
    sipmsd->SetDetectorNamingOrder(0);
    sipmsd->SetBoxGeom(1);
    sipmsd->SetSiPMCells(1);

    G4SDManager::GetSDMpointer()->AddNewDetector(sipmsd);
    active_cell_logic->SetSensitiveDetector(sipmsd);
  }

  G4VisAttributes cell_col = G4Colour::Yellow();
  active_cell_logic->SetVisAttributes(cell_col);
  
}
