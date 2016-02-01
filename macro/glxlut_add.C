TClonesArray *fLutSum[48];

void glxlut_add(TString inFile = "l_b*.root", TString outFile = "lut_all.root"){
  gROOT->ProcessLine(".L ../src/GlxLutNode.cxx+");

  TTree *fTreeNew = new TTree("glxlut","Look-up table for DIRC");
  for(Int_t l=0; l<48; l++){
    fLutSum[l] = new TClonesArray("GlxLutNode");
    fTreeNew->Branch(Form("LUT_%d",l),&fLutSum[l],256000,0); 
  }

  Int_t Nnodes = 20000;
  for(Int_t l=0; l<48; l++){
    TClonesArray &fLutaSum = *fLutSum[l];
    for (Long64_t n=0; n<Nnodes; n++) {
      new((fLutaSum)[n]) GlxLutNode(-1);
    }
  }

  if(inFile.Contains("*")){
    TString tname = inFile;
    TString tname1 = inFile;
    TString tname2 = inFile;
    TString end = tname1.Remove(0,inFile.Last('*')+1);
    TString start = tname2.Remove(inFile.Last('*'));
    
    TString dirname("./");
    if(inFile.Last('/')!=-1) dirname= tname.Remove(inFile.Last('/')) + "/";

    const char *ext=".root";
    TSystemDirectory dir(dirname, dirname);
    TList *files = dir.GetListOfFiles();
    if (files) {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file=(TSystemFile*)next())) {
	fname = file->GetName();
	if (!file->IsDirectory() && fname.EndsWith(ext)) {
	  TString path = dirname+fname;
	  TString substr = path.SubString(start);
	  if( substr.Length()>0 && path.EndsWith(end)){
	    adddirs(path);
	  }
	}
      }
    }
  }else{
    std::cout<<"infile  "<<inFile  <<std::endl;
    adddirs(inFile);
  }
 
  TFile *fFileNew = TFile::Open(outFile,"RECREATE");
  fTreeNew->Fill();
  fTreeNew->Write();
  fFileNew->Write();
  std::cout<<"File  "<<outFile<<" was created." <<std::endl;
  
}


void adddirs(TString filename){
  TFile* f = new TFile(filename);
  TTree *t=(TTree *) f->Get("glxlut") ;
  TClonesArray* fLut[48];
  for(Int_t l=0; l<48; l++){
    fLut[l] = new TClonesArray("GlxLutNode");
    t->SetBranchAddress(Form("LUT_%d",l),&fLut[l]); 
  }
  t->GetEntry(0);
  std::cout<<filename<<" has "<<fLut[0]->GetEntriesFast()<< " entries" <<std::endl;
  for(Int_t l=0; l<48; l++){
    for (Int_t inode=0; inode<fLut[l]->GetEntriesFast(); inode++){
      GlxLutNode *node= (GlxLutNode*) fLut[l]->At(inode);
      for(int i=0; i< node->Entries(); i++){
	((GlxLutNode*)(fLutSum[l]->At(inode)))->AddEntry(node->GetDetectorId(), node->GetEntry(i), node->GetPathId(i), node->GetNRefl(i), node->GetTime(i), node->GetHitPos(i), node->GetDigiPos());
      }
      delete node;
    }
  }
  for(Int_t l=0; l<48; l++) fLut[l]->Clear();
    
  f->Close();
}
