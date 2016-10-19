#define glx__sim
#include "../src/GlxHit.h"
#include "../src/GlxEvent.h"
#include "glxtools.C"

//void drawScan(TString infile="../../../data/pions_SLAC_t4_phi40_x0_lam.root"){
//void drawScan(TString infile="../build/hits.root"){
void drawScan(TString infile="pions_SLAC_traj_small.root"){
  if(infile=="") return;
  fSavePath = "";
  GlxInit(infile,1); //digi
  TH1F *hTime = new TH1F("hTime","hTime",500,0,200);
  TH2F *hHits = new TH2F("hHits",";x [mm];y [mm]",500,-1500,1500.,67,-150,252.);
  TH2F *hTraj = new TH2F("hTraj",";x [mm]; y[mm]", 500,-1500,1500.,67,-150,252.);
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
      Double_t gtra = fHit.GetPathInPrizm();
      Double_t time = fHit.GetLeadTime();
		
      hTime->Fill(time);
      hHits->Fill(-gpos.Y(),gpos.X()+100.); //-577.5
	/*	if(gpos.Y() > 556. && gpos.Y() < 573. && gpos.X() > 43. && gpos.X() < 52.){
		cout <<"gtra = "<<gtra<<endl;
		}
		if(gtra < 100000){
	  for(Int_t tt=0; tt<gtra;tt++){
	    hTraj->Fill(gpos.Y(),gpos.X());
	  }
		}*/
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

  //hHits->SetTitle(Form("#theta=%2.2f, #varphi=%2.2f, R=%2.0f, T=%2.0f#circ, N=%d",glx_theta,glx_phi, glx_radius,glx_tilt,ntotal));
hHits->SetTitle(Form("#theta=%2.2f, #varphi=%2.2f, N=%d",glx_theta,glx_phi,ntotal));

	SetRootPalette(6);
	/*canvasAdd("traj_"+name);
	TH2F* hRatio = (TH2F*)hTraj->Clone();
	hRatio->Divide(hHits);
	hRatio->Draw("col2z");*/
   canvasAdd("time_"+name);  
   hTime->Draw();
  canvasAdd("hits_"+name,800,400);
  hHits->Draw("colz");
  canvasSave(1,0);

}

