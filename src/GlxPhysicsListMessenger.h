#ifndef GlxPhysicsListMessenger_h
#define GlxPhysicsListMessenger_h

#include "globals.hh"
#include "G4UImessenger.hh"

class GlxPhysicsList;
class G4UIdirectory;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GlxPhysicsListMessenger: public G4UImessenger
{
  public:
    GlxPhysicsListMessenger(GlxPhysicsList* );
    virtual ~GlxPhysicsListMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:
    GlxPhysicsList*  fPhysicsList;
 
    G4UIdirectory*        fGlxDir;
    G4UIdirectory*        fPhysDir;
    G4UIcmdWithAnInteger* fVerboseCmd;
    G4UIcmdWithAnInteger* fCerenkovCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
