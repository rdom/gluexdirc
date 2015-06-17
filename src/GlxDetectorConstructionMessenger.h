// -----------------------------------------
// GlxDetectorConstruction.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxDetectorConstructionMessenger_h
#define GlxDetectorConstructionMessenger_h

#include "G4UImessenger.hh"
#include "globals.hh"

#include "GlxDetectorConstruction.h"

class GlxDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;


class GlxDetectorConstructionMessenger: public G4UImessenger
{ 
public:
  GlxDetectorConstructionMessenger(GlxDetectorConstruction*);
  virtual ~GlxDetectorConstructionMessenger();
 
  virtual void SetNewValue(G4UIcommand*, G4String);
 
private:
  GlxDetectorConstruction*        fGlxGeom;
  G4UIdirectory*                  fGeomDir;
  G4UIcmdWithADoubleAndUnit*      fAngleCmd;
  G4UIcmdWithAnInteger*           fLensIdCmd;  
  G4UIcmdWithAnInteger*           fDetEffType;
};

#endif
