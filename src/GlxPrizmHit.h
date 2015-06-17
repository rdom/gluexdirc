#ifndef GlxPrizmHit_h
#define GlxPrizmHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"


class GlxPrizmHit : public G4VHit
{
  public:
    GlxPrizmHit();
    GlxPrizmHit(const GlxPrizmHit&);
    virtual ~GlxPrizmHit();

    // operators
    const GlxPrizmHit& operator=(const GlxPrizmHit&);
    G4int operator==(const GlxPrizmHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetNormalId(G4int chamb)       { fNormalId = chamb; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetNormalId() const    { return fNormalId; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };

  private:

      G4int         fTrackID;
      G4int         fNormalId;
      G4double      fEdep;
      G4ThreeVector fPos;
};


typedef G4THitsCollection<GlxPrizmHit> GlxPrizmHitsCollection;

extern G4ThreadLocal G4Allocator<GlxPrizmHit>* GlxPrizmHitAllocator;

inline void* GlxPrizmHit::operator new(size_t)
{
  if(!GlxPrizmHitAllocator)
      GlxPrizmHitAllocator = new G4Allocator<GlxPrizmHit>;
  return (void *) GlxPrizmHitAllocator->MallocSingle();
}

inline void GlxPrizmHit::operator delete(void *hit)
{
  GlxPrizmHitAllocator->FreeSingle((GlxPrizmHit*) hit);
}

#endif
