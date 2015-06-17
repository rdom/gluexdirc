// Make this appear first!
#include "G4Timer.hh"
#include "GlxRunAction.h"
#include "G4Run.hh"
#include "GlxManager.h"


GlxRunAction::GlxRunAction(G4String outfile)
 : G4UserRunAction(),
   fTimer(0)
{
  fTimer = new G4Timer;
  fOutFile = outfile;
}

GlxRunAction::~GlxRunAction()
{
  delete fTimer;
}

void GlxRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  fTimer->Start();


  GlxManager::Instance(fOutFile);
  
}

void GlxRunAction::EndOfRunAction(const G4Run* aRun)
{
  GlxManager::Instance()->FillLut();
  GlxManager::Instance()->Save();
  
  fTimer->Stop();
  G4cout << "number of event = " << aRun->GetNumberOfEvent()
         << " " << *fTimer << G4endl;
}

