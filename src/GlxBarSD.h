// -----------------------------------------
// GlxBarSD.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxBarSD_h
#define GlxBarSD_h 1

#include <vector>
#include "G4VSensitiveDetector.hh"

#include "GlxBarHit.h"

class G4Step;
class G4HCofThisEvent;

class GlxBarSD : public G4VSensitiveDetector
{
public:
  GlxBarSD(const G4String& name, 
	     const G4String& hitsCollectionName, 
	     G4int nofCells);
  virtual ~GlxBarSD();
  
  // methods from base class
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

private: 
  GlxBarHitsCollection* fHitsCollection;
};

#endif
