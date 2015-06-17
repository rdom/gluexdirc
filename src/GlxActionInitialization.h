#ifndef GlxActionInitialization_h
#define GlxActionInitialization_h 1

#include "G4String.hh"
#include "G4VUserActionInitialization.hh"

class GlxActionInitialization : public G4VUserActionInitialization
{
public:
  GlxActionInitialization(G4String outfile);
  virtual ~GlxActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

  virtual G4VSteppingVerbose* InitializeSteppingVerbose() const;

private:
  G4String fOutFile;
};

#endif
