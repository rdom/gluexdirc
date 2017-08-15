#define glx__sim

#include "../src/GlxHit.h"
#include "../src/GlxEvent.h"
#include "glxtools.C"

void drawHP(TString infile="../build/hits.root"){
  if(!glx_init(infile,1,"data/drawHP")) return;

  GlxHit hit;
  for (Int_t ievent=0; ievent<glx_ch->GetEntries(); ievent++){
    glx_nextEvent(ievent,10);
    
    for(Int_t h=0; h<glx_event->GetHitSize(); h++){
      hit = glx_event->GetHit(h);
      Int_t pmt = hit.GetPmtId(); //-102
	
      Int_t pix = hit.GetPixelId()-1;
      TVector3 gpos = hit.GetPosition();
      Double_t time = hit.GetLeadTime();
      if(pmt<102 && pmt>=0) glx_hdigi[pmt]->Fill(7-pix%8, 7-pix/8);
    }
  }
  
  std::cout<<"glx_theta "<< glx_theta << " "<<glx_phi<<std::endl;
  int theta((180-glx_theta)+0.5);
  int phi((180+glx_phi)+0.5);

  std::cout<<"theta "<<theta<< "  "<<phi<<std::endl;
  
  
  glx_drawDigi("m,p,v\n",0);
  glx_cdigi->SetName(Form("cdigi_%d_%d",theta,phi/5));
  
  glx_canvasAdd(glx_cdigi);
  glx_canvasSave(1,0);
}
