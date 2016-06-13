#define glx__sim
//#include "../src/GlxHit.h"
//#include "../src/GlxEvent.h"
//#include "../src/GlxLutNode.h"
#include "glxtools.C"

Int_t    fNEntries(0);

void drawLaser(TString infile="laser_3fibers1M_sig15_inc4.root"){
//void drawLaser(TString infile="../build/laser_monbars.root"){
  if(infile=="") return;
	const Int_t Nfibers = 3;

  TFile* output = new TFile("lasertime.root", "RECREATE");	

  const int nmcp = 70, npix = 64;
  TH1F * hPTime[nmcp][npix];
  TH1F * hTime = new TH1F("time",";time, [ns];entries, [#]",500,0,150);
  for(Int_t m=0; m<nmcp; m++){
    for(Int_t p=0; p<npix; p++){
      hPTime[m][p]  = new TH1F(Form("hPTime_%d",m*100+p),Form("mcp %d, pixel %d",m, p),400,0,25);//200); //800,1800
      axisTime800x500(hPTime[m][p]);
	 // gStyle->SetOptTitle(0);
      hPTime[m][p]->SetStats(1);
      hPTime[m][p]->SetLineColor(1);
		hPTime[m][p]->SetLineWidth(3);
    }
  }

  gSystem->Load("../build/libGlxDirc.so");
  TFile* f = new TFile(infile);
  TTree* t = (TTree*)f->Get("glxlut");
  TClonesArray *fLut[Nfibers];
  for(int ifib=0;ifib<Nfibers;ifib++){
  fLut[ifib] = new TClonesArray("GlxLutNode");
  //t->SetBranchAddress("LUT_0", &fLut);
  t->SetBranchAddress(Form("LUT_%d",ifib),&fLut[ifib]);
  }
  t->GetEntry(0);

  Double_t evtime;	
  GlxLutNode* node;
  Int_t mcpid, pixid;
  for(int i=0; i<20000; i++){
	  for(int jfib=0;jfib<Nfibers;jfib++){
		node = (GlxLutNode*) fLut[jfib]->At(i);
		Int_t size = node->Entries();
		if(size > 0){
		// cout<<"node "<<node->GetDetectorId()<<" has "<<size<<" entries, mcp = "<<i/100<<", pix = "<<i%100<<endl;
			mcpid = i/100;
			pixid = i%100-1;
  			for(int u=0; u<size; u++){
				evtime = node->GetTime(u);
				hPTime[mcpid][pixid]->Fill(evtime);
			}
		}
	 } 
  }
  output->Write();

	gStyle->SetOptStat("e");
  gStyle->SetOptFit(0001);
	SetRootPalette(1);

	//TCanvas* big = new TCanvas("big","big", 1400, 500);
	//big->Divide(14,5);
	TH2F * hPMToc[nmcp];
	for(int ipad=0;ipad<nmcp;ipad++){
	  hPMToc[ipad]  = new TH2F(Form("hPMToc_%d",ipad),Form("PMT %d",ipad),8,0,8,8,0,8); 
	  hPMToc[ipad]->SetStats(0);
	  //big->cd(ipad);
	  //gPad->SetBottomMargin(0.001);
	  //gPad->SetLeftMargin(0.001);
	  //gPad->SetRightMargin(0.001);
	  //gPad->SetTopMargin(0.001);
	  //gPad->SetFrameBorderSize(0.01);
	}	
	
  TCanvas* c = new TCanvas("c","c",800,600);
  TF1* fit = new TF1("fit","gaus(0)",0.,10.);
	
	TPaveText* tit; 
	gStyle->SetOptFit(0001);
	//gStyle->SetOptStat(1111111);
	Double_t y1,y2,prob;
  for(Int_t m=0; m<nmcp; m++){
    for(Int_t p=0; p<npix; p++){
	  hPMToc[m]->SetBinContent(p%8+1, 8-p/8, hPTime[m][p]->GetEntries());
	 // cout<<"bin "<<p+1<<" filled with "<< hPTime[m][p]->GetEntries()<<endl;
	  if(hPTime[m][p]->GetEntries() > 10){
	    c->Update();
		fit->SetParameter(1,2.5);
		fit->SetParameter(2,0.3);
		fit->SetParameter(0,200.); 
		//hPTime[m][p]->Fit("fit","R","",0.,7.);	  
	    //hPTime[m][p]->Fit("fit","M","",0.,7.);
		//hPTime[m][p]->Fit("fit","MW","",0.,7.);  
		//cout<<"maximum - "<<hPTime[m][p]->GetMaximum()<<endl;
		y1 = hPTime[m][p]->GetMaximum()/2.;
		y2 = hPTime[m][p]->GetMaximum()/3.*2.;
		prob = hPTime[m][p]->GetEntries()/1000000.;
		tit = new TPaveText(17.0,y1,25.,y2,"NB");
		tit->SetFillColor(0);
		tit->AddText(Form("prob = %f", prob));
		hPTime[m][p]->Draw();
		tit->Draw();
		c->Update();
        c->SaveAs(Form("pixels_3fibers_sig15_inc4/%d.png", m*100+p));
	  }
    }
  }
  cout<<"loop is over"<<endl;
  TCanvas* coc = new TCanvas("coc","coc", 500, 500);
	coc->SetRightMargin(0.1);
	coc->SetTopMargin(0.1);
	coc->SetLeftMargin(0.05);
	coc->SetBottomMargin(0.05);
  coc->cd();
  for(Int_t iii=0; iii<nmcp; iii++){
	  //cout<<"iii = "<<iii<<endl;
	  coc->Update();
	  hPMToc[iii]->SetMaximum(900.);
	  hPMToc[iii]->SetMinimum(0.);
	  hPMToc[iii]->Draw("colz");
	  coc->SaveAs(Form("pmt_3fibers_sig15_inc4/pmt%d.png", iii));
  }
  
}

