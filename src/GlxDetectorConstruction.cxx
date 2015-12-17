
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
#include "G4Transform3D.hh"
#include "G4AssemblyVolume.hh"


#include "GlxManager.h"
#include "GlxTriggerSD.h"
#include "GlxBarSD.h"
#include "GlxPrizmSD.h"
#include "GlxPixelSD.h"

GlxDetectorConstruction::GlxDetectorConstruction()
  : G4VUserDetectorConstruction(){

  fGeomId = GlxManager::Instance()->GetGeometry();
  fMcpLayout = GlxManager::Instance()->GetMcpLayout();
  fLensId = GlxManager::Instance()->GetLens();
  fGap = GlxManager::Instance()->GetGap();

  fNsil = 1.406;
 
  fNRow = 5;
  fNCol = 35;
  
  fHall[0] = 3000; fHall[1] = 2000; fHall[2] = 6000;
  
  fBar[0] = 17.25; fBar[1] = 35; fBar[2] =1225*4;
  fMirror[0] = 19.761; fMirror[1] = 34.493; fMirror[2] =1;
  fPrizm[0]=33.25; fPrizm[1]=91.0; fPrizm[2]=78.9929; fPrizm[3]=27.0;
  fBarBox[0]= 18; fBarBox[1]=425; fBarBox[2]=fMirror[2]+fBar[2]+fPrizm[1];
  fWindow[0]=150; fWindow[1]=425; fWindow[2]=9.6;

  fWall = 12.7; // [mm]  = 0.5 inch thickness of the EV wall
  fMShift = (fWall+fGap)/cos(42.13*deg)/2. + 1.;//
	  
  fTankBox0[0]=582; fTankBox0[1]=2205+20; fTankBox0[2]=350; //240
  fTankBox1[0]=582; fTankBox1[1]=900; fTankBox1[2]=350; //240 

  if(fGeomId==0){
    fNRow = 5;
    fNCol = 35;
    fTankBox[0]=fTankBox0[0];
    fTankBox[1]=fTankBox0[1];
    fTankBox[2]=fTankBox0[2];
  }
  if(fGeomId==1){
    fNRow = 5;
    fNCol = 14;
    fTankBox[0]=fTankBox1[0];
    fTankBox[1]=fTankBox1[1];
    fTankBox[2]=fTankBox1[2];
  }
  
  fMirror1[0]=197; fMirror1[1]=fTankBox[1]-25;  fMirror1[2]=1; 
  fMirror2[0]=66.97; fMirror2[1]=fTankBox[1]-25;  fMirror2[2]=1;
  fMirror3[0]=422.9; fMirror3[1]=fTankBox[1]-25;  fMirror3[2]=1; //!!!!!!!!!!!!!
  fMirror4[0]=580;   fMirror4[1]=300;  fMirror4[2]=1;

  fFdp[0]=312; fFdp[1]=fTankBox[1]-25;  fFdp[2]=1;  
  
  fMcpTotal[0] = fMcpTotal[1] = 53+6; fMcpTotal[2]=0.5;
  fMcpActive[0] = fMcpActive[1] = 53; fMcpActive[2]=0.5;

 
   
  GlxManager::Instance()->SetRadiatorL(fBarBox[2]-2*fPrizm[1]);
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
  G4Box* gDirc = new G4Box("gDirc",600.,2000.,fBarBox[2]/2.+400);
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
  G4Box* gTankBox = new G4Box("gTankBox",fTankBox[0]/2.,fTankBox[1]/2.,fTankBox[2]/2.);
  lTankBox = new G4LogicalVolume(gTankBox,H2OMaterial,"lTankBox",0,0,0); // OilMaterial //BarMaterial
  
  // Mirrors in tank
  G4Box* gTankMirror1 = new G4Box("gTankMirr1",fMirror1[0]/2.,fMirror1[1]/2.,fMirror1[2]/2.);
  lTankMirror1 = new G4LogicalVolume(gTankMirror1,MirrorMaterial,"lTankMirror1",0,0,0);

  G4Box* gTankMirror2 = new G4Box("gTankMirr2",fMirror2[0]/2.,fMirror2[1]/2.,fMirror2[2]/2.);
  lTankMirror2 = new G4LogicalVolume(gTankMirror2,MirrorMaterial,"lTankMirror2",0,0,0);

  G4Box* gTankMirror3 = new G4Box("gTankMirr3",fMirror3[0]/2.-fMShift,fMirror3[1]/2.,fMirror3[2]/2.); //!!!!!!!!!!!!!
  lTankMirror3 = new G4LogicalVolume(gTankMirror3,MirrorMaterial,"lTankMirror3",0,0,0);

  G4Box* gTankMirror4 = new G4Box("gTankMirr3",fMirror4[0]/2.,fMirror4[1]/2.,fMirror4[2]/2.);
  lTankMirror4 = new G4LogicalVolume(gTankMirror4,MirrorMaterial,"lTankMirror4",0,0,0);

  Double_t pi=4.*atan(1.);
  Double_t fradius = GlxManager::Instance()->GetMirrorR();
  Double_t rot_fm = GlxManager::Instance()->GetMirrorT()*deg;
  Double_t fmx = 300; // width of the focusing mirror
  Double_t fmy = sqrt(fradius*fradius-fmx*fmx/4.);
  Double_t seg = 2*asin(fmx/(2*fradius))/deg;

  // focusing mirror made of flat segments:
  if(fLensId > 0){
    fMirrorS[0] = 2*fradius*sin(seg*deg/2/fLensId);
	fMirrorS[1] = fTankBox[1]-25;
	fMirrorS[2] = 1;
    G4Box* gSmirror = new G4Box("gSmirror",fMirrorS[0]/2.,fMirrorS[1]/2.,fMirrorS[2]/2.);
    lSmirror = new G4LogicalVolume(gSmirror, MirrorMaterial, "lSmirror", 0,0,0);
  }
  
  G4Tubs* gFmirror = new G4Tubs("gFmirror",fradius,fradius+1,fMirror1[1]/2., (-90-seg/2.)*deg,seg*deg);
  lFmirror = new G4LogicalVolume(gFmirror,MirrorMaterial,"lFmirror",0,0,0);
  
  // The FD plane
  G4Box* gFdp = new G4Box("gFdp",fFdp[0]/2.,fFdp[1]/2.,fFdp[2]/2.);
  lFdp = new G4LogicalVolume(gFdp,BarMaterial,"lFdp",0,0,0);// BarMaterial

  // The Air Gap
  if(fGap > 0.){
    G4Box* gGap = new G4Box("gGap",fFdp[0]/2.,fFdp[1]/2.,fGap/2.);
    lGap = new G4LogicalVolume(gGap,SiliconMaterial,"lGap",0,0,0);
  }
  // The FS wall of the EV
  G4Box* gWall = new G4Box("gWall",fFdp[0]/2.,fFdp[1]/2.,fWall/2.);
  lWall = new G4LogicalVolume(gWall,BarMaterial,"lWall",0,0,0);

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
  
  Double_t redge = 0.5*fWindow[0]-47.75;
  
  if(fGeomId==0){
    new G4PVPlacement(0,G4ThreeVector(0.5*fTankBox[0]-redge,0,-0.5*fBarBox[2]-fWindow[2]-0.5*fTankBox[2] ),lTankBox,"wTankBox",lDirc,false,0);
  }
  if(fGeomId==1){
    new G4PVPlacement(0,G4ThreeVector(0.5*fTankBox[0]-redge, 365+0.5*fBarBox[1],-0.5*fBarBox[2]-fWindow[2]-0.5*fTankBox[2] ),lTankBox,"wTankBox",lDirc,false,0);
    new G4PVPlacement(0,G4ThreeVector(0.5*fTankBox[0]-redge,-365-0.5*fBarBox[1],-0.5*fBarBox[2]-fWindow[2]-0.5*fTankBox[2] ),lTankBox,"wTankBox",lDirc,false,1);
  }

  G4RotationMatrix* rotm1= new G4RotationMatrix; rotm1->rotateY(90.*deg);
  new G4PVPlacement(rotm1,G4ThreeVector(redge-0.5*(fBar[0]+fMirror1[2]+fTankBox[0]),0,0.5*(fTankBox[2]-fMirror1[0])-20),lTankMirror1,"wMirror1",lTankBox,false,0);
  G4RotationMatrix* rotm2= new G4RotationMatrix; rotm2->rotateY(-60.*deg);
  new G4PVPlacement(rotm2,G4ThreeVector(redge-0.5*(fBar[0]+fMirror2[2]+fTankBox[0])+130-0.5*fMirror2[0]*cos(60*pi/180.),0,0.5*fTankBox[2]-78+0.5*fMirror2[0]*sin(60*pi/180.)),lTankMirror2,"wMirror2",lTankBox,false,0);
  G4RotationMatrix* rotm3= new G4RotationMatrix; rotm3->rotateY(0.*deg);

  //new G4PVPlacement(rotm3,G4ThreeVector(redge-0.5*(fBar[0]+fMirror3[2]+fTankBox[0]-fMirror3[0])+130,0,0.5*fTankBox[2]-78),lTankMirror3,"wMirror3",lTankBox,false,0);
  new G4PVPlacement(rotm3,G4ThreeVector(redge-0.5*(fBar[0]+fMirror3[2]+fTankBox[0]-fMirror3[0])+130 -fMShift,0,0.5*fTankBox[2]-78),lTankMirror3,"wMirror3",lTankBox,false,0);

  G4RotationMatrix* rotm4= new G4RotationMatrix; rotm4->rotateX(90.*deg);
  new G4PVPlacement(rotm4,G4ThreeVector(0, 0.5*fTankBox[1]-12,0),lTankMirror4,"wMirror3",lTankBox,false,0);
  new G4PVPlacement(rotm4,G4ThreeVector(0,-0.5*fTankBox[1]+12,0),lTankMirror4,"wMirror3",lTankBox,false,0);

  G4AssemblyVolume* assemblyFMirror = new G4AssemblyVolume();

  G4RotationMatrix* Ra= new G4RotationMatrix;   
  G4ThreeVector Ta(0.5*fmx,0,fmy);

  if(fLensId == 0){
	Ra->rotateX(90.*deg);  
    assemblyFMirror->AddPlacedVolume(lFmirror,Ta,Ra);
  }
  if(fLensId > 0){ 
	G4ThreeVector c(0.,0.,fradius*cos(seg*deg/fLensId/2.));
 	c.rotateY(-seg*deg/2. + seg*deg/fLensId/2.);
 	G4ThreeVector a(0.,0.,fmy);
 	G4ThreeVector b(0.,0.,0.);
    Ra->rotateY(+seg*deg/2. - seg*deg/2./fLensId);
 	for(int isec=0; isec<fLensId; isec++){
      b = c - a;
 	  Ta.setX(0.5*fmx + b.getX());
 	  Ta.setZ(-b.getZ());
 	  assemblyFMirror->AddPlacedVolume(lSmirror,Ta,Ra);
 	  c.rotateY(seg*deg/fLensId);
 	  Ra->rotateY(-seg*deg/fLensId);
    }	
  }

	
  // // Fill the assembly by the plates
  // Ta.setX( caloX/4. ); Ta.setY( caloY/4. ); Ta.setZ( 0. );
  // assemblyDetector->AddPlacedVolume( plateLV, G4Transform3D(Ta,Ra) );
  
  // Ta.setX( -1*caloX/4. ); Ta.setY( caloY/4. ); Ta.setZ( 0. );
  // assemblyDetector->AddPlacedVolume( plateLV, G4Transform3D(Ta,Ra) );
  
  // Now instantiate the layers
  // for(int i = 0; i < layers; i++ ){
     // Translation of the assembly inside the world
  G4ThreeVector Tm = G4ThreeVector(-0.5*fTankBox[0]+20,0,0.5*fTankBox[2]-fMirror1[0]-20);
  G4RotationMatrix *Rm = new G4RotationMatrix; Rm->rotateY(rot_fm);
  assemblyFMirror->MakeImprint(lTankBox,Tm,Rm,0);
  // }

  
  G4Box* gMcp;
  G4Box* gPixel;
  
 if(fMcpLayout>1){
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
      double shifty = (fMcpTotal[0]+3)*(j-1);
      if(fGeomId==0) shifty = shifty-0.5*fFdp[1]+110;	 
      if(fGeomId==1) shifty = shifty-0.5*fFdp[1]+97;	      

      new G4PVPlacement(0,G4ThreeVector(shiftx,shifty,0),lMcp,"wMcp", lFdp,false,mcpId);
      mcpId++;
    }
  }
 }else{
   // for layout optimization 
   gMcp = new G4Box("gMcp",fFdp[0]/2.,fFdp[1]/2.,fFdp[2]/2.);
   lMcp = new G4LogicalVolume(gMcp,BarMaterial,"lMcp",0,0,0);
   
   // The MCP Pixel
   if(fMcpLayout==0){ //one prism-size mcp with one pixel
     gPixel = new G4Box("gPixel",fFdp[0]/2.,fFdp[1]/2.,fMcpActive[2]/16.);
     lPixel = new G4LogicalVolume(gPixel,BarMaterial,"lPixel",0,0,0);
     new G4PVPlacement(0,G4ThreeVector(0,0,0),lPixel,"wPixel", lMcp,false,1);
   }
   new G4PVPlacement(0,G4ThreeVector(0,0,0),lMcp,"wMcp", lFdp,false,1);
 }
 
  G4RotationMatrix* rotmm= new G4RotationMatrix; rotmm->rotateY(42.13*deg);

  G4AssemblyVolume * assemblyPD = new G4AssemblyVolume();
  G4RotationMatrix* Rp = new G4RotationMatrix;
  Rp->rotateY(0.);
  G4ThreeVector Tpd(0.,0.,0.);
  assemblyPD->AddPlacedVolume(lFdp,Tpd,Rp);
  if(fGap > 0.){	
    G4ThreeVector Tgap(0.,0.,(fGap+fFdp[2])/2.);
    assemblyPD->AddPlacedVolume(lGap,Tgap,Rp);
  }
  G4ThreeVector Twa(0.,0.,(fWall+2.*fGap+fFdp[2])/2.);
  assemblyPD->AddPlacedVolume(lWall,Twa,Rp);
	
  double pdShiftX = redge-0.5*(fBar[0]+fMirror3[2]+fTankBox[0]-2*fMirror3[0])+130-0.5*fFdp[0]*cos(42.13*deg);
  double pdShiftZ = 0.5*fTankBox[2]-185;
  G4ThreeVector pdShift(pdShiftX,0,pdShiftZ);	
  G4RotationMatrix *rotmm1 = new G4RotationMatrix;
  rotmm1->rotateY((-42.13)*deg);
  assemblyPD->MakeImprint(lTankBox, pdShift, rotmm1, 0);

 // new G4PVPlacement(rotmm,G4ThreeVector(redge-0.5*(fBar[0]+fMirror3[2]+fTankBox[0]-2*fMirror3[0])+130-0.5*fFdp[0]*cos(42.13*deg),0,0.5*fTankBox[2]-185),lFdp,"wFdp", lTankBox,false,0);

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

  // hamamatsu H12700 quantum efficiency (as a function of photon E):	
  G4double QuantumEfficiencyPMT12700[num]=
	{0.001,0.001,0.00118865,0.00511371,0.0104755,0.0174337,0.0259711,
	0.0358296,0.046982,0.0593714,0.0729143,0.0875043,0.103016,0.119306,
	0.13622,0.153591,0.171246,0.188889,0.206372,0.223528,0.239941,0.255526,
	0.269913,0.283034,0.294369,0.303953,0.31158,0.317117,0.320523,0.321858,
	0.321271,0.31895,0.315347,0.310875,0.306056,0.301365};

  // these quantum efficiencies have to be multiplied by geometry
  //   efficiency of given PMT's
  //   for Hamamatsu by factor 0.7
  //   for Burle by factor 0.45 
  for(G4int k=0;k<36;k++)
    {
      QuantumEfficiencyB[k] =  QuantumEfficiencyB[k] * 0.45 ;
      QuantumEfficiencyPMT[k] =  QuantumEfficiencyPMT[k] *.7;
	  QuantumEfficiencyPMT12700[k] =  QuantumEfficiencyPMT12700[k] *.7;
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
  
  fQuantumEfficiency = QuantumEfficiencyIdial;//QuantumEfficiencyPMT12700;// QuantumEfficiencyPMT;
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
  if(fLensId > 0){
    new G4LogicalSkinSurface("MirrorSurface", lSmirror,MirrorOpSurface);
  }
  new G4LogicalSkinSurface("MirrorSurface", lTankMirror1,MirrorOpSurface);
  new G4LogicalSkinSurface("MirrorSurface", lTankMirror2,MirrorOpSurface);
  new G4LogicalSkinSurface("MirrorSurface", lTankMirror3,MirrorOpSurface);
  new G4LogicalSkinSurface("MirrorSurface", lTankMirror4,MirrorOpSurface);


  // // TankBox
  // G4OpticalSurface* TankOpSurface =new G4OpticalSurface("TankOpSurface",glisur,polished,dielectric_metal);
  // G4MaterialPropertiesTable *TankMPT = new G4MaterialPropertiesTable();
  // TankMPT->AddProperty("REFLECTIVITY", PhotonEnergy, PMTReflectivity, num);
  // TankOpSurface->SetMaterialPropertiesTable(TankMPT);
  // //new G4LogicalBorderSurface("TankAirSurface",wTankBox,wDirc,TankOpSurface);

  
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
  
  H2OMaterial = new G4Material("Water",density=1.000*g/cm3,ncomponents=2);
  H2OMaterial->AddElement(H, natoms=2);
  H2OMaterial->AddElement(O, natoms=1);

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

	/* as I don't know the exact material composition,
     I will use Epoxyd material composition and add
     the optical property of Silicon to this material, I'll use the density of Silicon = 1.02 */
  G4Material* Silicon = new G4Material("Silicon",density=1.02*g/cm3,ncomponents=3);
  Silicon->AddElement(C,natoms=3);
  Silicon->AddElement(H,natoms=5);
  Silicon->AddElement(O,natoms=2);
	
  // assign main materials
  if(fGeomId == 1) defaultMaterial = Vacuum;
  else defaultMaterial = Air; //Vacuum // material of world
  frontMaterial = CarbonFiber; 
  BarMaterial = SiO2; // material of all Bars, Quartz and Window
  SiliconMaterial = Silicon;
  
  OilMaterial = KamLandOil; // material of volume 1,2,3,4
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

  // absorption of silicon per 1 m - assumed no absorption
  G4double SiliconAbsorption[num] = 
    {100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,
	 100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,
	 100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,
	 100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,100.*m,
	 100.*m,100.*m,100.*m,100.*m};	

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


  //water
  const G4int nEntries = 32;
  G4double photonEnergyH2O[nEntries] =
    { 2.034*eV, 2.068*eV, 2.103*eV, 2.139*eV,
      2.177*eV, 2.216*eV, 2.256*eV, 2.298*eV,
      2.341*eV, 2.386*eV, 2.433*eV, 2.481*eV,
      2.532*eV, 2.585*eV, 2.640*eV, 2.697*eV,
      2.757*eV, 2.820*eV, 2.885*eV, 2.954*eV,
      3.026*eV, 3.102*eV, 3.181*eV, 3.265*eV,
      3.353*eV, 3.446*eV, 3.545*eV, 3.649*eV,
      3.760*eV, 3.877*eV, 4.002*eV, 4.136*eV };
  G4double H2ORefractiveIndex[nEntries] =
    { 1.3435, 1.344,  1.3445, 1.345,  1.3455,
      1.346,  1.3465, 1.347,  1.3475, 1.348,
      1.3485, 1.3492, 1.35,   1.3505, 1.351,
      1.3518, 1.3522, 1.3530, 1.3535, 1.354,
      1.3545, 1.355,  1.3555, 1.356,  1.3568,
      1.3572, 1.358,  1.3585, 1.359,  1.3595,
      1.36,   1.3608};
      
  G4double H2OAbsorption[nEntries] =
    {3.448*m,  4.082*m,  6.329*m,  9.174*m, 12.346*m, 13.889*m,
     15.152*m, 17.241*m, 18.868*m, 20.000*m, 26.316*m, 35.714*m,
     45.455*m, 47.619*m, 52.632*m, 52.632*m, 55.556*m, 52.632*m,
     52.632*m, 47.619*m, 45.455*m, 41.667*m, 37.037*m, 33.333*m,
     30.000*m, 28.500*m, 27.000*m, 24.500*m, 22.000*m, 19.500*m,
     17.500*m, 14.500*m };
       

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

  G4double SiliconRefractiveIndex[num]={
    fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,
	fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,
	fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,
	fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil,fNsil};

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
  QuartzMPT->AddProperty("ABSLENGTH",    PhotonEnergy, QuartzAbsorption,num);

  // assign this parameter table to BAR material
  BarMaterial->SetMaterialPropertiesTable(QuartzMPT);

  // Silicon material
  G4MaterialPropertiesTable* SiliconMPT = new G4MaterialPropertiesTable();
  SiliconMPT->AddProperty("RINDEX",       PhotonEnergy, SiliconRefractiveIndex,num);
  SiliconMPT->AddProperty("ABSLENGTH",    PhotonEnergy, SiliconAbsorption,num);
  SiliconMaterial->SetMaterialPropertiesTable(SiliconMPT);
	
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
  OilMaterial->SetMaterialPropertiesTable(KamLandOilMPT);

  // Water                                                
  G4MaterialPropertiesTable* WaterMPT = new G4MaterialPropertiesTable();
  WaterMPT->AddProperty("RINDEX", photonEnergyH2O, H2ORefractiveIndex, nEntries);
  WaterMPT->AddProperty("ABSLENGTH", photonEnergyH2O, H2OAbsorption, nEntries);
  H2OMaterial->SetMaterialPropertiesTable(WaterMPT);  

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

  G4VisAttributes *waTank = new G4VisAttributes(G4Colour(0.,0.33,0.5));
  waTank->SetForceWireframe(true);
  lTankBox->SetVisAttributes(waTank);

  G4VisAttributes *waBarBox = new G4VisAttributes(DircColour);
  waBarBox->SetVisibility(false);
  waBarBox->SetForceWireframe(true);
  lBarBox->SetVisAttributes(waBarBox);

  G4VisAttributes *waBar = new G4VisAttributes(G4Colour(0.,1.,0.9,0.2));
  waBar->SetVisibility(true);
  lBar->SetVisAttributes(waBar);
  lWall->SetVisAttributes(waBar);

  G4VisAttributes *waWindow = new G4VisAttributes(G4Colour(0.2,0.8,0.9));
  lWindow->SetVisAttributes(waWindow);
  if(fGap > 0.){
    lGap->SetVisAttributes(waWindow);
  }
	
  G4VisAttributes *waMirror = new G4VisAttributes(G4Colour(1.,0.7,0.2,0.5));
  waMirror->SetVisibility(true);
  lMirror->SetVisAttributes(waMirror);
  lFmirror->SetVisAttributes(waMirror);
  if(fLensId > 0){
    lSmirror->SetVisAttributes(waMirror);
  }
  lTankMirror1->SetVisAttributes(waMirror);
  lTankMirror2->SetVisAttributes(waMirror);
  lTankMirror3->SetVisAttributes(waMirror);

  G4VisAttributes *waMirror4 = new G4VisAttributes(G4Colour(1.,0.7,0.2,0.5));
  waMirror4->SetVisibility(false);
  lTankMirror4->SetVisAttributes(waMirror4);

  G4VisAttributes *waPrizm = new G4VisAttributes(G4Colour(0.,0.9,0.9,0.2));
  waPrizm->SetVisibility(true);
  //waPrizm->SetForceAuxEdgeVisible(true);
  //waPrizm->SetForceSolid(true);
  lWedge->SetVisAttributes(waPrizm);

  G4VisAttributes *waFdp = new G4VisAttributes(green);
  waFdp->SetForceWireframe(true);
  lFdp->SetVisAttributes(waFdp);

  G4VisAttributes *waMcp = new G4VisAttributes(green);
  waMcp->SetForceWireframe(true);
  lMcp->SetVisAttributes(waMcp);

  G4VisAttributes *waPixel = new G4VisAttributes(G4Colour(1,0,0,0.6));
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
  SetSensitiveDetector("lMirror",prizmSD);
  SetSensitiveDetector("lFmirror",prizmSD);
  if(fLensId > 0){
    SetSensitiveDetector("lSmirror",prizmSD);
  }
  SetSensitiveDetector("lTankMirror1",prizmSD);
  SetSensitiveDetector("lTankMirror2",prizmSD);
  SetSensitiveDetector("lTankMirror3",prizmSD);
  SetSensitiveDetector("lTankMirror4",prizmSD);

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

  // hamamatsu H12700 quantum efficiency (as a function of photon E):	
  G4double QuantumEfficiencyPMT12700[num]=
	{0.001,0.001,0.00118865,0.00511371,0.0104755,0.0174337,0.0259711,
	0.0358296,0.046982,0.0593714,0.0729143,0.0875043,0.103016,0.119306,
	0.13622,0.153591,0.171246,0.188889,0.206372,0.223528,0.239941,0.255526,
	0.269913,0.283034,0.294369,0.303953,0.31158,0.317117,0.320523,0.321858,
	0.321271,0.31895,0.315347,0.310875,0.306056,0.301365};
  
  if(id == 0 ) fQuantumEfficiency = QuantumEfficiencyIdial;
  if(id == 1 ) fQuantumEfficiency = QuantumEfficiencyPMT12700;

  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
