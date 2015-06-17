// -----------------------------------------
// GlxPhotonInfo.h
//
// Created on: 18.10.2013
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#include "GlxPhotonInfo.h"

ClassImp(GlxPhotonInfo)

// -----   Default constructor   -------------------------------------------
GlxPhotonInfo::GlxPhotonInfo(){ 
  fAmbiguitySize = 0;
  fHitTime = 0;
  fReflected = kFALSE;
  fEvReflections = 0;
  fSensorId = 0;
}

GlxPhotonInfo::~GlxPhotonInfo(){ 
}


void GlxPhotonInfo::AddAmbiguity(GlxAmbiguityInfo ambiguity){
  fAmbiguityArray.push_back(ambiguity);
  fAmbiguitySize++;
}
