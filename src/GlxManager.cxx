#include "TInterpreter.h"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"

#include "GlxManager.h"
#include "GlxHit.h"
#include "GlxLutNode.h"

GlxManager * GlxManager::fInstance= NULL;

GlxManager::GlxManager(G4String outfile, G4int runtype)
{
  TString filename = outfile;
  fOutName = filename; 
  fOutName = fOutName.Remove(fOutName.Last('.'));
  fRunType = runtype;
  fRootFile = new TFile(filename,"RECREATE");

  if(fRunType==0 || fRunType==6){
    fTree = new TTree("data","Prototype hits tree");
    fEvent = new GlxEvent();
    fTree->Branch("GlxEvent", "GlxEvent", &fEvent, 64000, 2);
  }

  if(fRunType==1 || fRunType==5){
    fLut = new TClonesArray("GlxLutNode");
    fTree = new TTree("prtlut","Look-up table for the geometrical reconstruction.");
    fTree->Branch("LUT",&fLut,256000,2); 
    Int_t Nnodes = 20000;
    
    TClonesArray &fLuta = *fLut; 
    for (Long64_t n=0; n<Nnodes; n++) {
      new((fLuta)[n]) GlxLutNode(n);
    }    
  }

  if(fRunType==2){
    fTree = new TTree("recodata","Reconstructed info for the prototype");
    fTrackInfoArray = new TClonesArray("GlxTrackInfo");
    fTree->Branch("GlxTrackInfo",&fTrackInfoArray,256000,2); 
  }
  
  // fHist = new TH1F("id", "name", 100, 0., 100);

  fPhysList = 0;
  fParticle = 0;
  fMomentum = TVector3(0,0,0);
  fGeometry = 0;
  fAngle = 0;
  fRadiatorL=0;
  fRadiatorW=0;
  fRadiatorH=0;
  fShift = 150;
  fTest1 = 0;
  fTest2 = 0;
  fLens = 0;
  fMcpLayout = 2014;
  fBeamDimension = 0;

  fPrismStep=0;
  fBeamX=0;
  fBeamZ=-1;
  fMirrorR = 1200;
  fMirrorT = 20;
  fMcpT = 42.13;
  fInfo="";
  
  std::cout<<"GlxManager has been successfully initialized. " <<std::endl;
}

GlxManager* GlxManager::Instance(G4String outfile, G4int runtype){
  if ( !fInstance){
    std::cout<<"Info in (GlxManager::Instance): Making a new instance. "<<std::endl;
    fInstance = new GlxManager(outfile, runtype);
  }
  return fInstance;
}

void GlxManager::AddEvent(GlxEvent event){
  if(fRunType==0 || fRunType==6){
    fEvent = new GlxEvent(event);
    fEvent->SetPhysList(fPhysList);
    fEvent->SetAngle((180*deg-fAngle)/deg);
    //fEvent->SetRadiatorL(fRadiatorL);
    //fEvent->SetRadiatorW(fRadiatorW);
    //fEvent->SetRadiatorH(fRadiatorH);
    fEvent->SetParticle(fParticle);
    fEvent->SetMomentum(fMomentum);
    fEvent->SetGeometry(fGeometry);
    fEvent->SetLens(fLens);
    fEvent->SetTest1(fTest1);
    fEvent->SetTest2(fTest2);
    fEvent->SetPrismStep(fPrismStep);
    fEvent->SetBeamX(fBeamX);
    fEvent->SetBeamZ(fBeamZ);
    fEvent->SetMirrorR(fMirrorR);
    fEvent->SetMirrorT(fMirrorT);
    fEvent->SetMcpT(fMcpT);
    fEvent->SetInfo(fInfo);
  }
}


void GlxManager::AddHit(GlxHit hit){
  if(fRunType==0 || fRunType==6){
    if ( fEvent ){
      fEvent->AddHit(hit);
    }else{
      std::cout<<"Event does not exist. Create it first. "<<std::endl;
    }
  }
  if(fRunType==1 || fRunType==5){
    int id = 100*hit.GetMcpId() + hit.GetPixelId();
    ((GlxLutNode*)(fLut->At(id)))->
      AddEntry(id, fMomentum, hit.GetPathInPrizm(),
	       hit.GetNreflectionsInPrizm(),
	       hit.GetLeadTime(),hit.GetGlobalPos(),hit.GetDigiPos());
  } 
}

void GlxManager::AddTrackInfo(GlxTrackInfo trackinfo){
  new ((*fTrackInfoArray)[fTrackInfoArray->GetEntriesFast()]) GlxTrackInfo(trackinfo);
}



void GlxManager::Fill(){
  if(fRunType==0 || fRunType==6){
    fTree->Fill();
    fEvent->Clear();
  }
  if(fRunType==2){
    fTree->Fill();
    fTrackInfoArray->Clear();
  }
}

void GlxManager::FillLut(){
  if(fRunType==1 || fRunType==5) fTree->Fill();
}
