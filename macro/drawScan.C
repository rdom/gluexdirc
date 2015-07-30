#define glx__sim
#include "../src/GlxHit.h"
#include "../src/GlxEvent.h"
#include "glxtools.C"

void drawScan(TString infile="hits_w10.root"){
  if(infile=="") return;
  fSavePath = "data/ang_mir";
  GlxInit(infile,1); //digi
  TH1F *hTime = new TH1F("hTime","hTime",500,0,200);
  TH2F *hHits = new TH2F("hHits",";x [mm];y [mm]",1000,-1100,1100,500,-150,150);
  hHits->SetStats(0);

  Int_t ntotal(0);
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
      ntotal++;
      
      //fhDigi[mcpid]->Fill(7-pixid/8, pixid%8);
      //fhDigi[mcpid]->Fill(pixid%8, pixid/8); // for beam data
    }
  }
  //  drawDigi("m,p,v\n",2,-2,-2); //for beam data
  //  drawDigi("m,p,v\n",4);
  //  cDigi->SetName(Form("sc_%d_%d",fAngle,fMomentum/1000));

  Int_t rid = (glx_radius-300)/40.;
  Int_t tid = (glx_tilt-5)/1.;
  Int_t thetaid = (glx_hitx+2490)/((2390+2490)/40.);
  Int_t phiid = (glx_hity-160)/((1000-160)/20.);
  
  //TString name =  Form("%d_%d",rid,tid);
  //TString name =  Form("%d_%d",thetaid,phiid);
  TString name =  Form("%d_%d",thetaid,rid);

  hHits->SetTitle(Form("#theta=%2.2f, #varphi=%2.2f, R=%2.0f, T=%2.0f#circ, N=%d",glx_theta,glx_phi, glx_radius,glx_tilt,ntotal));
  
  // canvasAdd("time_"+name);  
  // hTime->Draw();
  canvasAdd("hits_"+name,800,400);
  hHits->Draw("colz");
  canvasSave(1,0);
  
}

