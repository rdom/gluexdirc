#include "GlxPhysicsListMessenger.h"

#include "GlxPhysicsList.h"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"

GlxPhysicsListMessenger::
  GlxPhysicsListMessenger(GlxPhysicsList* pPhys) 
  : G4UImessenger(),
    fPhysicsList(pPhys)
{
  fGlxDir = new G4UIdirectory("/Glx/");
  fGlxDir->SetGuidance("UI commands of this example");

  fPhysDir = new G4UIdirectory("/Glx/phys/");
  fPhysDir->SetGuidance("PhysicsList control");
 
  fVerboseCmd = new G4UIcmdWithAnInteger("/Glx/phys/verbose",this);
  fVerboseCmd->SetGuidance("set verbose for physics processes");
  fVerboseCmd->SetParameterName("verbose",true);
  fVerboseCmd->SetDefaultValue(1);
  fVerboseCmd->SetRange("verbose>=0");
  fVerboseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  fCerenkovCmd =
           new G4UIcmdWithAnInteger("/Glx/phys/cerenkovMaxPhotons",this);
  fCerenkovCmd->SetGuidance("set max nb of photons per step");
  fCerenkovCmd->SetParameterName("MaxNumber",false);
  fCerenkovCmd->SetRange("MaxNumber>=0");
  fCerenkovCmd->AvailableForStates(G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GlxPhysicsListMessenger::~GlxPhysicsListMessenger()
{
  delete fVerboseCmd;
  delete fCerenkovCmd;
  delete fPhysDir;
  delete fGlxDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GlxPhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                               G4String newValue)
{
  if( command == fVerboseCmd )
   {fPhysicsList->SetVerbose(fVerboseCmd->GetNewIntValue(newValue));}

  if( command == fCerenkovCmd )
   {fPhysicsList->
              SetNbOfPhotonsCerenkov(fCerenkovCmd->GetNewIntValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
