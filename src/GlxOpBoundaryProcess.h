// -----------------------------------------
// GlxOpBoundaryProcess.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxOpBoundaryProcess_h
#define GlxOpBoundaryProcess_h

#include "globals.hh"
#include "G4OpBoundaryProcess.hh"

class GlxOpBoundaryProcess : public G4OpBoundaryProcess
{
public:
  GlxOpBoundaryProcess();
  ~GlxOpBoundaryProcess(){};

public:
  G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep);

private:
  int fLensId;
};


#endif /*GlxOpBoundaryProcess_h*/
