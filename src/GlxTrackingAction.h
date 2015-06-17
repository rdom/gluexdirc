#include "G4UserTrackingAction.hh"

#include "TGraph.h"
#include "TRandom.h"


class G4Track;
class GlxTrackingAction : public G4UserTrackingAction
{
public:
  GlxTrackingAction();
  void PreUserTrackingAction(const G4Track* aTrack);
private:
  TRandom* fRand;
  TGraph* fDetEff;
};
