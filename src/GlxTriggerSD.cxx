#include "GlxTriggerSD.h"
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

GlxTriggerSD::GlxTriggerSD(
                            const G4String& name, 
                            const G4String& hitsCollectionName,
                            G4int nofCells)
  : G4VSensitiveDetector(name)
{
  collectionName.insert(hitsCollectionName);
}

GlxTriggerSD::~GlxTriggerSD() 
{ 
}

void GlxTriggerSD::Initialize(G4HCofThisEvent* hce)
{ 

}

G4bool GlxTriggerSD::ProcessHits(G4Step* aStep, G4TouchableHistory* hist)
{   
  // energy deposit
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4ThreeVector globalpos = aStep->GetPostStepPoint()->GetPosition();
  G4Track* track = aStep->GetTrack(); 
  G4ThreeVector g4pos = track->GetVertexPosition();
  
  GlxManager::Instance()->Event()->SetTrigger(1);
  //GlxManager::Instance()->Event()->SetPosition(TVector3(globalpos.x(),globalpos.y(),globalpos.z()));

  return true;
}

void GlxTriggerSD::EndOfEvent(G4HCofThisEvent*)
{ 
 
}

