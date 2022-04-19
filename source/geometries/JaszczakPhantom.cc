// ----------------------------------------------------------------------------
// petalosim | JaszczakPhantom.cc
//
// This class implements the geometry of a Jaszczak phantom, filled with water.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "JaszczakPhantom.h"

#include "nexus/FactoryBase.h"

#include <G4Tubs.hh>
#include <G4Orb.hh>
#include <G4LogicalVolume.hh>
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4RotationMatrix.hh>

using namespace nexus;

REGISTER_CLASS(JaszczakPhantom, GeometryBase)

JaszczakPhantom::JaszczakPhantom(): GeometryBase(),
                                    cylinder_inner_diam_(216*mm),
                                    cylinder_height_(186*mm),
                                    cylinder_thickn_(3.2*mm),
                                    sphere1_d_(9.5*mm),
                                    sphere2_d_(12.7*mm),
                                    sphere3_d_(15.9*mm),
                                    sphere4_d_(19.1*mm),
                                    sphere5_d_(25.4*mm),
                                    sphere6_d_(31.8*mm),
                                    sphere_height_(127*mm),
                                    rod1_d_(3.2*mm),
                                    rod2_d_(4.8*mm),
                                    rod3_d_(6.4*mm),
                                    rod4_d_(7.9*mm),
                                    rod5_d_(9.5*mm),
                                    rod6_d_(11.1*mm),
                                    rod_height_(88*mm)
{
}


JaszczakPhantom::~JaszczakPhantom()
{
}


void JaszczakPhantom::Construct()
{
  G4String cyl_name = "CYLINDER";
  G4Tubs* cylinder_solid =
    new G4Tubs(cyl_name, 0, (cylinder_inner_diam_+2*cylinder_thickn_)/2,
               (cylinder_height_+2*cylinder_thickn_)/2., 0, twopi);

  G4Material* aluminum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  G4LogicalVolume* cylinder_logic =
    new G4LogicalVolume(cylinder_solid, aluminum, cyl_name);

  this->SetLogicalVolume(cylinder_logic);


  G4String water_name = "WATER_BCKG";
  G4Tubs* water_solid =
    new G4Tubs(water_name, 0, cylinder_inner_diam_/2, cylinder_height_/2., 0, twopi);

  G4Material* water = G4NistManager::Instance()->FindOrBuildMaterial("G4_water");
  G4LogicalVolume* water_logic =
    new G4LogicalVolume(water_solid, water, water_name);
  new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), water_logic, water_name, cylinder_logic, false, 0, true);

  // Spheres
  std::vector<G4double> sphere_radii =
    {sphere1_d_/2, sphere2_d_/2, sphere3_d_/2, sphere4_d_/2, sphere5_d_/2, sphere6_d_/2};
  auto sphere_pos_radius = cylinder_inner_diam_/4.;
  auto z = - cylinder_height_/2. + sphere_height_;

  for (unsigned long i=0; i<sphere_radii.size(); i++) {
    G4String sphere_name = "SPHERE" + std::to_string(i+1);
    G4Orb* sphere_solid = new G4Orb(sphere_name, sphere_radii[i]);
    G4LogicalVolume* sphere_logic = new G4LogicalVolume(sphere_solid, water, sphere_name);
    G4double angle = i * 60 * deg;
    auto x = sphere_pos_radius * cos(angle);
    auto y = sphere_pos_radius * sin(angle);
    new G4PVPlacement(0, G4ThreeVector(x, y, z), sphere_logic, sphere_name, water_logic, false, 0, true);
  }

  // Rods
  std::vector<G4double> rod_radii =
    {rod1_d_/2, rod2_d_/2, rod3_d_/2, rod4_d_/2, rod5_d_/2, rod6_d_/2};
  for (unsigned long i=0; i<rod_radii.size(); i++) {
    BuildRods(i, rod_radii[i], water_logic, water);
  }

}




void JaszczakPhantom::BuildRods(unsigned long n, G4double r,
                                G4LogicalVolume* mother_logic, G4Material* mat) const
  {
    // Rods
    auto d = 2 * r;
    auto z = - cylinder_height_/2. + rod_height_/2;
    //G4RotationMatrix rot_z{{0,0,1}, n*pi/3};
    G4RotationMatrix rot_z;
    rot_z.rotateZ(n*pi/3);


    // Sector displacement from centre, to accommodate gap between sectors
    auto gap =  14.4 * mm;
    auto dx  = gap * cos(pi/6);
    auto dy  = gap * sin(pi/6);
    // Displacement of first rod WRT sector corner
    dx += r * sqrt(3);
    dy += r;
    // Basis vectors of rod lattice
    const auto Ax = 2.0, Ay = 0.0;
    const auto Bx = 1.0, By = sqrt(3);
    auto a = 0;
    for (bool did_b=true ; did_b; a+=1) {
      did_b = false;
      for (auto b = 0; /*break in body*/; b+=1, did_b = true) {
        auto x = (a*Ax + b*Bx) * d + dx;
        auto y = (a*Ay + b*By) * d + dy;
        auto margin = 0.1 * mm;
        if (sqrt(x*x + y*y) + r + margin >= cylinder_inner_diam_/2.) { break; }
        auto label = std::string("ROD") + std::to_string(n);
        auto rod_solid = new G4Tubs(label, 0, r, rod_height_/2, 0, twopi);
        auto rod_logic = new G4LogicalVolume(rod_solid, mat, label);
        new G4PVPlacement(G4Transform3D(rot_z, G4ThreeVector(x, y, z)),
                          rod_logic, label, mother_logic, false, 0, true);
      }
    }
  }
