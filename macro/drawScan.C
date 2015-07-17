#define glx__sim
#include "../src/GlxHit.h"
#include "../src/GlxEvent.h"
#include "glxtools.C"

void drawScan(TString infile="hits.root"){
  fSavePath = "scan3";
  GlxInit(infile,1); //digi
  TH1F *hTime = new TH1F("hTime","hTime",500,0,200);
  TH2F *hHits = new TH2F("hHits",";x [mm];y [mm]",500,-1100,1100,100,-150,150);
  hHits->SetStats(0);
   
  GlxHit fHit;
  for (Int_t ievent=0; ievent<fCh->GetEntries(); ievent++){
    GlxNextEvent(ievent,1000);
    for(Int_t h=0; h<fEvent->GetHitSize(); h++){
      fHit = fEvent->GetHit(h);
      Int_t mcpid = fHit.GetMcpId();
      Int_t pixid = fHit.GetPixelId()-1;
      TVector3 gpos = fHit.GetGlobalPos();
      
      Double_t time = fHit.GetLeadTime();
      hTime->Fill(time);
      hHits->Fill(gpos.Y(),gpos.X());
      //std::cout<<"X "<<gpos.X()<<" Y "<<gpos.Y()<< "  Z  "<<   gpos.Z() <<std::endl;

	
      //fhDigi[mcpid]->Fill(7-pixid/8, pixid%8);
      //fhDigi[mcpid]->Fill(pixid%8, pixid/8); // for beam data
    }
  }
  //  drawDigi("m,p,v\n",2,-2,-2); //for beam data
  //  drawDigi("m,p,v\n",4);
  //  cDigi->SetName(Form("sc_%d_%d",fAngle,fMomentum/1000));

  
  canvasAdd("hTime");  
  hTime->Draw();
  canvasAdd("hHits",800,400);
  hHits->Draw("colz");
  canvasSave(1,0);
  
}

