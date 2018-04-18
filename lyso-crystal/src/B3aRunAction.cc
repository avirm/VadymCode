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
// $Id: B3aRunAction.cc 99559 2016-09-27 07:02:21Z gcosmo $
//
/// \file B3aRunAction.cc
/// \brief Implementation of the B3aRunAction class

#include "B3aRunAction.hh"
#include "B3PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RootAnalysisManager.hh"
#include "B3aEventAction.hh"

#include <iostream>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3aRunAction::B3aRunAction()
: G4UserRunAction(),
fGoodEvents(0),
fSumDose(0.),
//fEdep(0.)
fOneHitDeposit(0),
fMultiHitsDeposit(0),
fParticlyDeposit(0)
{

   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

   // Default settings
   analysisManager->SetVerboseLevel(1);
   analysisManager->SetFirstHistoId(1);
   analysisManager->SetFileName("data");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3aRunAction::~B3aRunAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B3aRunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  const B3aEventAction* constEventAction = static_cast<const B3aEventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
  B3aEventAction* eventAction = const_cast<B3aEventAction*>(constEventAction);

  analysisManager->CreateNtuple("Optical photons", "Optical photons");
  analysisManager->CreateNtupleDColumn(0,"LocalTime", eventAction->GetLocalTime());
  analysisManager->CreateNtupleDColumn(0,"GlobalTime", eventAction->GetGlobalTime());
  analysisManager->CreateNtupleDColumn(0,"Edep", eventAction->GetEdep());
  analysisManager->CreateNtupleIColumn(0,"NumOfElectrons", eventAction->GetNumOfElectrons());
  analysisManager->FinishNtuple(0);

  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B3aRunAction::EndOfRunAction(const G4Run* run)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Print results
  //
  G4cout
   << G4endl
   << "--------------------End of Local Run------------------------"
   << G4endl
   << "  The run was " << nofEvents << G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
