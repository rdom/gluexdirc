#ifndef GlxRunAction_h
#define GlxRunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"

class G4Timer;
class G4Run;

class GlxRunAction : public G4UserRunAction
{
public:
  GlxRunAction(G4String outfile);
  virtual ~GlxRunAction();

public:
  virtual void BeginOfRunAction(const G4Run* aRun);
  virtual void EndOfRunAction(const G4Run* aRun);

private:
  G4Timer* fTimer;
  G4String fOutFile;
};

#endif
