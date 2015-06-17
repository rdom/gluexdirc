// -----------------------------------------
// GlxBarHit.h
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxBarHit_h
#define GlxBarHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


class GlxBarHit : public G4VHit
{
  public:
    GlxBarHit();
    GlxBarHit(const GlxBarHit&);
    virtual ~GlxBarHit();

    // operators
    const GlxBarHit& operator=(const GlxBarHit&);
    G4int operator==(const GlxBarHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };
    void SetMom (G4ThreeVector mom){ fMom = mom; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };
    G4ThreeVector GetMom() const { return fMom; };

  private:

      G4int         fTrackID;
      G4double      fEdep;
      G4ThreeVector fPos;
      G4ThreeVector fMom;
};


typedef G4THitsCollection<GlxBarHit> GlxBarHitsCollection;

extern G4ThreadLocal G4Allocator<GlxBarHit>* GlxBarHitAllocator;

inline void* GlxBarHit::operator new(size_t)
{
  if(!GlxBarHitAllocator)
      GlxBarHitAllocator = new G4Allocator<GlxBarHit>;
  return (void *) GlxBarHitAllocator->MallocSingle();
}

inline void GlxBarHit::operator delete(void *hit)
{
  GlxBarHitAllocator->FreeSingle((GlxBarHit*) hit);
}

#endif
