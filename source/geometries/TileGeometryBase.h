// ----------------------------------------------------------------------------
// petalosim | TileGeometryBase.h
//
// This is an abstract base class for encapsulation of SiPM board geometries.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef TILE_BASE_GEOMETRY_H
#define TILE_BASE_GEOMETRY_H

#include "nexus/GeometryBase.h"
#include <G4ThreeVector.hh>


class G4VPhysicalVolume;
using namespace nexus;

class TileGeometryBase : public GeometryBase
{
public:
  /// Destructor
  virtual ~TileGeometryBase();

  void SetBoxGeom(G4int box_geom);
  G4int GetBoxGeom() const;

  void SetTileVisibility(G4bool vis);
  G4bool GetTileVisibility() const;

  void SetTileReflectivity(G4double refl);
  G4double GetTileReflectivity() const;

  void SetPDE(G4double eff);
  G4double GetPDE() const;

  void SetSiPMCells(G4int cells);
  G4int GetSiPMCells() const;

  void SetMotherPhysicalVolume(G4VPhysicalVolume *mpv);
  G4VPhysicalVolume *GetMotherPhysicalVolume() const;

  G4VPhysicalVolume *mpv_;

protected:
  /// Default constructor defined as protected so no instance of
  /// this base class can be created.
  TileGeometryBase();

private:
  G4int box_geom_;
  G4bool tile_vis_;
  G4double tile_refl_;
  G4double sipm_pde_;
  G4bool sipm_cells_;
};

// Inline definitions ///////////////////////////////////

inline TileGeometryBase::TileGeometryBase() : box_geom_(0) {}
inline TileGeometryBase::~TileGeometryBase() {}

inline void TileGeometryBase::SetBoxGeom(G4int box_geom) { box_geom_ = box_geom; }
inline G4int TileGeometryBase::GetBoxGeom() const { return box_geom_; }

inline void TileGeometryBase::SetTileVisibility(G4bool vis) { tile_vis_ = vis; }
inline G4bool TileGeometryBase::GetTileVisibility() const { return tile_vis_; }

inline void TileGeometryBase::SetTileReflectivity(G4double refl) { tile_refl_ = refl; }
inline G4double TileGeometryBase::GetTileReflectivity() const { return tile_refl_; }

inline void TileGeometryBase::SetPDE(G4double eff) { sipm_pde_ = eff; }
inline G4double TileGeometryBase::GetPDE() const { return sipm_pde_; }

inline void TileGeometryBase::SetSiPMCells(G4int cells) { sipm_cells_ = cells; }
inline G4int TileGeometryBase::GetSiPMCells() const { return sipm_cells_; }

inline void TileGeometryBase::SetMotherPhysicalVolume(G4VPhysicalVolume *mpv) { mpv_ = mpv; }
inline G4VPhysicalVolume *TileGeometryBase::GetMotherPhysicalVolume() const { return mpv_; }

#endif
