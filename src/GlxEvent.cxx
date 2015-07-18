#include "GlxEvent.h"

ClassImp(GlxEvent)

// // -----   Default constructor   -------------------------------------------
GlxEvent::GlxEvent(){ 
  fDecoderId = -1;
  fId = -1;
  fType = 0;
  fTime = -1;

  fPhysList = 0;
  fAngle = 0;
  fMomentum = TVector3(0,0,0);
  fPosition = TVector3(0,0,0);
  fHitSize = 0;
  fGeometry = 0;
  fLens = -1;
  fTrigger = 0;
  fTest1 = 0;
  fTest2 = 0;
  fPrismStep = 0;
  fBeamX = 0;
  fBeamZ = 0;
  fMirrorR = 0;
  fMirrorT = 0;
  fMcpT = 0;
  fInfo="";
}

void GlxEvent::AddHit(GlxHit hit){
  fHitArray.push_back(hit);
  fHitSize++;
}

TString GlxEvent::PrintInfo(){
  TString info="Basic sim information: \n";
  info += fInfo + "\n";
  info += Form("Physics list %d \n",fPhysList);
  info += Form("Particle  id %d \n",fParticle);
  info += Form("Particle momentum %f \n", fMomentum.Mag());
  info += Form("Geometry id %d \n", fGeometry);
  info += Form("Lens  id %d \n",    fLens);
  info += Form("Focusing mirror radius %f \n", fMirrorR);
  info += Form("Focusing mirror tilt   %f \n", fMirrorT);
  info += Form("Mcp tilt  %f \n", fMcpT);
  
  return info;
}
