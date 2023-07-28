// ----------------------------------------------------------------------------
// petalosim | SiPMFBKVUVCell.cc
//
// 3x3 mm2 VUV FBK SiPM geometry with individual cells.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "SiPMFBKVUVCells.h"
#include "PetMaterialsList.h"
#include "PixelFBKVUV.h"

#include "nexus/Visibilities.h"
#include "nexus/IonizationSD.h"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>
#include <G4Material.hh>
#include <G4PhysicalConstants.hh>


using namespace nexus;
using namespace CLHEP;

SiPMFBKVUVCells::SiPMFBKVUVCells() : GeometryBase()
{
}

SiPMFBKVUVCells::~SiPMFBKVUVCells()
{
}

void SiPMFBKVUVCells::Construct()
{

  // PACKAGE ///////////////////////////////////////////////////////
  G4double sipm_x = 3.  * mm;
  G4double sipm_y = 3.4 * mm;
  G4double sipm_z = 0.6 * mm;

  SetDimensions(G4ThreeVector(sipm_x, sipm_y, sipm_z));

  G4Box* sipm_solid =
    new G4Box("SiPMFBKVUVCells", sipm_x/2., sipm_y/2., sipm_z/2);

  G4Material* plastic = petmaterials::FR4();
  G4LogicalVolume *sipm_logic =
      new G4LogicalVolume(sipm_solid, plastic, "SiPMFBKVUVCells");

  this->SetLogicalVolume(sipm_logic);
  
  PixelFBKVUV pxl;
  pxl.SetPDE(eff_);
  pxl.Construct();
  G4LogicalVolume* pxl_logic = pxl.GetLogicalVolume();
  G4double pxl_depth = pxl.GetDimensions().z();
  
  G4double pitch  = 0.035 * mm;
  G4int n_rows    = sipm_x / pitch;
  G4int n_columns = sipm_y / pitch;

  G4double x = 0;
  G4double y = 0;

  G4int copy_no = 0;

  for (G4int i=0; i<n_rows; i++) {
    for (G4int j=0; j<n_columns; j++) {
      x = - sipm_x/2 + pitch/2 + i*pitch;
      y = - sipm_y/2 + pitch/2 + j*pitch;
      new G4PVPlacement(0, G4ThreeVector(x, y, sipm_z/2. - pxl_depth/2.),
                        pxl_logic, "PHOTODIODES", sipm_logic,
                        false, copy_no, false);
      copy_no += 1;
    }
  }
}
