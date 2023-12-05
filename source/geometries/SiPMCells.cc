// ----------------------------------------------------------------------------
// petalosim | SiPMCell.cc
//
// Generic SiPM geometry with individual cells.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "SiPMCells.h"
#include "PetMaterialsList.h"
#include "MicroCellHmtsuVUV.h"
#include "MicroCellFBK.h"

#include "nexus/Visibilities.h"
#include "nexus/IonizationSD.h"

#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VisAttributes.hh>
#include <G4PVPlacement.hh>
#include <G4Material.hh>
#include <G4PhysicalConstants.hh>

#include <cassert>


using namespace nexus;
using namespace CLHEP;

SiPMCells::SiPMCells() : GeometryBase()
{
}

SiPMCells::~SiPMCells()
{
}

void SiPMCells::Construct()
{

  // PACKAGE //
  G4ThreeVector sipm_dim = GetDim();

  G4Box* sipm_solid = new G4Box("SiPM", sipm_dim.x()/2.,
                                sipm_dim.y()/2., sipm_dim.z()/2);

  G4Material* plastic = petmaterials::FR4();
  G4LogicalVolume* sipm_logic =
      new G4LogicalVolume(sipm_solid, plastic, "SiPM");

  this->SetLogicalVolume(sipm_logic);

  if (pxl_name_ == "MicroCellHmtsuVUV") {
    pxl_ = new MicroCellHmtsuVUV();
    pxl_->Construct();
  } else if (pxl_name_ == "MicroCellFBK") {
    pxl_ = new MicroCellFBK();
    pxl_->SetPDE(GetPDE());
    pxl_->Construct();
  } else {
    G4Exception("[SiPMCells]", "Construct()", FatalException,
                "Unknown type of microcell!");
  }
  G4LogicalVolume* pxl_logic = pxl_->GetLogicalVolume();
  G4double pxl_depth = pxl_->GetDimensions().z();
  
  G4int n_rows    =  sipm_dim.x() / pitch_ + 1.e-10;
  G4int n_columns =  sipm_dim.y() / pitch_ + 1.e-10;

  assert(n_rows*n_columns == n_microcells_);

  G4double x = 0;
  G4double y = 0;

  G4int copy_no = 0;

  for (G4int i=0; i<n_rows; i++) {
    for (G4int j=0; j<n_columns; j++) {
      x = - sipm_dim.x()/2 + pitch_/2 + i*pitch_;
      y = - sipm_dim.y()/2 + pitch_/2 + j*pitch_;
      new G4PVPlacement(0, G4ThreeVector(x, y, sipm_dim.z()/2. - pxl_depth/2.),
                        pxl_logic, "PHOTODIODES", sipm_logic,
                        false, copy_no, false);
      copy_no += 1;
    }
  }
}
