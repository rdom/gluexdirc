#include "GlxPixelSD.h"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TransportationManager.hh"
#include "G4Navigator.hh" 
#include "G4RunManager.hh"
#include <TVector3.h>

#include "GlxEvent.h"
#include "GlxPrizmHit.h"

#include "GlxRunAction.h"
#include "GlxManager.h"

GlxPixelSD::GlxPixelSD( const G4String& name, 
			const G4String& hitsCollectionName,
			G4int nofCells)
  : G4VSensitiveDetector(name){
  collectionName.insert(hitsCollectionName);
}

GlxPixelSD::~GlxPixelSD(){ 
}

void GlxPixelSD::Initialize(G4HCofThisEvent* hce){

 // TTree *gTree = new TTree("Glx","Prototype hits tree");
 // Event *fHit = 0;
 // gTree->Branch("event", "Event", &event, 64000, 0);

  // // Create hits collection
  // fHitsCollection 
  //   = new B4cCalorHitsCollection(SensitiveDetectorName, collectionName[0]); 

  // // Add this collection in hce
  // G4int hcID 
  //   = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  // hce->AddHitsCollection( hcID, fHitsCollection ); 

  // // Create hits
  // // fNofCells for cells + one more for total sums 
  // for (G4int i=0; i<fNofCells+1; i++ ) {
  //   fHitsCollection->insert(new B4cCalorHit());
  // }
 
  //GlxManager::Instance()->AddEvent(GlxEvent());

}

G4bool GlxPixelSD::ProcessHits(G4Step* step, G4TouchableHistory* hist){  
  if(step == 0) return false;
 
  //G4ThreeVector translation = hist->GetTranslation();
  //G4ThreeVector localpos = step->GetPreStepPoint()->GetPhysicalVolume()->GetObjectTranslation();
  G4TouchableHistory* touchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());

  // Get cell id 
  G4int layerNumber = touchable->GetReplicaNumber(0);
  //G4cout<< " PixelId = "<<layerNumber << G4endl;
  G4Track* track = step->GetTrack();
  const G4DynamicParticle* dynParticle = track->GetDynamicParticle();
  G4ParticleDefinition* particle = dynParticle->GetDefinition();  
  G4String ParticleName = particle->GetParticleName();
   
  G4ThreeVector globalpos = step->GetPostStepPoint()->GetPosition();
  G4ThreeVector localpos = touchable->GetHistory()->GetTopTransform().TransformPoint(globalpos);
  G4ThreeVector translation = touchable->GetHistory()->GetTopTransform().Inverse().TransformPoint(G4ThreeVector(0,0,0));
  G4ThreeVector inPrismpos = touchable->GetHistory()->GetTransform( 3 ).TransformPoint(globalpos);
  G4ThreeVector g4mom =track->GetVertexMomentumDirection();// track->GetMomentum();//
  G4ThreeVector g4idi = track->GetVertexMomentumDirection();
  G4ThreeVector g4pos = track->GetVertexPosition();

  TVector3 globalPos(inPrismpos.x(),inPrismpos.y(),inPrismpos.z());
  //TVector3 globalPos(globalpos.x(),globalpos.y(),globalpos.z());
  TVector3 localPos(localpos.x(),localpos.y(),localpos.z());
  TVector3 digiPos(translation.x(),translation.y(),translation.z());
  TVector3 momentum(g4mom.x(),g4mom.y(),g4mom.z());
  TVector3 initdir(g4idi.x(), g4idi.y(), g4idi.z());
  TVector3 position(g4pos.x(),g4pos.y(),g4pos.z());

  // information form prizm
  G4SDManager* fSDM = G4SDManager::GetSDMpointer();
  G4RunManager* fRM = G4RunManager::GetRunManager();
  G4int collectionID = fSDM->GetCollectionID("PrizmHitsCollection");
  const G4Event* currentEvent = fRM->GetCurrentEvent();
  G4HCofThisEvent* HCofEvent = currentEvent->GetHCofThisEvent();
  GlxPrizmHitsCollection* prizmCol = (GlxPrizmHitsCollection*)(HCofEvent->GetHC(collectionID));


	
  Double_t pathId = 0;
  Int_t refl=-1, barId(-1);
  if(GlxManager::Instance()->GetRunType()==3) {barId = GlxManager::Instance()->GetRadiator();} // if laser run
  for (G4int i=0;i<prizmCol->entries();i++){
    GlxPrizmHit* phit = (*prizmCol)[i];
    if(i==0 && GlxManager::Instance()->GetRunType()!=3){	
	  barId=phit->GetId();
	}
    if(phit->GetTrackId()==track->GetTrackID()) {
      refl++;
      pathId += phit->GetNormalId()*1000*refl;
	//	pathId = pathId*10 + phit->GetNormalId(); // in case reflections sequence is used in GlxPrizmSD.cxx
    }
  }
  
  // for the 6x17 mcp layout
  Int_t mcpid = 6*17*touchable->GetReplicaNumber(3) + touchable->GetReplicaNumber(1);
  
  GlxHit hit;
  hit.SetType(barId);
  hit.SetMcpId(mcpid);
  hit.SetPixelId(touchable->GetReplicaNumber(0));
  hit.SetGlobalPos(globalPos);
  hit.SetLocalPos(localPos);
  hit.SetDigiPos(digiPos);
  hit.SetPosition(position);
  hit.SetMomentum(momentum);
  hit.SetInitDir(initdir);
  if(GlxManager::Instance()->GetRunType()==6){
    G4ThreeVector mominend = step->GetPostStepPoint()->GetMomentum();
    TVector3 mominendv(mominend.x(),mominend.y(),mominend.z());
    hit.SetMomentum(mominendv);
  }
  hit.SetParticleId(track->GetTrackID());
  hit.SetParentParticleId(track->GetParentID());
  hit.SetNreflectionsInPrizm(refl);
  hit.SetPathInPrizm(pathId);
  hit.SetCherenkovMC(GlxManager::Instance()->GetCurrentCherenkov());

  // time since track created
  G4double time = step->GetPreStepPoint()->GetLocalTime();
  G4double resolution = 0.3; //ns
  time = G4RandGauss::shoot(time,resolution);
  hit.SetLeadTime(time);
  hit.SetTotTime(track->GetTrackLength());

  // time since event created
  // hit.SetTrailTime(0,step->GetPreStepPoint()->GetGlobalTime()*1000);

  GlxManager::Instance()->AddHit(hit);
  
  // // Get hit accounting data for this cell
  // B4cCalorHit* hit = (*fHitsCollection)[layerNumber];
  // if ( ! hit ) {
  //   G4ExceptionDescription msg;
  //   msg << "Cannot access hit " << layerNumber; 
  //   G4Exception("GlxPixelSD::ProcessHits()",
  //     "MyCode0004", FatalException, msg);
  // }         

  // // Get hit for total accounting
  // B4cCalorHit* hitTotal 
  //   = (*fHitsCollection)[fHitsCollection->entries()-1];
  
  // // Add values
  // hit->Add(edep, stepLength);
  // hitTotal->Add(edep, stepLength); 
      
  return true;
}

void GlxPixelSD::EndOfEvent(G4HCofThisEvent*)
{ 
  G4int eventNumber = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
  if(eventNumber%1==0 && GlxManager::Instance()->GetRunType()==0) std::cout<<"Event # "<<eventNumber <<std::endl;
  if(eventNumber%1000==0 && GlxManager::Instance()->GetRunType()!=0) std::cout<<"Event # "<<eventNumber <<std::endl;
  GlxManager::Instance()->Fill();
}

