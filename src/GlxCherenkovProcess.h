// -----------------------------------------
// GlxCherenkovProcess.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxCherenkovProcess_h
#define GlxCherenkovProcess_h

#include "globals.hh"
#include "G4Cerenkov.hh"

class GlxCherenkovProcess : public G4Cerenkov
{
public:
  GlxCherenkovProcess(const G4String& processName, G4ProcessType type = fElectromagnetic);
  ~GlxCherenkovProcess(){};

public:
  G4VParticleChange* PostStepDoIt(const G4Track& aTrack, const G4Step& aStep);

private:
  int fLensId;

  G4double GetAverageNumberOfPhotons(const G4double charge,
				     const G4double beta,
				     const G4Material *aMaterial,
				     G4MaterialPropertyVector* Rindex) const;
};


#endif /*GlxCherenkovProcess_h*/
