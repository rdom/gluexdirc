// -----------------------------------------
// GlxSteppingAction.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxSteppingAction_h
#define GlxSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

/// Stepping action class 

class GlxSteppingAction : public G4UserSteppingAction{
public:
  GlxSteppingAction();
  virtual ~GlxSteppingAction();

  // method from the base class
  virtual void UserSteppingAction(const G4Step*);

private:
  G4int fScintillationCounter;
  G4int fCerenkovCounter;
  G4int fEventNumber;
};

#endif
