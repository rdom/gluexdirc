// -----------------------------------------
// GlxPrizmSD class
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#include "GlxPrizmSD.h"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
#include "G4TransportationManager.hh"
#include <TVector3.h>

#include "GlxEvent.h"

#include "GlxRunAction.h"
#include "GlxManager.h"

GlxPrizmSD::GlxPrizmSD(
                            const G4String& name, 
                            const G4String& hitsCollectionName,
                            G4int nofCells)
  : G4VSensitiveDetector(name), fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

GlxPrizmSD::~GlxPrizmSD() 
{ 
}

void GlxPrizmSD::Initialize(G4HCofThisEvent* hce)
{ 
  // Create hits collection
  fHitsCollection = new GlxPrizmHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection );
}

G4bool GlxPrizmSD::ProcessHits(G4Step* aStep, G4TouchableHistory* hist)
{   
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();

  //if (edep==0.) return false;

  GlxPrizmHit* newHit = new GlxPrizmHit();

  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetEdep(edep);
  newHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

  // // store normal to the closest boundary
  G4Navigator* theNavigator 
    = G4TransportationManager::GetTransportationManager()
    ->GetNavigatorForTracking();

  Double_t normalId = 0;
  G4bool valid;
  G4ThreeVector theLocalNormal = theNavigator->GetLocalExitNormal(&valid);
  if (valid ){
    G4ThreeVector theGlobalNormal 
      = theNavigator->GetLocalToGlobalTransform().TransformAxis(theLocalNormal);
    normalId = theGlobalNormal.x() + 10*theGlobalNormal.y() + 100*theGlobalNormal.z();
  }
  newHit->SetNormalId(normalId);

  fHitsCollection->insert( newHit );

  //newHit->Print();

  return true;
}

void GlxPrizmSD::EndOfEvent(G4HCofThisEvent*)
{ 
  if ( verboseLevel>1 ) { 
    G4int nofHits = fHitsCollection->entries();
    G4cout << "\n-------->Prizm Hits Collection: in this event they are " << nofHits 
	   << " hits in the tracker chambers: " << G4endl;
    for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}

