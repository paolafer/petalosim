// ----------------------------------------------------------------------------
// petalosim | MicroCellHmtsuVUV.cc
//
// Each one of the cells of a Hamamatsu VUV SiPM.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "MicroCellHmtsuVUV.h"
#include "ToFSD.h"

#include <G4Box.hh>
#include <G4NistManager.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4SDManager.hh>
#include <G4VisAttributes.hh>

using namespace nexus;
using namespace CLHEP;

MicroCellHmtsuVUV::MicroCellHmtsuVUV() : MicroCellGeometryBase()
{
}


MicroCellHmtsuVUV::~MicroCellHmtsuVUV()
{
}

void MicroCellHmtsuVUV::Construct()
{
  G4double cell_size   = 50 * micrometer;
  G4double cell_thickn = 300 * micrometer;

  SetDimensions(G4ThreeVector(cell_size, cell_size, cell_thickn));

  G4Box* active_cell_solid =
      new G4Box("PHOTODIODES", cell_size/2., cell_size/2., cell_thickn/2);

  G4Material* silicon =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");

  G4LogicalVolume* active_cell_logic =
    new G4LogicalVolume(active_cell_solid, silicon, "PHOTODIODES");

  this->SetLogicalVolume(active_cell_logic);

    // SiPM efficiency set using the official Hamamatsu specs.
  const G4int entries = 104;

  G4double energies[entries] = {1.37760 * eV, 1.38104 * eV, 1.42124 * eV, 1.47165 * eV,
                                1.53422 * eV, 1.57798 * eV, 1.61482 * eV, 1.66337 * eV,
                                1.70787 * eV, 1.75853 * eV, 1.80050 * eV, 1.83230 * eV,
                                1.85690 * eV, 1.88217 * eV, 1.89940 * eV, 1.91695 * eV,
                                1.93482 * eV, 1.95304 * eV, 1.97159 * eV, 1.99051 * eV,
                                2.00494 * eV, 2.02945 * eV, 2.05457 * eV, 2.11209 * eV,
                                2.15318 * eV, 2.19301 * eV, 2.22535 * eV, 2.25253 * eV,
                                2.28039 * eV, 2.31215 * eV, 2.35146 * eV, 2.39904 * eV,
                                2.46312 * eV, 2.52303 * eV, 2.58592 * eV, 2.63518 * eV,
                                2.72160 * eV, 2.78554 * eV, 2.85256 * eV, 2.91263 * eV,
                                2.96465 * eV, 3.00763 * eV, 3.05749 * eV, 3.12660 * eV,
                                3.18052 * eV, 3.24266 * eV, 3.28112 * eV, 3.32050 * eV,
                                3.36084 * eV, 3.40916 * eV, 3.46611 * eV, 3.51753 * eV,
                                3.56284 * eV, 3.62508 * eV, 3.67322 * eV, 3.76487 * eV,
                                3.86121 * eV, 3.95317 * eV, 4.04961 * eV, 4.13023 * eV,
                                4.13280 * eV, 4.32615 * eV, 4.40398 * eV, 4.47022 * eV,
                                4.51876 * eV, 4.56837 * eV, 4.63451 * eV, 4.71860 * eV,
                                4.97232 * eV, 5.06307 * eV, 5.15720 * eV, 5.20884 * eV,
                                5.24826 * eV, 5.39105 * eV, 5.45465 * eV, 5.64711 * eV,
                                5.68569 * eV, 5.79657 * eV, 5.95416 * eV, 6.07587 * eV,
                                6.18423 * eV, 6.26807 * eV, 6.34452 * eV, 6.42286 * eV,
                                6.65927 * eV, 6.74563 * eV, 6.85679 * eV, 6.96000 * eV,
                                7.09046 * eV, 7.17605 * eV, 7.25108 * eV, 7.30198 * eV,
                                7.39278 * eV, 7.45904 * eV, 7.48588 * eV, 7.52650 * eV,
                                7.55382 * eV, 7.67929 * eV, 7.83840 * eV, 7.88295 * eV,
                                8.03517 * eV, 8.12935 * eV, 8.25842 * eV, 8.34119 * eV};

  G4double reflectivity[entries] = {0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0., 0., 0., 0., 0.,
                                    0., 0., 0., 0., 0.};

  G4double efficiency[entries] = {0.     , 0.06129, 0.08068, 0.10494, 0.13758,
                                  0.16277, 0.18236, 0.20754, 0.22405, 0.24672,
                                  0.26208, 0.27843, 0.28963, 0.30082, 0.30735,
                                  0.31567, 0.32492, 0.33629, 0.34745, 0.35578,
                                  0.36228, 0.36984, 0.37113, 0.38757, 0.39475,
                                  0.40529, 0.41164, 0.42094, 0.43047, 0.43514,
                                  0.43794, 0.43861, 0.43875, 0.43743, 0.43504,
                                  0.43047, 0.41648, 0.40249, 0.3885 , 0.37684,
                                  0.36425, 0.35115, 0.3372 , 0.31618, 0.29761,
                                  0.28030, 0.26559, 0.25256, 0.24605, 0.24839,
                                  0.25605, 0.26258, 0.26652, 0.26817, 0.27097,
                                  0.27393, 0.27068, 0.26631, 0.25978, 0.24912,
                                  0.24910, 0.21058, 0.20050, 0.19805, 0.19677,
                                  0.19792, 0.20604, 0.22089, 0.26290, 0.27849,
                                  0.29454, 0.29912, 0.29750, 0.29233, 0.29011,
                                  0.28528, 0.28448, 0.28719, 0.28719, 0.28438,
                                  0.28420, 0.28386, 0.28732, 0.29307, 0.31518,
                                  0.31967, 0.32558, 0.32794, 0.32929, 0.33150,
                                  0.33448, 0.33375, 0.31970, 0.31370, 0.30989,
                                  0.29676, 0.28548, 0.21963, 0.13575, 0.11150,
                                  0.04841, 0.02487, 0.00916, 0.};

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
    sipmsd->SetBoxConf(hama);
    sipmsd->SetSiPMCells(true);

    G4SDManager::GetSDMpointer()->AddNewDetector(sipmsd);
    active_cell_logic->SetSensitiveDetector(sipmsd);
  }

  G4VisAttributes cell_col = G4Colour::Yellow();
  active_cell_logic->SetVisAttributes(cell_col);
  
}
