// -----------------------------------------
// GlxPhotonInfo.h
//
// Created on: 18.10.2013
// Author: R.Dzhygadlo at gsi.de
// -----------------------------------------
// Container for DrcTrackInfo

#ifndef GlxPhotonInfo_h
#define GlxPhotonInfo_h 1

#include "GlxAmbiguityInfo.h"

#include "TObject.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include <vector>

class GlxPhotonInfo : public TObject {

public:
  
  // Default constructor
  GlxPhotonInfo ();

  // Default destructor
  ~GlxPhotonInfo ();

  // Copy constructor 
  GlxPhotonInfo (const GlxPhotonInfo& val):TObject() { *this = val; }  

  // Mutators
  void SetHitTime(Double_t val)               {fHitTime = val;}
  void SetReflected(Bool_t val)               {fReflected = val;}
  void SetEvReflections(Int_t val)            {fEvReflections = val;}
  void SetSensorId(Int_t val)                 {fSensorId = val;}
  void SetMcPrimeMomentumInBar(TVector3 val)  {fMcPrimeMomentumInBar = val;}
  void SetMcCherenkovInBar(Double_t val)      {fMcCherenkovInBar = val;}
  
  void AddAmbiguity(GlxAmbiguityInfo ambiguity);

  // Accessors  
  Double_t    GetHitTime()	              {return fHitTime;}    
  Bool_t GetReflected()	                      {return fReflected;}
  Int_t GetEvReflections()	              {return fEvReflections;}
  Int_t GetSensorId()	                      {return fSensorId;}

  Int_t GetAmbiguitySize()	              {return fAmbiguitySize;}
  GlxAmbiguityInfo GetAmbiguity(Int_t id)     {return fAmbiguityArray[id];}
  TVector3 GetMcPrimeMomentumInBar()	      {return fMcPrimeMomentumInBar;}
  Double_t  GetMcCherenkovInBar()	      {return fMcCherenkovInBar;}    

protected:

  std::vector<GlxAmbiguityInfo> fAmbiguityArray;
  Int_t fAmbiguitySize;

  Int_t    fSensorId;
  Double_t fHitTime;
  Bool_t   fReflected;
  Int_t    fEvReflections;
  TVector3 fMcPrimeMomentumInBar;
  Double_t fMcCherenkovInBar;
 
  ClassDef(GlxPhotonInfo,1)
};

#endif
