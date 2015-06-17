#ifndef GlxPrimaryGeneratorMessenger_h
#define GlxPrimaryGeneratorMessenger_h

#include "G4UImessenger.hh"
#include "globals.hh"

class GlxPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GlxPrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    GlxPrimaryGeneratorMessenger(GlxPrimaryGeneratorAction* );
    virtual ~GlxPrimaryGeneratorMessenger();
 
    virtual void SetNewValue(G4UIcommand*, G4String);
 
  private:
    GlxPrimaryGeneratorAction* fGlxAction;
    G4UIdirectory*                  fGunDir;
    G4UIcmdWithADoubleAndUnit*      fPolarCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
