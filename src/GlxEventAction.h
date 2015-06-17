// -----------------------------------------
// GlxEventAction.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxEventAction_h
#define GlxEventAction_h 1

#include "G4UserEventAction.hh"

#include "TGraph.h"
#include "TRandom.h"

class G4Track;
class GlxEventAction : public G4UserEventAction
{
public:
  GlxEventAction() {;}
  virtual ~GlxEventAction() {;}
  // void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);
protected:
  G4EventManager* fpEventManager;
};
#endif
