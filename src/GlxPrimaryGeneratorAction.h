// -----------------------------------------
// GlxPrimaryGeneratorAction class
//
// Author  : R.Dzhygadlo at gsi.de
// -----------------------------------------

#ifndef GlxPrimaryGeneratorAction_h
#define GlxPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class GlxPrimaryGeneratorMessenger;

class GlxPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    GlxPrimaryGeneratorAction();
    virtual ~GlxPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event*);

    void SetOptPhotonPolar();
    void SetOptPhotonPolar(G4double);

  private:
    G4ParticleGun* fParticleGun;
    GlxPrimaryGeneratorMessenger* fGunMessenger;
};

#endif
