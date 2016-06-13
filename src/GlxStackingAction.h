// -----------------------------------------
// GlxStackingAction class
//
// Author  : R.Dzhygadlo at gsi.de
// --------------------------------------

#ifndef GlxStackingAction_H
#define GlxStackingAction_H 1

#include "globals.hh"
#include "G4UserStackingAction.hh"

#include "TGraph.h"
#include "TRandom.h"
#include "TVector3.h"

class GlxStackingAction : public G4UserStackingAction
{
  public:
    GlxStackingAction();
    virtual ~GlxStackingAction();

  public:
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();

  private:
    G4int fScintillationCounter;
    G4int fCerenkovCounter; 
    TRandom* fRand;
    TGraph* fDetEff;
	Double_t FindOutPoint(Double_t, Double_t, Double_t, Double_t *, Bool_t);
		void NumberOfBounces(TVector3, TVector3, Int_t*, Int_t*, Double_t*, Double_t*);
};

#endif
