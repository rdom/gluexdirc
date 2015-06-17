//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: GlxActionInitialization.cc 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file GlxActionInitialization.cc
/// \brief Implementation of the GlxActionInitialization class

#include "GlxActionInitialization.h"
#include "GlxPrimaryGeneratorAction.h"
#include "GlxRunAction.h"
#include "GlxSteppingAction.h"
#include "GlxStackingAction.h"
#include "GlxSteppingVerbose.h"
#include "GlxTrackingAction.h"
#include "GlxEventAction.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GlxActionInitialization::GlxActionInitialization(G4String outfile)
  : G4VUserActionInitialization(){
  fOutFile = outfile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GlxActionInitialization::~GlxActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GlxActionInitialization::BuildForMaster() const
{
  SetUserAction(new GlxRunAction(fOutFile));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GlxActionInitialization::Build() const
{
  SetUserAction(new GlxPrimaryGeneratorAction());
  SetUserAction(new GlxRunAction(fOutFile));
  SetUserAction(new GlxSteppingAction());
  SetUserAction(new GlxStackingAction());
  SetUserAction(new GlxTrackingAction());
  SetUserAction(new GlxEventAction());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSteppingVerbose*
GlxActionInitialization::InitializeSteppingVerbose() const
{
  return new GlxSteppingVerbose();
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
