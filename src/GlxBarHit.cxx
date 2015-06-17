#include "GlxBarHit.h"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<GlxBarHit>* GlxBarHitAllocator=0;

GlxBarHit::GlxBarHit()
  : G4VHit(),
    fTrackID(-1),
    fEdep(0.),
    fPos(G4ThreeVector()),
    fMom(G4ThreeVector())
{}

GlxBarHit::~GlxBarHit() {}

GlxBarHit::GlxBarHit(const GlxBarHit& right)
  : G4VHit()
{
  fTrackID   = right.fTrackID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fMom       = right.fMom;
}

const GlxBarHit& GlxBarHit::operator=(const GlxBarHit& right)
{
  fTrackID   = right.fTrackID;
  fEdep      = right.fEdep;
  fPos       = right.fPos;
  fMom       = right.fMom;

  return *this;
}

G4int GlxBarHit::operator==(const GlxBarHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

void GlxBarHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
    {
      G4Circle circle(fPos);
      circle.SetScreenSize(4.);
      circle.SetFillStyle(G4Circle::filled);
      G4Colour colour(0.,1.,0.);
      G4VisAttributes attribs(colour);
      circle.SetVisAttributes(attribs);
      pVVisManager->Draw(circle);
    }
}

void GlxBarHit::Print()
{
  G4cout
    << "Edep: "
    << std::setw(7) << G4BestUnit(fEdep,"Energy")
    << " Position: "
    << std::setw(7) << G4BestUnit( fPos,"Length")
    << G4endl;
}

