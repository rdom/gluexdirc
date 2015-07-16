#include "GlxOpBoundaryProcess.h"

#include "GlxManager.h"

GlxOpBoundaryProcess::GlxOpBoundaryProcess()
  : G4OpBoundaryProcess()
{
  fLensId = GlxManager::Instance()->GetLens();
}

G4VParticleChange* GlxOpBoundaryProcess::PostStepDoIt(const G4Track& aTrack, const G4Step& aStep)
{  
  G4StepPoint* pPreStepPoint  = aStep.GetPreStepPoint();
  G4StepPoint* pPostStepPoint = aStep.GetPostStepPoint();
  G4VParticleChange* particleChange = G4OpBoundaryProcess::PostStepDoIt(aTrack, aStep); 
  
  // int parentId = aTrack.GetParentID();
  // std::cout<<"parentId   "<<parentId <<std::endl;
  // if(parentId==1) particleChange->ProposeTrackStatus(fStopAndKill);

  // ideal focusing
  if(GlxManager::Instance()->GetLens() == 10){
    G4String ParticleName = aTrack.GetDynamicParticle()->GetParticleDefinition()->GetParticleName();  
    if (ParticleName == "opticalphoton"){
      double endofbar = 1250/2.;
      G4ThreeVector theGlobalPoint1 = pPostStepPoint->GetPosition();
      G4TouchableHistory* touchable = (G4TouchableHistory*)(pPostStepPoint->GetTouchable());
      G4ThreeVector lpoint =  touchable->GetHistory()->GetTransform( 1 ).TransformPoint(theGlobalPoint1);
     
      if(lpoint.getZ() < endofbar+0.0001 && lpoint.getZ() > endofbar-0.0001){
	G4ThreeVector ww  = pPreStepPoint->GetTouchableHandle()->GetHistory()->
	  GetTopTransform().Inverse().TransformPoint(G4ThreeVector(0,0,endofbar));
	if(aStep.GetPreStepPoint()->GetPhysicalVolume()->GetName()!="wBar") 
	  particleChange->ProposeTrackStatus(fStopAndKill);
	else{
	  G4Navigator* theNavigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
	  theNavigator->LocateGlobalPointWithinVolume(ww);
	  aParticleChange.ProposePosition(ww.getX(), ww.getY(),ww.getZ());
	}
	return G4VDiscreteProcess::PostStepDoIt(aTrack, aStep);
      }
    }
  }

  if(GlxManager::Instance()->GetRunType() == 1 && pPostStepPoint->GetPosition().z()<pPreStepPoint->GetPosition().z()){
    particleChange->ProposeTrackStatus(fStopAndKill);
  }

  if(GlxManager::Instance()->GetRunType() == 5 &&  aStep.GetPreStepPoint()->GetPhysicalVolume()->GetName()=="wDirc" && aStep.GetPostStepPoint()->GetPhysicalVolume()->GetName()=="wPrizm" && GetStatus() == FresnelRefraction){
    particleChange->ProposeTrackStatus(fStopAndKill);
  }
  
  // kill photons outside bar and prizm
  if(GetStatus() == FresnelRefraction 
     && aStep.GetPostStepPoint()->GetPhysicalVolume()->GetName()=="wDirc"){
    particleChange->ProposeTrackStatus(fStopAndKill);
  }
  
  // kill photons outside tankbox
  if(aStep.GetPreStepPoint()->GetPhysicalVolume()->GetName()=="wTankBox" 
     &&  aStep.GetPostStepPoint()->GetPhysicalVolume()->GetName()=="wDirc"){
    particleChange->ProposeTrackStatus(fStopAndKill);
  }

  // kill photons exiting FD
  if(aStep.GetPreStepPoint()->GetPhysicalVolume()->GetName()=="wFdp" 
     &&  aStep.GetPostStepPoint()->GetPhysicalVolume()->GetName()=="wTankBox"){
    particleChange->ProposeTrackStatus(fStopAndKill);
  }

  return particleChange;

}
