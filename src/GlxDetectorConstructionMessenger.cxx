#include "GlxDetectorConstructionMessenger.h"
#include "GlxPrimaryGeneratorAction.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4SystemOfUnits.hh"

#include "GlxManager.h"

GlxDetectorConstructionMessenger::GlxDetectorConstructionMessenger
(GlxDetectorConstruction *GlxGeom): G4UImessenger(),
    fGlxGeom(GlxGeom)
{
  fGeomDir = new G4UIdirectory("/Glx/geom/");
  fGeomDir->SetGuidance("Geometry control");

  fAngleCmd = new G4UIcmdWithADoubleAndUnit("/Glx/geom/prtRotation",this);
  fAngleCmd->SetGuidance("Rotation angle of the prototype");
  fAngleCmd->SetParameterName("angle",true);
  fAngleCmd->SetRange("angle>=0. && angle<=180.");
  fAngleCmd->SetDefaultValue(0.);
  fAngleCmd->SetDefaultUnit("deg");

  fLensIdCmd = new G4UIcmdWithAnInteger("/Glx/geom/lensId",this);
  fLensIdCmd->SetGuidance("Lens Id");
  fLensIdCmd->SetParameterName("lenseId",true);
  fLensIdCmd->SetRange("lenseId>=0");
  fLensIdCmd->SetDefaultValue(1);

  fDetEffType = new G4UIcmdWithAnInteger("/Glx/geom/detEffType",this);
  fDetEffType->SetGuidance("Type of the detector efficiency");
  fDetEffType->SetParameterName("detEffType",true);
  fDetEffType->SetRange("detEffType>=0");
  fDetEffType->SetDefaultValue(0);
}

GlxDetectorConstructionMessenger::~GlxDetectorConstructionMessenger()
{
  delete fAngleCmd;
  delete fGeomDir;
}

void GlxDetectorConstructionMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == fAngleCmd ) {
    G4double angle = 180*deg - fAngleCmd->GetNewDoubleValue(newValue);// + 90*deg;
    fGlxGeom->SetRotation(angle);
    GlxManager::Instance()->SetAngle(angle);
    std::cout<<"angle deg   "<<angle/deg <<std::endl;
    
  } 
  
  if( command == fLensIdCmd ) {
    G4int id = fLensIdCmd->GetNewIntValue(newValue);
    GlxManager::Instance()->SetLens(id);
  }

  if( command == fDetEffType ) {
    G4int id = fDetEffType->GetNewIntValue(newValue);
    //GlxManager::Instance()->SetQuantumEfficiency(id);
    //GlxManager::Instance()->SetDetEffType();
  }
}
