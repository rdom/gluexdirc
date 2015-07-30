// -----------------------------------------
// GlxPrizmHit class
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#include "GlxPrizmHit.h"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<GlxPrizmHit>* GlxPrizmHitAllocator=0;

GlxPrizmHit::GlxPrizmHit()
 : G4VHit(),
   fTrackID(-1),
   fNormalId(-1),
   fEdep(0.),
   fPos(G4ThreeVector()){}

GlxPrizmHit::~GlxPrizmHit(){}

GlxPrizmHit::GlxPrizmHit(const GlxPrizmHit& right)
  : G4VHit(){
  fTrackID   = right.fTrackID;
  fNormalId = right.fNormalId;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
}

const GlxPrizmHit& GlxPrizmHit::operator=(const GlxPrizmHit& right){
  fTrackID   = right.fTrackID;
  fNormalId = right.fNormalId;
  fEdep      = right.fEdep;
  fPos       = right.fPos;

  return *this;
}

G4int GlxPrizmHit::operator==(const GlxPrizmHit& right) const{
  return ( this == &right ) ? 1 : 0;
}

void GlxPrizmHit::Draw(){
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager){
    G4Circle circle(fPos);
    circle.SetScreenSize(4.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void GlxPrizmHit::Print(){
  G4cout
     << "  trackID: " << fTrackID << " chamberNb: " << fNormalId
     << "Edep: "
     << std::setw(7) << G4BestUnit(fEdep,"Energy")
     << " Position: "
     << std::setw(7) << G4BestUnit( fPos,"Length")
     << G4endl;
}

