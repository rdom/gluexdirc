
#include "GlxDetectorConstruction.h"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4SubtractionSolid.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include "GlxManager.h"
#include "GlxTriggerSD.h"
#include "GlxBarSD.h"
#include "GlxPrizmSD.h"
#include "GlxPixelSD.h"

GlxDetectorConstruction::GlxDetectorConstruction()
  : G4VUserDetectorConstruction(){

  fGeomId = GlxManager::Instance()->GetGeometry();
  fMcpLayout = GlxManager::Instance()->GetMcpLayout();
 
  fNRow = 5;
  fNCol = 35;
  
  fHall[0] = 2000; fHall[1] = 500; fHall[2] = 6000;
  
  fBar[0] = 17.25; fBar[1] = 35; fBar[2] =1225*4;
  fMirror[0] = 19.761; fMirror[1] = 34.493; fMirror[2] =1;
  fPrizm[0]=33.25; fPrizm[1]=91.0; fPrizm[2]=78.9929; fPrizm[3]=27.0;
  fBarBox[0]= 18; fBarBox[1]=425; fBarBox[2]=fMirror[2]+fBar[2]+fPrizm[1];
  fWindow[0]=150; fWindow[1]=425; fWindow[2]=9.6;
  
  fTankBox[0]=582; fTankBox[1]=2205; fTankBox[2]=300; //240 

  fMirror1[0]=197; fMirror1[1]=2200;  fMirror1[2]=1; 
  fMirror2[0]=66.97; fMirror2[1]=2200;  fMirror2[2]=1;
  fMirror3[0]=422.9; fMirror3[1]=2200;  fMirror3[2]=1;

  fFdp[0]=312; fFdp[1]=2200;  fFdp[2]=1; 
  
  fMcpTotal[0] = fMcpTotal[1] = 53+6; fMcpTotal[2]=1;
  fMcpActive[0] = fMcpActive[1] = 53; fMcpActive[2]=1;
   
  GlxManager::Instance()->SetRadiatorL(fBar[2]);
  GlxManager::Instance()->SetRadiatorW(fBar[1]);
  GlxManager::Instance()->SetRadiatorH(fBar[0]);			  

  fGeomMessenger = new GlxDetectorConstructionMessenger(this);
  GlxManager::Instance()->AddInfo("Initialization done");
}

GlxDetectorConstruction::~GlxDetectorConstruction(){}

G4VPhysicalVolume* GlxDetectorConstruction::Construct(){
  DefineMaterials();

  // ------------- Volumes --------------

  // The experimental Hall
  G4Box* gExpHall = new G4Box("gExpHall",fHall[0],fHall[1],fHall[2]);
  lExpHall = new G4LogicalVolume(gExpHall,defaultMaterial,"lExpHall",0,0,0);
  G4VPhysicalVolume* wExpHall  = new G4PVPlacement(0,G4ThreeVector(),lExpHall,"gExpHall",0,false,0);
  
  // The DIRC
  G4Box* gDirc = new G4Box("gDirc",400.,2000.,fBarBox[2]/2.+350);
  lDirc = new G4LogicalVolume(gDirc,defaultMaterial,"lDirc",0,0,0);
  G4RotationMatrix* yRot = new G4RotationMatrix();
  yRot->rotateX(M_PI*rad);
  yRot->rotateY(M_PI/2.*rad);
  wDirc = new G4PVPlacement(yRot,G4ThreeVector(0,0,0),lDirc,"wDirc",lExpHall,false,0);

  // The bar box
  G4Box* gBarBox = new G4Box("gBarBox",100,fBarBox[1]/2.,fBarBox[2]/2.);
  lBarBox = new G4LogicalVolume(gBarBox,defaultMaterial,"lBarBox",0,0,0);
  // The Bar
  G4Box* gBar = new G4Box("gBar",fBar[0]/2.,fBar[1]/2.,fBar[2]/2.);
  lBar = new G4LogicalVolume(gBar,BarMaterial,"lBar",0,0,0);
  // The Mirror
  G4Box* gMirror = new G4Box("gMirror",fMirror[0]/2.,fMirror[1]/2.,fMirror[2]/2.);
  lMirror = new G4LogicalVolume(gMirror,MirrorMaterial,"lMirror",0,0,0);
  // The Wedge
  G4Trap* gWedge = new G4Trap("gWedge",fPrizm[0],fPrizm[1],fPrizm[2],fPrizm[3]);
  lWedge = new G4LogicalVolume(gWedge, BarMaterial,"lWedge",0,0,0);
  G4RotationMatrix* xRot = new G4RotationMatrix();
  xRot->rotateX(-M_PI/2.*rad);
  // The Window
  G4Box* gWindow = new G4Box("gWindow",fWindow[0]/2.,fWindow[1]/2.,fWindow[2]/2.);
  lWindow = new G4LogicalVolume(gWindow,BarMaterial,"lWindow",0,0,0);
  // The tank box
  G4Box* gTankBox = new G4Box("gBarBox",fTankBox[0]/2.,fTankBox[1]/2.,fTankBox[2]/2.);
  lTankBox = new G4LogicalVolume(gTankBox,OilMaterial,"lTankBox",0,0,0); //OilMaterial //BarMaterial

  // Mirrors in tank
  G4Box* gTankMirror1 = new G4Box("gTankMirr1",fMirror1[0]/2.,fMirror1[1]/2.,fMirror1[2]/2.);
  G4LogicalVolume *lTankMirror1 = new G4LogicalVolume(gTankMirror1,MirrorMaterial,"lTankMirror1",0,0,0);

  G4Box* gTankMirror2 = new G4Box("gTankMirr2",fMirror2[0]/2.,fMirror2[1]/2.,fMirror2[2]/2.);
  G4LogicalVolume *lTankMirror2 = new G4LogicalVolume(gTankMirror2,MirrorMaterial,"lTankMirror2",0,0,0);

  G4Box* gTankMirror3 = new G4Box("gTankMirr3",fMirror3[0]/2.,fMirror3[1]/2.,fMirror3[2]/2.);
  G4LogicalVolume *lTankMirror3 = new G4LogicalVolume(gTankMirror3,MirrorMaterial,"lTankMirror3",0,0,0);

  G4Tubs* gFmirror = new G4Tubs("gFmirror",600, 601,1100, 90*deg,30*deg);
  G4LogicalVolume *lFmirror = new G4LogicalVolume(gFmirror,MirrorMaterial,"lFmirror",0,0,0);
  
  // The FD plane
  G4Box* gFdp = new G4Box("gFdp",fFdp[0]/2.,fFdp[1]/2.,fFdp[2]/2.);
  G4LogicalVolume* lFdp = new G4LogicalVolume(gFdp,BarMaterial,"lFdp",0,0,0);// BarMaterial

  for(Int_t i=0; i<12; i++){
    G4double yshift = (fBar[1]+0.15)*i - fBarBox[1]/2. + fBar[1]/2.;
    new G4PVPlacement(0,G4ThreeVector(0,yshift,0.5*(fBar[2]+fPrizm[1])),lMirror,"wMirror", lBarBox,false,1);
    new G4PVPlacement(0,G4ThreeVector(0,yshift,0.5*(fPrizm[1]-fMirror[2])),lBar,"wBar", lBarBox,false,i); 
    new G4PVPlacement(xRot,G4ThreeVector((fPrizm[2]+fPrizm[3])/4.-fBar[0]/2.,(fPrizm[0]-fBar[1])/2.+yshift,0.5*(-fBar[2]-fMirror[2])),lWedge,"wWedge", lBarBox,false,i);
  }

  new G4PVPlacement(0,G4ThreeVector(0,-795,0),lBarBox,"wBarBox",lDirc,false,0);
  new G4PVPlacement(0,G4ThreeVector(47.75,-795,0.5*(-fBarBox[2]-fWindow[2])),lWindow,"wWindow",lDirc,false,0);
  new G4PVPlacement(0,G4ThreeVector(0,-365,0),lBarBox,"wBarBox",lDirc,false,1);
  new G4PVPlacement(0,G4ThreeVector(47.75,-365,0.5*(-fBarBox[2]-fWindow[2])),lWindow,"wWindow",lDirc,false,1);
  new G4PVPlacement(0,G4ThreeVector(0, 365,0),lBarBox,"wBarBox",lDirc,false,2);
  new G4PVPlacement(0,G4ThreeVector(47.75, 365,0.5*(-fBarBox[2]-fWindow[2])),lWindow,"wWindow",lDirc,false,2);
  new G4PVPlacement(0,G4ThreeVector(0, 795,0),lBarBox,"wBarBox",lDirc,false,3);
  new G4PVPlacement(0,G4ThreeVector(47.75, 795,0.5*(-fBarBox[2]-fWindow[2])),lWindow,"wWindow",lDirc,false,3);

  Double_t pi=4.*atan(1.);
  
  Double_t redge = 0.5*fWindow[0]-47.75;
  new G4PVPlacement(0,G4ThreeVector(0.5*fTankBox[0]-redge,0,-0.5*fBarBox[2]-fWindow[2]-0.5*fTankBox[2] ),lTankBox,"wTankBox",lDirc,false,0);
  G4RotationMatrix* rotm1= new G4RotationMatrix; rotm1->rotateY(90.*deg);
  new G4PVPlacement(rotm1,G4ThreeVector(redge-0.5*(fBar[0]+fMirror1[2]+fTankBox[0]),0,0.5*(fTankBox[2]-fMirror1[0])-20),lTankMirror1,"wMirror1",lTankBox,false,0);
  G4RotationMatrix* rotm2= new G4RotationMatrix; rotm2->rotateY(-60.*deg);
  new G4PVPlacement(rotm2,G4ThreeVector(redge-0.5*(fBar[0]+fMirror2[2]+fTankBox[0])+130-0.5*fMirror2[0]*cos(60*pi/180.),0,0.5*fTankBox[2]-78+0.5*fMirror2[0]*sin(60*pi/180.)),lTankMirror2,"wMirror2",lTankBox,false,0);
  G4RotationMatrix* rotm3= new G4RotationMatrix; rotm3->rotateY(0.*deg);
  new G4PVPlacement(rotm3,G4ThreeVector(redge-0.5*(fBar[0]+fMirror3[2]+fTankBox[0]-fMirror3[0])+130,0,0.5*fTankBox[2]-78),lTankMirror3,"wMirror3",lTankBox,false,0);
  
  G4RotationMatrix* rotm4= new G4RotationMatrix; rotm4->rotateX(90.*deg); 
  new G4PVPlacement(rotm4,G4ThreeVector(28,0,450),lFmirror,"wFmirror",lTankBox,false,0);
 
  G4Box* gMcp;
  G4Box* gPixel;

  // The MCP
  gMcp = new G4Box("gMcp",fMcpTotal[0]/2.,fMcpTotal[1]/2.,fMcpTotal[2]/2.);
  lMcp = new G4LogicalVolume(gMcp,BarMaterial,"lMcp",0,0,0);// BarMaterial
  
  // The MCP Pixel
  int mcpDimx = 8;
  int mcpDimy = 8;
  if(fGeomId>101) {
    mcpDimx = fGeomId/100;
    mcpDimy = fGeomId%100;
  }
  gPixel = new G4Box("gPixel",fMcpActive[0]/(2*(double)mcpDimx),fMcpActive[1]/(2*(double)mcpDimy),fMcpActive[2]/20.);
  lPixel = new G4LogicalVolume(gPixel,BarMaterial,"lPixel",0,0,0);
  

  int pixelId = 0;
  for(int i=0; i<mcpDimx; i++){
    for(int j=mcpDimy-1; j>=0; j--){
      double shiftx = i*(fMcpActive[0]/(double)mcpDimx)-fMcpActive[0]/2.+fMcpActive[0]/(2*(double)mcpDimx);
      double shifty = j*(fMcpActive[0]/(double)mcpDimy)-fMcpActive[0]/2.+fMcpActive[0]/(2*(double)mcpDimy);
      new G4PVPlacement(0,G4ThreeVector(shiftx,shifty,0),lPixel,"wPixel", lMcp,false,64-pixelId++);      
    }
  }

  int mcpId = 0;
  for(int j=0; j<fNCol; j++){
    for(int i=0; i<fNRow; i++){
      double shiftx = i*(fMcpTotal[0]+3)-fFdp[0]/2.+fMcpTotal[0]/2.; 
      double shifty = (fMcpTotal[0]+3)*(j-1)-1000; 

      new G4PVPlacement(0,G4ThreeVector(shiftx,shifty,0),lMcp,"wMcp", lFdp,false,mcpId);
      mcpId++;
    }
  }

  G4RotationMatrix* rotmm= new G4RotationMatrix; rotmm->rotateY(42.13*deg);
  new G4PVPlacement(rotmm,G4ThreeVector(redge-0.5*(fBar[0]+fMirror3[2]+fTankBox[0]-2*fMirror3[0])+130-0.5*fFdp[0]*cos(42.13*deg),0,-32),lFdp,"wFdp", lTankBox,false,0);

  const G4int num = 36; 
  G4double WaveLength[num];
  G4double PhotonEnergy[num];
  G4double PMTReflectivity[num];
  G4double EfficiencyMirrors[num];
  const G4double LambdaE = 2.0 * 3.14159265358979323846 * 1.973269602e-16 * m * GeV;
  for(int i=0;i<num;i++){
    WaveLength[i]= (300 +i*10)*nanometer;
    PhotonEnergy[num-(i+1)]= LambdaE/WaveLength[i];
    PMTReflectivity[i]=0.;
    EfficiencyMirrors[i]=0; 
  }

  /***************** QUANTUM EFFICIENCY OF BURLE AND HAMAMTSU PMT'S *****/

  //ideal pmt quantum efficiency
  G4double QuantumEfficiencyIdial[num]=
    {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
     1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
     1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
     1.0,1.0,1.0,1.0,1.0,1.0};

  // Burle PMT's 
  G4double QuantumEfficiencyB[num] =
    {0.,0.001,0.002,0.005,0.01,0.015,0.02,0.03,0.04,0.05,0.06,
     0.07,0.09,0.1,0.13,0.15,0.17,0.2,0.24,0.26,0.28,0.282,0.284,0.286,
     0.288,0.29,0.28,0.26,0.24,0.22,0.20,0.18,0.15,0.13,0.12,0.10};
  
  //hamamatsu pmt quantum efficiency
  G4double QuantumEfficiencyPMT[num]=
    {0.001,0.002,0.004,0.007,0.011,0.015,0.020,0.026,0.033,0.040,0.045,
     0.056,0.067,0.085,0.109,0.129,0.138,0.147,0.158,0.170,
     0.181,0.188,0.196,0.203,0.206,0.212,0.218,0.219,0.225,0.230,
     0.228,0.222,0.217,0.210,0.199,0.177};

  // these quantum efficiencies have to be multiplied by geometry
  //   efficiency of given PMT's
  //   for Hamamatsu by factor 0.7
  //   for Burle by factor 0.45 
  for(G4int k=0;k<36;k++)
    {
      QuantumEfficiencyB[k] =  QuantumEfficiencyB[k] * 0.45 ;
      QuantumEfficiencyPMT[k] =  QuantumEfficiencyPMT[k] *.7;
    }
 
  // G4double QuantumEfficiency[num]= 
  //    { 1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,
  //      1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,
  //      1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.};

  //  G4double QuantumEfficiencyPMT[num] =
  //    { 1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,
  //      1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,
  //      1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.};
  
  /* define quantum efficiency for burle PMT's - the same efficiency is 
     assigned to pads and also to slots !!!! */
    
  //burle pmt - bigger slots => logicPad
  G4MaterialPropertiesTable* PhotocatodBurleMPT = new G4MaterialPropertiesTable();
  PhotocatodBurleMPT->AddProperty("EFFICIENCY",  PhotonEnergy,QuantumEfficiencyB,num);
  PhotocatodBurleMPT->AddProperty("REFLECTIVITY",PhotonEnergy,PMTReflectivity,num);

 
  G4OpticalSurface* BurlePMTOpSurface= 
    new G4OpticalSurface("BurlePMTOpSurface",glisur,polished,
			 dielectric_metal);
  BurlePMTOpSurface->SetMaterialPropertiesTable(PhotocatodBurleMPT);

  // // assignment for pad
  // if(burle)
  //   new G4LogicalSkinSurface("BurlePMTSurface",logicBurPad,BurlePMTOpSurface); 

  // if(burle1)
  //   new G4LogicalSkinSurface("Burle1PMTSurface",logicBur1Pad,BurlePMTOpSurface); 

  /* hamamatsu pmt's - smaller slots => quantum efficiency again
     assign to slot and pad */
  
  fQuantumEfficiency = QuantumEfficiencyIdial; //QuantumEfficiencyPMT;//QuantumEfficiencyIdial;
  G4MaterialPropertiesTable* PhotocatodHamamatsuMPT = new G4MaterialPropertiesTable();
  PhotocatodHamamatsuMPT->AddProperty("EFFICIENCY",  PhotonEnergy,fQuantumEfficiency,num);
  PhotocatodHamamatsuMPT->AddProperty("REFLECTIVITY",PhotonEnergy,PMTReflectivity,num);

 
  G4OpticalSurface* HamamatsuPMTOpSurface= 
    new G4OpticalSurface("HamamatsuPMTOpSurface",glisur,polished,dielectric_metal);
  HamamatsuPMTOpSurface->SetMaterialPropertiesTable(PhotocatodHamamatsuMPT);

  // // assignment to pad
  // if(hamamatsu8500)
  new G4LogicalSkinSurface("HamamatsuPMTSurface",lPixel,HamamatsuPMTOpSurface);

  // Mirror
  G4OpticalSurface* MirrorOpSurface= 
    new G4OpticalSurface("MirrorOpSurface",glisur,polished,dielectric_metal);
  
  G4double ReflectivityMirrorBar[num]={
    0.8755,0.882,0.889,0.895,0.9,0.9025,0.91,0.913,0.9165,0.92,0.923,
    0.9245,0.9285,0.932,0.934,0.935,0.936,0.9385,0.9395,0.94,
    0.9405,0.9405,0.9405,0.9405,0.94,0.9385,0.936,0.934,
    0.931,0.9295,0.928,0.928,0.921,0.92,0.927,0.9215};

  G4MaterialPropertiesTable *MirrorMPT = new G4MaterialPropertiesTable();
  MirrorMPT->AddProperty("REFLECTIVITY", PhotonEnergy, ReflectivityMirrorBar, num);
  MirrorMPT->AddProperty("EFFICIENCY", PhotonEnergy, EfficiencyMirrors,   num);
  
  MirrorOpSurface->SetMaterialPropertiesTable(MirrorMPT);
  new G4LogicalSkinSurface("MirrorSurface", lMirror,MirrorOpSurface);
  new G4LogicalSkinSurface("MirrorSurface", lFmirror,MirrorOpSurface);
  new G4LogicalSkinSurface("MirrorSurface", lTankMirror1,MirrorOpSurface);
  new G4LogicalSkinSurface("MirrorSurface", lTankMirror2,MirrorOpSurface);
  new G4LogicalSkinSurface("MirrorSurface", lTankMirror3,MirrorOpSurface);


  SetVisualization();

  return wExpHall;
}

void GlxDetectorConstruction::DefineMaterials(){
  G4String symbol;             //a=mass of a mole;
  G4double a, z,  density;      //z=mean number of protons;  

  G4int ncomponents, natoms;
  G4double fractionmass;

  // define Elements
  G4Element* H  = new G4Element("Hydrogen",symbol="H" , z= 1., a= 1.01*g/mole);
  G4Element* C  = new G4Element("Carbon"  ,symbol="C" , z= 6., a= 12.01*g/mole);
  G4Element* N  = new G4Element("Nitrogen",symbol="N" , z= 7., a= 14.01*g/mole);
  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);
  G4Element* Si = new G4Element("Silicon" ,symbol="Si", z= 14., a= 28.09*g/mole);

  G4Element* Al = new G4Element("Aluminum",symbol="Al",z=13.,a=26.98*g/mole);
  
  G4Material* H2O = new G4Material("Water",density=1.000*g/cm3,ncomponents=2);
  H2O->AddElement(H, natoms=2);
  H2O->AddElement(O, natoms=1);

  // quartz material = SiO2
  G4Material* SiO2 = new G4Material("quartz",density= 2.200*g/cm3, ncomponents=2);
  SiO2->AddElement(Si, natoms=1);
  SiO2->AddElement(O , natoms=2);

  Nlak33aMaterial  = new G4Material("Nlak33a",density= 4.220*g/cm3, ncomponents=2);
  Nlak33aMaterial->AddElement(Si, natoms=1);
  Nlak33aMaterial->AddElement(O , natoms=2);

  G4Material* Vacuum = new G4Material("interGalactic", 1., 1.008*g/mole, 
				      1.e-25*g/cm3, kStateGas, 
				      2.73*kelvin, 3.e-18*pascal);
  G4Material* Air = new G4Material("Air"  , density= 1.290*mg/cm3, ncomponents=2);
  Air->AddElement(N, fractionmass=0.7);
  Air->AddElement(O, fractionmass=0.3);

  G4Material* Aluminum = new G4Material("Aluminum",density=2.7*g/cm3,ncomponents=1);
  Aluminum->AddElement(Al,fractionmass=1.0);

  G4Material* KamLandOil = new G4Material("KamLandOil",density=0.914*g/cm3,ncomponents=2);
  KamLandOil->AddElement(C,natoms=12);
  KamLandOil->AddElement(H,natoms=26);

  G4Material* CarbonFiber = new G4Material("CarbonFiber", density=0.145*g/cm3, ncomponents=1);
  CarbonFiber->AddElement(C,fractionmass=1.0);
			

  /* as I don't know the exact material composition,
     I will use Epoxyd material composition and add
     the optical property of Epotek to this material */

  G4Material* Epotek = new G4Material("Epotek",density=1.2*g/cm3,ncomponents=3);

  Epotek->AddElement(C,natoms=3);
  Epotek->AddElement(H,natoms=5);
  Epotek->AddElement(O,natoms=2);


  // assign main materials
  if(fGeomId == 1) defaultMaterial = Vacuum;
  else defaultMaterial = Air; //Vacuum // material of world
  frontMaterial = CarbonFiber; 
  BarMaterial = SiO2; // material of all Bars, Quartz and Window
  
  OilMaterial = H2O; //KamLandOil; // material of volume 1,2,3,4
  MirrorMaterial = Aluminum; // mirror material
  epotekMaterial = Epotek; // Epotek material - glue between bars


  // ------------ Generate & Add Material Properties Table ------------

  static const G4double LambdaE = 2.0 * 3.14159265358979323846 * 1.973269602e-16 * m * GeV;
  const G4int num = 36;
  G4double WaveLength[num];
  G4double Absorption[num]; // default value for absorption
  G4double AirAbsorption[num]; // absorption value for air
  G4double AirRefractiveIndex[num]; // air refractive index
  G4double PhotonEnergy[num]; // energy of photons which correspond to the given 
  // refractive or absoprtion values

  G4double PhotonEnergyNlak33a[76] = {1,1.2511,1.26386,1.27687,1.29016,1.30372,1.31758,1.33173,1.34619,1.36097,1.37607,1.39152,1.40731,1.42347,1.44,1.45692,1.47425,1.49199,1.51016,1.52878,1.54787,1.56744,1.58751,1.6081,1.62923,1.65092,1.6732,1.69609,1.71961,1.7438,1.76868,1.79427,1.82062,1.84775,1.87571,1.90452,1.93423,1.96488,1.99652,2.0292,2.06296,2.09787,2.13398,2.17135,2.21006,2.25017,2.29176,2.33492,2.37973,2.42631,2.47473,2.52514,2.57763,2.63236,2.68946,2.7491,2.81143,2.87666,2.94499,3.01665,3.09187,3.17095,3.25418,3.34189,3.43446,3.53231,3.6359,3.74575,3.86244,3.98663,4.11908,4.26062,4.41225,4.57506,4.75035,4.93961};

  /*************************** ABSORPTION COEFFICIENTS *****************************/

  // absorption of KamLandOil per 50 cm - from jjv
  G4double KamLandOilAbsorption[num]=
    {0.97469022,0.976603956,0.978511548,0.980400538,0.982258449,0.984072792,
     0.985831062,0.987520743,0.989129303,0.990644203,0.992052894,
     0.993342822,0.994501428,0.995516151,0.996374433,0.997063719,
     0.997571464,0.997885132,0.997992205,0.997880183,0.997536591,
     0.99,0.98,0.97,0.96,0.94,0.93,0.924507,0.89982,0.883299,
     0.85657,0.842637,0.77020213,0.65727,0.324022,0.019192};

  // absorption of quartz per 1 m - from jjv
  G4double QuartzAbsorption[num] = 
    {0.999572036,0.999544661,0.999515062,0.999483019,0.999448285,
     0.999410586,0.999369611,0.999325013,0.999276402,0.999223336,
     0.999165317,0.999101778,0.999032079,0.998955488,0.998871172,
     0.998778177,0.99867541,0.998561611,0.998435332,0.998294892,0.998138345,
     0.997963425,0.997767484,0.997547418,
     0.99729958,0.99701966,0.99670255,0.996342167,0.995931242,0.995461041,
     0.994921022,0.994298396,0.993577567,0.992739402,0.991760297,0.990610945};
  
  // absorption of epotek per one layer - thicknes 0.001'' - from jjv
  G4double EpotekAbsorption[num] = 
    {0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.9999,0.9998,0.9995,0.999,0.998,0.997,0.996,0.9955,0.993,
     0.9871,0.9745};

  //N-Lak 33a
  G4double Nlak33aAbsorption[76]={371813,352095,331021,310814,291458,272937,255238,238342,222234,206897,192313,178463,165331,152896,141140,130043,119585,109747,100507,91846.3,83743.1,76176.7,69126.1,62570.2,56488,50858.3,45660.1,40872.4,36474.6,32445.8,28765.9,25414.6,22372.2,19619.3,17136.9,14906.5,12910.2,11130.3,9550.13,8153.3,6924.25,5848.04,4910.46,4098.04,3398.06,2798.54,2288.32,1856.99,1494.92,1193.28,943.973,739.657,573.715,440.228,333.94,250.229,185.064,134.967,96.9664,68.5529,47.6343,32.4882,21.7174,14.2056,9.07612,5.65267,3.4241,2.01226,1.14403,0.62722,0.330414,0.166558,0.0799649,0.0363677,0.0155708,0.00623089};
  

  G4double EpotekThickness = 0.001*2.54*cm;
  for(int i=0;i<num;i++){
    WaveLength[i]= (300 +i*10)*nanometer;
    Absorption[i]= 100*m; // not true, just due to definiton -> not absorb any
    AirAbsorption[i] = 4.*cm; // if photon in the air -> kill it immediately
    AirRefractiveIndex[i] = 1.; 
    PhotonEnergy[num-(i+1)]= LambdaE/WaveLength[i];

    /* as the absorption is given per length and G4 needs 
       mean free path length, calculate it here
       mean free path length - taken as probability equal 1/e
       that the photon will be absorbed */
      
    EpotekAbsorption[i] = (-1)/log(EpotekAbsorption[i])*EpotekThickness;
    QuartzAbsorption[i] = (-1)/log(QuartzAbsorption[i])*100*cm;
    KamLandOilAbsorption[i] = (-1)/log(KamLandOilAbsorption[i])*50*cm;
  }

  /**************************** REFRACTIVE INDEXES ****************************/
  
  // only phase refractive indexes are necessary -> g4 calculates group itself !!
  
  G4double QuartzRefractiveIndex[num]={
    1.456535,1.456812,1.4571,1.457399,1.457712,1.458038,1.458378,
    1.458735,1.459108,1.4595,1.459911,1.460344,1.460799,1.46128,
    1.461789,1.462326,1.462897,1.463502,1.464146,1.464833,
    1.465566,1.46635,1.46719,1.468094,1.469066,1.470116,1.471252,1.472485,
    1.473826,1.475289,1.476891,1.478651,1.480592,1.482739,1.485127,1.487793};

  G4double EpotekRefractiveIndex[num]={
    1.554034,1.555575,1.55698,1.558266,1.559454,1.56056,1.561604,
     1.562604,1.563579,1.564547,1.565526,1.566536,1.567595,
     1.568721,1.569933,1.57125,1.57269,1.574271,1.576012,
     1.577932,1.580049,1.582381,1.584948,1.587768,1.590859,
     1.59424,1.597929,1.601946,1.606307,1.611033,1.616141,1.621651,1.62758,
     1.633947,1.640771,1.64807};

  G4double KamLandOilRefractiveIndex[num]={
    1.433055,1.433369,1.433698,1.434045,1.434409,1.434793,1.435198,
    1.435626,1.436077,1.436555,1.4371,1.4376,1.4382,1.4388,1.4395,
    1.4402,1.4409,1.4415,1.4425,1.4434,1.4444,1.4455,1.4464,1.4479,1.4501,
    1.450428,1.451976,1.453666,1.455513,1.45754,1.45977,1.462231,1.464958,
    1.467991,1.471377,1.475174};

  double Nlak33aRefractiveIndex[76]={1.73816,1.73836,1.73858,1.73881,1.73904,1.73928,1.73952,1.73976,1.74001,1.74026,1.74052,1.74078,1.74105,1.74132,1.7416,1.74189,1.74218,1.74249,1.74279,1.74311,1.74344,1.74378,1.74412,1.74448,1.74485,1.74522,1.74562,1.74602,1.74644,1.74687,1.74732,1.74779,1.74827,1.74878,1.7493,1.74985,1.75042,1.75101,1.75163,1.75228,1.75296,1.75368,1.75443,1.75521,1.75604,1.75692,1.75784,1.75882,1.75985,1.76095,1.76211,1.76335,1.76467,1.76608,1.76758,1.7692,1.77093,1.77279,1.7748,1.77698,1.77934,1.7819,1.7847,1.78775,1.79111,1.79481,1.79889,1.80343,1.8085,1.81419,1.82061,1.8279,1.83625,1.84589,1.85713,1.87039};

  G4double MirrorReflectivity[num] = 
    {0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
      0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,
     0.99999999,0.99999999,0.99999999,0.99999999,0.99999999,0.99999999};
 
  
  /* ASSIGNING REFRACTIVE AND ABSORPTION PROPERTIES TO THE GIVEN MATERIALS */

  // Quartz material => Si02
  G4MaterialPropertiesTable* QuartzMPT = new G4MaterialPropertiesTable();
  QuartzMPT->AddProperty("RINDEX",       PhotonEnergy, QuartzRefractiveIndex,num);
  QuartzMPT->AddProperty("ABSLENGTH",    PhotonEnergy, QuartzAbsorption,           num);

  // assign this parameter table to BAR material
  BarMaterial->SetMaterialPropertiesTable(QuartzMPT);

  // Air
  G4MaterialPropertiesTable* AirMPT = new G4MaterialPropertiesTable();
  AirMPT->AddProperty("RINDEX",    PhotonEnergy, AirRefractiveIndex, num);
  AirMPT->AddProperty("ABSLENGTH", PhotonEnergy, AirAbsorption,      num);
  //  assign this parameter table to the air 
  defaultMaterial->SetMaterialPropertiesTable(AirMPT);


  // Mirror                                              
  G4MaterialPropertiesTable* MirrorMPT = new G4MaterialPropertiesTable();
  MirrorMPT->AddProperty("RINDEX", PhotonEnergy, KamLandOilRefractiveIndex, num);
  MirrorMPT->AddProperty("REFLECTIVITY", PhotonEnergy, MirrorReflectivity, num);
  MirrorMaterial->SetMaterialPropertiesTable(MirrorMPT);  



  // KamLandOil                                                
  G4MaterialPropertiesTable* KamLandOilMPT = new G4MaterialPropertiesTable();
  KamLandOilMPT->AddProperty("RINDEX", PhotonEnergy, KamLandOilRefractiveIndex, num);
  KamLandOilMPT->AddProperty("ABSLENGTH", PhotonEnergy, KamLandOilAbsorption, num);
  // assing this parameter table  to the KamLandOil
  OilMaterial->SetMaterialPropertiesTable(KamLandOilMPT);  

  // N-Lak 33a                                                
  G4MaterialPropertiesTable* Nlak33aMPT = new G4MaterialPropertiesTable();
  Nlak33aMPT->AddProperty("RINDEX", PhotonEnergyNlak33a, Nlak33aRefractiveIndex, 76);
  Nlak33aMPT->AddProperty("ABSLENGTH",PhotonEnergyNlak33a, Nlak33aAbsorption, 76);
  Nlak33aMaterial->SetMaterialPropertiesTable(Nlak33aMPT);  

  // Epotek Glue                                        
  G4MaterialPropertiesTable* EpotekMPT = new G4MaterialPropertiesTable();
  EpotekMPT->AddProperty("RINDEX", PhotonEnergy, EpotekRefractiveIndex, num);
  EpotekMPT->AddProperty("ABSLENGTH", PhotonEnergy, EpotekAbsorption, num);
  // assign this parameter table to the epotek
  epotekMaterial->SetMaterialPropertiesTable(EpotekMPT);

}

void GlxDetectorConstruction::SetVisualization(){

  G4Colour blue = G4Colour(0.0,0.0,1.0);
  G4Colour green = G4Colour(0.0,1.0,.0);
  G4Colour red = G4Colour(1.0,0.0,.0); 
  G4Colour DircColour = G4Colour(1.,1.0,0.);

  G4VisAttributes *waExpHall = new G4VisAttributes(DircColour);
  waExpHall->SetVisibility(false);
  lExpHall->SetVisAttributes(waExpHall);

  G4VisAttributes *waDirc = new G4VisAttributes(DircColour);
  waDirc->SetVisibility(false);
  lDirc->SetVisAttributes(waDirc);

  G4VisAttributes *waBarBox = new G4VisAttributes(DircColour);
  waBarBox->SetVisibility(false);
  waBarBox->SetForceWireframe(true);
  lBarBox->SetVisAttributes(waBarBox);

  G4VisAttributes *waBar = new G4VisAttributes(G4Colour(0.,1.,0.9,0.2));
  waBar->SetVisibility(true);
  lBar->SetVisAttributes(waBar);

  G4VisAttributes *waMirror = new G4VisAttributes(G4Colour(1.,1.,0.9));
  waMirror->SetVisibility(true);
  lMirror->SetVisAttributes(waMirror);

  G4VisAttributes *waPrizm = new G4VisAttributes(G4Colour(0.,0.9,0.9,0.2));
  waPrizm->SetVisibility(true);
  //waPrizm->SetForceAuxEdgeVisible(true);
  //waPrizm->SetForceSolid(true);
  lWedge->SetVisAttributes(waPrizm);

  G4VisAttributes *waMcp = new G4VisAttributes(green);
  waMcp->SetForceWireframe(true);
  lMcp->SetVisAttributes(waMcp);

  G4VisAttributes *waPixel = new G4VisAttributes(red);
  waPixel->SetForceWireframe(true);
  lPixel->SetVisAttributes(waPixel);

}

void GlxDetectorConstruction::ConstructSDandField(){
  // Sensitive detectors
  GlxPixelSD* pixelSD = new GlxPixelSD("PixelSD", "PixelHitsCollection", 0);
  SetSensitiveDetector("lPixel",pixelSD);
  //SetSensitiveDetector("lScan",pixelSD);

  GlxPrizmSD* prizmSD = new GlxPrizmSD("PrizmSD", "PrizmHitsCollection", 0);
  SetSensitiveDetector("lWedge",prizmSD);

  GlxBarSD* barSD = new GlxBarSD("BarSD", "BarHitsCollection", 0);
  SetSensitiveDetector("lBar",barSD);
  // Magnetic field
}

void GlxDetectorConstruction::SetRotation(G4double angle){
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void GlxDetectorConstruction::SetQuantumEfficiency(G4int id){
  const G4int num = 36;
  //ideal pmt quantum efficiency
  G4double QuantumEfficiencyIdial[num]=
    {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
     1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
     1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
     1.0,1.0,1.0,1.0,1.0,1.0};

  // Burle PMT's 
  G4double QuantumEfficiencyB[num] =
    {0.,0.001,0.002,0.005,0.01,0.015,0.02,0.03,0.04,0.05,0.06,
     0.07,0.09,0.1,0.13,0.15,0.17,0.2,0.24,0.26,0.28,0.282,0.284,0.286,
     0.288,0.29,0.28,0.26,0.24,0.22,0.20,0.18,0.15,0.13,0.12,0.10};
  
  //hamamatsu pmt quantum efficiency
  G4double QuantumEfficiencyPMT[num]=
    {0.001,0.002,0.004,0.007,0.011,0.015,0.020,0.026,0.033,0.040,0.045,
     0.056,0.067,0.085,0.109,0.129,0.138,0.147,0.158,0.170,
     0.181,0.188,0.196,0.203,0.206,0.212,0.218,0.219,0.225,0.230,
     0.228,0.222,0.217,0.210,0.199,0.177};
  
  if(id == 0 ) fQuantumEfficiency = QuantumEfficiencyIdial;
  if(id == 1 ) fQuantumEfficiency = QuantumEfficiencyPMT;

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
