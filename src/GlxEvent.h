// -----------------------------------------
// GlxEvent.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxEvent_h
#define GlxEvent_h 1

#include "TObject.h"
#include "TString.h"

#include <vector>
#include "GlxHit.h"

class GlxEvent: public TObject  {

public:

  GlxEvent(); 	//the default constructor
  ~GlxEvent(){}; 

  void AddHit(GlxHit hit);
  GlxHit GetHit(Int_t ind) { return fHitArray[ind]; }
  TString PrintInfo();

  // Accessors 
  Int_t GetDecoderId() const { return fDecoderId; }
  Int_t GetId() const { return fId; }
  Int_t GetType() const { return fType; }
  Long_t GetTime() const { return fTime; }

  Double_t GetAngle()      const { return fAngle; }
  Int_t GetPhysList()      const { return fPhysList; }
  Int_t GetPdg()      const { return fParticle; }
  TVector3 GetMomentum()   const { return fMomentum; }
  TVector3 GetPosition()   const { return fPosition; }
  Int_t GetHitSize()       const { return fHitSize; }
  Int_t GetGeometry()      const { return fGeometry; }
  Int_t GetLens()          const { return fLens; }
  Double_t GetGap()		   const { return fGap;}
  Double_t GetMirrorGap()  const { return fMirrorGap;}
  Int_t GetTrigger()       const { return fTrigger; } 
  Double_t GetTest1()      const { return fTest1; }
  Double_t GetTest2()      const { return fTest2; }
  Double_t GetPrismStep(){ return fPrismStep; }
  Double_t GetBeamX(){ return fBeamX; }
  Double_t GetBeamZ(){ return fBeamZ; }
  Double_t GetMirrorR(){ return fMirrorR; }
  Double_t GetMirrorT(){ return fMirrorT; }
  Double_t GetMcpT(){ return fMcpT; }
  TString  GetInfo() { return fInfo; }
  
  // Mutators
  void SetDecoderId(Int_t val)  { fDecoderId=val; }
  void SetId(Int_t val)        { fId=val; }
  void SetType(Int_t val)        { fType=val; }
  void SetTime(Long_t val)      { fTime=val; }

  void SetPhysList(Int_t val) { fPhysList = val; }
  void SetAngle(Double_t val) { fAngle = val; }
  void SetPdg(Int_t val) { fParticle = val; }
  void SetMomentum(TVector3 val) { fMomentum = val; }
  void SetPosition(TVector3 val) { fPosition = val; }
  void SetGeometry(Int_t val) { fGeometry = val; }
  void SetLens(Int_t val) { fLens = val; }
  void SetGap(Double_t val) { fGap = val; }
  void SetMirrorGap(Double_t val) { fMirrorGap = val; }
  void SetTrigger(Int_t val) { fTrigger = val; }
  void SetTest1(Double_t val) { fTest1 = val; }
  void SetTest2(Double_t val) { fTest2 = val; }
  void SetPrismStep(Double_t val){ fPrismStep = val; }
  void SetBeamX(Double_t val){ fBeamX = val; }
  void SetBeamZ(Double_t val){ fBeamZ = val; }
  void SetMirrorR(Double_t val){ fMirrorR = val; }
  void SetMirrorT(Double_t val){ fMirrorT = val; }
  void SetMcpT(Double_t val){ fMcpT = val; }
  void SetInfo(TString val){ fInfo = val; }

private: 
  Int_t fDecoderId;
  Int_t fId;
  Int_t fType;
  Long_t fTime;


  Int_t fHitSize;
  std::vector<GlxHit> fHitArray;

  Int_t fPhysList;
  Int_t fParticle;
  Double_t fAngle;
  TVector3 fMomentum;
  TVector3 fPosition;
  Int_t fGeometry;
  Int_t fLens;
  Double_t fGap;
  Double_t fMirrorGap;
  Int_t fTrigger; 
  Double_t fTest1;
  Double_t fTest2;
  Double_t fPrismStep;
  Double_t fBeamX;
  Double_t fBeamZ;
  Double_t fMirrorR;
  Double_t fMirrorT;
  Double_t fMcpT;
  TString  fInfo;
  
  ClassDef(GlxEvent, 4);
};
#endif
