// ----------------------------------------------------------------------------
// petalosim | Lab.cc
//
// This class consists of two LXe cells placed opposite to each other.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "Lab.h"
#include "Pet2boxes.h"
#include "LXeCellTOF.h"
#include "LYSOCellTOF.h"
#include "SiPMHamamatsuVUV.h"

#include "nexus/IonizationSD.h"
#include "nexus/FactoryBase.h"
#include "nexus/MaterialsList.h"

#include <G4GenericMessenger.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4SDManager.hh>
#include <G4NistManager.hh>
#include <G4VisAttributes.hh>

using namespace nexus;

using namespace CLHEP;

REGISTER_CLASS(Lab, GeometryBase)

Lab::Lab(): GeometryBase(), msg_(0)
{
  msg_ = new G4GenericMessenger(this, "/Geometry/Lab/",
                                "Control commands of geometry Lab.");

  //module_ = new LXeCellTOF();
  module_ = new LYSOCellTOF();
}

Lab::~Lab()
{
  delete msg_;
}

void Lab::Construct()
{
  // LAB /////////////////////////////////////////////////////////////
  // This is just a volume of air surrounding the detector so that
  // events can be generated on the outside.

  G4double lab_size(2. * m);
  G4Box* lab_solid =
      new G4Box("LAB", lab_size / 2., lab_size / 2., lab_size / 2.);

  G4LogicalVolume* lab_logic =
      new G4LogicalVolume(lab_solid, G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR"), "AIR");
  lab_logic->SetVisAttributes(G4VisAttributes::GetInvisible());

  // Set this volume as the wrapper for the whole geometry
  // (i.e., this is the volume that will be placed in the world)
  this->SetLogicalVolume(lab_logic);

  module_->Construct();
  G4ThreeVector cell_dim = module_->GetDimensions();

  G4LogicalVolume* module_logic = module_->GetLogicalVolume();

  new G4PVPlacement(0, G4ThreeVector(0., 0., -5.*mm - cell_dim.z()/2.),
                    module_logic, "MODULE_0",
                    lab_logic, false, 0, true);

  G4RotationMatrix rot;
  rot.rotateY(pi);
  new G4PVPlacement(G4Transform3D(rot, G4ThreeVector(0., 0., 5.*mm + cell_dim.z()/2.)),
                    module_logic, "MODULE_1", lab_logic, false, 1, true);

  //G4cout << "Placement: " << 5.*mm + cell_dim.z()/2. << ", semilunghezza: " <<  << G4endl;

}

G4ThreeVector Lab::GenerateVertex(const G4String &region) const
{
  G4ThreeVector vertex(0., 0., 0.);

  if (region == "CENTER")
  {
    return vertex;
  }
  else
  {
    G4Exception("[Lab]", "GenerateVertex()", FatalException,
                "Unknown vertex generation region!");
  }

  return vertex;

}
