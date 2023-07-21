// ----------------------------------------------------------------------------
// petalosim | SiPMLYSO.cc
//
// 6x6 mm2 SiPMs, same as VUV Hamamatsu SiPM except for PDE.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "SiPMLYSO.h"
#include "ToFSD.h"
#include "PetMaterialsList.h"

#include "nexus/Visibilities.h"
#include "nexus/IonizationSD.h"

#include <G4GenericMessenger.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4SDManager.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalSkinSurface.hh>
#include <G4PhysicalConstants.hh>


using namespace nexus;
using namespace CLHEP;

SiPMLYSO::SiPMLYSO() : GeometryBase(),
                                       visibility_(1),
                                       sensor_depth_(-1),
                                       mother_depth_(0),
                                       naming_order_(0),
                                       box_geom_(0)
{
}

SiPMLYSO::~SiPMLYSO()
{
}

void SiPMLYSO::Construct()
{

  // PACKAGE ///////////////////////////////////////////////////////
  // Hammamatsu 6x6mm VUV
  G4double sipm_x = 5.95 * mm;
  G4double sipm_y = 5.85 * mm;
  G4double sipm_z = 0.35 * mm;

  SetDimensions(G4ThreeVector(sipm_x, sipm_y, sipm_z));

  G4Box *sipm_solid = new G4Box("SiPMHmtsuVUV", sipm_x / 2., sipm_y / 2., sipm_z / 2);

  G4Material *plastic = petmaterials::FR4();
  G4LogicalVolume *sipm_logic =
      new G4LogicalVolume(sipm_solid, plastic, "SiPMHmtsuVUV");

  this->SetLogicalVolume(sipm_logic);

  // Active window
  G4double active_window_x = sipm_x;
  G4double active_window_y = sipm_y;
  G4double active_window_depth = 0.01 * mm;

  G4Box *active_window_solid =
      new G4Box("PHOTODIODES", active_window_x / 2., active_window_y / 2., active_window_depth / 2);

  G4Material *silicon =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");

  G4LogicalVolume *active_window_logic =
      new G4LogicalVolume(active_window_solid, silicon, "PHOTODIODES");

  new G4PVPlacement(0, G4ThreeVector(0., 0., sipm_z / 2. - active_window_depth / 2.),
                    active_window_logic, "PHOTODIODES", sipm_logic, false, 0, false);

  // OPTICAL SURFACES //////////////////////////////////////////////

  // SiPM efficiency set using the official Hamamatsu specs.
  const G4int entries = 23;

  G4double energies[entries] =
    {1.5 * eV, 1.54980241262 * eV, 1.59979603883 * eV,
     1.65312257346 * eV, 1.71012680013 * eV, 1.77120275727 * eV,
     1.83680285940 * eV, 1.90744912322 * eV, 1.98374708815 * eV,
     2.06640321682 * eV, 2.15624683494 * eV, 2.25425805471 * eV,
     2.36160367637 * eV, 2.47968386018 * eV, 2.61019353704 * eV,
     2.75520428909 * eV, 2.91727512963 * eV, 3.09960482523 * eV,
     3.30624514691 * eV, 3.54240551455 * eV, 3.81489824644 * eV,
     3.96749000000 * eV, 4.0 * eV };

  G4double reflectivity[entries] =
    {0.      ,0.      ,0.      ,0.      ,0.      ,0.      ,
     0.      ,0.      ,0.      ,0.      ,0.      ,
     0.      ,0.      ,0.      ,0.      ,0.      ,
     0.      ,0.      ,0.      ,0.      ,0.,
     0.      ,0.};

  G4double efficiency[entries] =
    {0., 0.036, 0.048,
     0.060, 0.070, 0.090,
     0.105, 0.120, 0.145,
     0.170, 0.200, 0.235,
     0.275, 0.320, 0.370,
     0.420, 0.425, 0.415,
     0.350, 0.315, 0.185,
     0.060, 0.};


  G4MaterialPropertiesTable *sipm_mt = new G4MaterialPropertiesTable();
  sipm_mt->AddProperty("EFFICIENCY", energies, efficiency, entries);
  sipm_mt->AddProperty("REFLECTIVITY", energies, reflectivity, entries);

  G4OpticalSurface *sipm_opsurf =
      new G4OpticalSurface("SIPM_OPSURF", unified, polished, dielectric_metal);
  sipm_opsurf->SetMaterialPropertiesTable(sipm_mt);

  new G4LogicalSkinSurface("SIPM_OPSURF", active_window_logic, sipm_opsurf);

  // SENSITIVE DETECTOR ////////////////////////////////////////////

  G4String sdname = "/SIPM/SiPMLYSO";
  G4SDManager *sdmgr = G4SDManager::GetSDMpointer();

  if (!sdmgr->FindSensitiveDetector(sdname, false))
  {
    ToFSD *sipmsd = new ToFSD(sdname);
    if (sensor_depth_ == -1)
      G4Exception("[SiPMLYSO]", "Construct()", FatalException,
                  "Sensor Depth must be set before constructing");

    sipmsd->SetDetectorVolumeDepth(sensor_depth_);
    sipmsd->SetMotherVolumeDepth(mother_depth_);
    sipmsd->SetDetectorNamingOrder(naming_order_);
    sipmsd->SetBoxGeom(box_geom_);

    G4SDManager::GetSDMpointer()->AddNewDetector(sipmsd);
    active_window_logic->SetSensitiveDetector(sipmsd);
  }

  // Visibilities
  if (visibility_)
  {
    G4VisAttributes sipm_col = nexus::Yellow();
    sipm_logic->SetVisAttributes(sipm_col);
    G4VisAttributes active_window_col = nexus::Blue();
    active_window_col.SetForceSolid(true);
    active_window_logic->SetVisAttributes(active_window_col);
  }
  else
  {
    sipm_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
    active_window_logic->SetVisAttributes(G4VisAttributes::GetInvisible());
  }
}
