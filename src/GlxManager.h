// -----------------------------------------
// GlxManager.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxManager_h
#define GlxManager_h

#include "globals.hh"

#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1F.h>
#include "TClonesArray.h"

#include "GlxEvent.h"
#include "GlxHit.h"
#include "GlxTrackInfo.h"

class GlxManager
{
  static GlxManager* fInstance;
  TFile *fRootFile;
  TTree *fTree;
  GlxEvent *fEvent;
  GlxTrackInfo *fTrackInfo;
  GlxHit *fHit;
  TH1F *fHist;

public:
  GlxManager(G4String outfile, G4int runtype);
  ~GlxManager(){};
  static GlxManager* Instance(G4String outfile="hits.root", G4int runtype=0);
  void Save()             { fRootFile->Write(); }
  void Fill();
  void FillLut();
  void AddEvent(GlxEvent event);
  void AddHit(GlxHit hit);
  void AddTrackInfo(GlxTrackInfo trackinfo);
  GlxEvent* Event(){ return fEvent; }
  
  // Mutators
  void SetRunType(int val){ fRunType = val; }
  void SetPhysList(int val){ fPhysList = val; }
  void SetGeometry(int val){ fGeometry = val; }
  void SetBeamDimension(int val){ fBeamDimension = val; }
  //void SetRadiator(int val){ fRadiator = val; }
  void SetLens(int val){ fLens = val; }
  void SetGap(double val){ fGap = val; }
  void SetMcpLayout(int val){ fMcpLayout = val; }
  void SetAngle(double val){ fAngle = val; }
  void SetRadiatorL(double val){ fRadiatorL = val; }
  void SetRadiatorW(double val){ fRadiatorW = val; }
  void SetRadiatorH(double val){ fRadiatorH = val; }
  void SetParticle(int val){ fParticle = val; }
  void SetMomentum(TVector3 val){ fMomentum = val; if(fRunType==0) fEvent->SetMomentum(fMomentum);}
  void SetCurrentCherenkov(double val){ fCurrentCherenkov = val; }
  void SetShift(double val){ fShift = val; }
  void SetTest1(double val){ fTest1 = val; }
  void SetTest2(double val){ fTest2 = val; }
  void SetPrismStep(double val){ fPrismStep = val; }
  void SetBeamX(double val){ fBeamX = val; }
  void SetBeamZ(double val){ fBeamZ = val; }
  void SetMirrorR(double val){ fMirrorR = val; }
  void SetMirrorT(double val){ fMirrorT = val; }
  void SetMcpT(double val){ fMcpT = val; }
  void SetInfo(TString val){ fInfo = val; }
  void AddInfo(TString val){ fInfo += val + "\n"; }

  // Accessors
  int GetRunType(){ return fRunType; }
  int GetPhysList(){ return fPhysList; }
  int GetGeometry(){ return fGeometry; }
  int GetBeamDimension(){ return fBeamDimension; }
  //int GetRadiator(){ return fRadiator; }
  int GetLens(){ return fLens; }
  double GetGap(){ return fGap; }
  int GetMcpLayout(){ return fMcpLayout; }
  double GetAngle(){ return fAngle; }
  double GetRadiatorL(){ return fRadiatorL; }
  double GetRadiatorW(){ return fRadiatorW; }
  double GetRadiatorH(){ return fRadiatorH; }
  int GetParticle(){ return fParticle; }
  TVector3 GetMomentum(){ return fMomentum; }
  double GetCurrentCherenkov(){ return fCurrentCherenkov; }
  double GetShift(){ return fShift; }
  double GetTest1(){ return fTest1; }
  double GetTest2(){ return fTest2; }
  double GetPrismStep(){ return fPrismStep; }
  double GetBeamX(){ return fBeamX; }
  double GetBeamZ(){ return fBeamZ; }
  double GetMirrorR(){ return fMirrorR; }
  double GetMirrorT(){ return fMirrorT; }
  double GetMcpT(){ return fMcpT; }
  
  TString GetOutName(){return fOutName;}
  TString GetInfo() { return fInfo; }

private: 
  int fRunType;
  int fPhysList;
  int fGeometry;
  //int fRadiator;
  int fLens;
  double fGap;
  int fMcpLayout;
  double fAngle;
  double fRadiatorL;
  double fRadiatorW;
  double fRadiatorH;
  int fParticle;
  int fBeamDimension;
  TVector3 fMomentum;
  TClonesArray *fLut[48];
  TClonesArray *fTrackInfoArray;
  double fCurrentCherenkov;
  double fShift;
  double fTest1;
  double fTest2;
  double fPrismStep;
  double fBeamX;
  double fBeamZ;
  double fMirrorR;
  double fMirrorT;
  double fMcpT;
  
  TString fOutName;
  TString fInfo;
};

#endif
