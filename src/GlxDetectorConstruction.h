// -----------------------------------------
// GlxDetectorConstruction class
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxDetectorConstruction_h
#define GlxDetectorConstruction_h 1

#include "globals.hh"
#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4RotationMatrix.hh"

#include "GlxDetectorConstructionMessenger.h"

class GlxDetectorConstructionMessenger;

class GlxDetectorConstruction : public G4VUserDetectorConstruction
{ 
public:
  GlxDetectorConstruction();
  virtual ~GlxDetectorConstruction();

public:
  virtual G4VPhysicalVolume* Construct();
  virtual void ConstructSDandField();
  void DefineMaterials();
  void SetVisualization();
  void SetRotation(G4double angle);
  void SetQuantumEfficiency(G4int id);
  

private:
  G4LogicalVolume* lExpHall;
  G4LogicalVolume* lFront;
  G4LogicalVolume* lTrigger;
  G4LogicalVolume* lDirc;
  G4LogicalVolume* lBarBox;
  G4LogicalVolume* lTankBox;
  G4LogicalVolume* lBar;
  G4LogicalVolume* lMirror;
  G4LogicalVolume* lLens1;
  G4LogicalVolume* lLens2;
  G4LogicalVolume* lLens3;
  G4LogicalVolume* lWedge;
  G4LogicalVolume* lWindow;
  G4LogicalVolume* lMcp;
  G4LogicalVolume* lPixel;
  G4LogicalVolume* lPrizmC;
  G4LogicalVolume *lTankMirror1;
  G4LogicalVolume *lTankMirror2;
  G4LogicalVolume *lTankMirror3;
  G4LogicalVolume *lTankMirror4;
  G4LogicalVolume *lFmirrorC;
  G4LogicalVolume *lFmirror;
  G4LogicalVolume *lFdp;

  
  G4VPhysicalVolume* wBar;
  G4VPhysicalVolume* wMirror;
  G4VPhysicalVolume* wDirc;
  G4VPhysicalVolume* wBarBox;

  G4Material*        defaultMaterial; // material for bars
  G4Material*        BarMaterial; // material for bars
  G4Material*        OilMaterial;
  G4Material*        H2OMaterial;
  G4Material*        MirrorMaterial; // material of mirror
  G4Material*        epotekMaterial;  
  G4Material*        Nlak33aMaterial;  
  G4Material*        frontMaterial;
  
  G4int fNRow;
  G4int fNCol;
  G4int fGeomId;
  G4int fMcpLayout;
  G4double fHall[3];
  G4double fBar[3];
  G4double fBarBox[3];
  G4double fTankBox[3];
  G4double fTankBox0[3];
  G4double fTankBox1[3];
  G4double fMirror[3];
  G4double fMirror1[3];
  G4double fMirror2[3];
  G4double fMirror3[3];
  G4double fMirror4[3];
  G4double fFdp[3];

  G4double fWindow[3];
  G4double fPrizm[4];
  G4double fMcpTotal[3];
  G4double fMcpActive[3];
  G4ThreeVector fPrismShift;
  G4double fPrismRadiatorStep;
  G4ThreeVector fCenterShift;

  GlxDetectorConstructionMessenger* fGeomMessenger;
  G4double *fQuantumEfficiency;
};

#endif
