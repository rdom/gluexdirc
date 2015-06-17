// -----------------------------------------
// GlxTriggerSD.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxTriggerSD_h
#define GlxTriggerSD_h 1

#include <vector>
#include "G4VSensitiveDetector.hh"

#include "GlxEvent.h"

class G4Step;
class G4HCofThisEvent;

class GlxTriggerSD : public G4VSensitiveDetector
{
public:
  GlxTriggerSD(const G4String& name, 
	     const G4String& hitsCollectionName, 
	     G4int nofCells);
  virtual ~GlxTriggerSD();
  
  // methods from base class
  virtual void   Initialize(G4HCofThisEvent* hitCollection);
  virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
  virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

};


#endif
