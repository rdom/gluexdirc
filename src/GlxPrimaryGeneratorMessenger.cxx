#include "GlxPrimaryGeneratorMessenger.h"
#include "GlxPrimaryGeneratorAction.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GlxPrimaryGeneratorMessenger::
  GlxPrimaryGeneratorMessenger(GlxPrimaryGeneratorAction* GlxGun)
  : G4UImessenger(),
    fGlxAction(GlxGun)
{
  fGunDir = new G4UIdirectory("/Glx/gun/");
  fGunDir->SetGuidance("PrimaryGenerator control");

  fPolarCmd =
           new G4UIcmdWithADoubleAndUnit("/Glx/gun/optPhotonPolar",this);
  fPolarCmd->SetGuidance("Set linear polarization");
  fPolarCmd->SetGuidance("  angle w.r.t. (k,n) plane");
  fPolarCmd->SetParameterName("angle",true);
  fPolarCmd->SetUnitCategory("Angle");
  fPolarCmd->SetDefaultValue(-360.0);
  fPolarCmd->SetDefaultUnit("deg");
  fPolarCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GlxPrimaryGeneratorMessenger::~GlxPrimaryGeneratorMessenger()
{
  delete fPolarCmd;
  delete fGunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GlxPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{
  if( command == fPolarCmd ) {
      G4double angle = fPolarCmd->GetNewDoubleValue(newValue);
      if ( angle == -360.0*deg ) {
         fGlxAction->SetOptPhotonPolar();
      } else {
         fGlxAction->SetOptPhotonPolar(angle);
      }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
