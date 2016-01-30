// -----------------------------------------
// GlxLutNode.h
//
// Created on: 09.07.2013
// Author: R.Dzhygadlo at gsi.de
// -----------------------------------------
// Container for look-up table

#ifndef GlxLutNode_h
#define GlxLutNode_h

#include "TObject.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include <vector>
#include <iostream>

class GlxLutNode : public TObject {

public:    
  
  // Default constructor
  GlxLutNode ();

  // Standard constructors
  GlxLutNode (Int_t detectorId);

  // Copy constructor 
  GlxLutNode (GlxLutNode& node) { *this = node; }  

  // Modifiers
  void AddEntry(Int_t nodeId, TVector3 dir, Double_t pathid, Int_t nrefl, Double_t time, TVector3 hitpos, TVector3 digipos, Double_t weight=1);
  void SetDigiPos(TVector3 pos){fDigiPos = pos;}

  // Accessors
  Int_t Entries() { return fSize; }
  Double_t GetDetectorId() { return fDetectorId; }

  TVector3 GetEntry(Int_t entry) { return fNodeArray[entry]; }
  Double_t GetPathId(Int_t entry){ return fPathIdArray[entry]; }
  Double_t GetWeight(Int_t entry){ return fWeightArray[entry]; }
  Int_t GetNRefl(Int_t entry){ return fNRefl[entry]; }
  Double_t GetTime(Int_t entry){ return fTimeArray[entry]; }
  TVector3 GetHitPos(Int_t entry){ return fHitPos[entry]; }
  TVector3 GetDigiPos(){ return fDigiPos; }

protected:

  Int_t fDetectorId;
  Int_t fSize;
  TVector3 fDigiPos;

  std::vector<TVector3> fNodeArray;
  std::vector<TVector3> fHitPos;
  std::vector<Double_t> fPathIdArray;
  std::vector<Double_t> fWeightArray;
  std::vector<Int_t> fNRefl;
  std::vector<Double_t> fTimeArray;

protected: 
  ClassDef(GlxLutNode, 2);
  
};

#endif
