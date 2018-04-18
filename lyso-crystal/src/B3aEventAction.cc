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
// $Id$
//
/// \file B3aEventAction.cc
/// \brief Implementation of the B3aEventAction class

#include "B3aEventAction.hh"
#include "B3aRunAction.hh"
#include "TrackerHit.hh"
#include "CathodeHit.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RootAnalysisManager.hh"

#include <iostream>
#include <fstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3aEventAction::B3aEventAction(B3aRunAction* runAction)
 : G4UserEventAction(),
   fRunAction(runAction),
   fCollID_cryst(-1),
   fGlobalTime(),
   fLocalTime(),
   fEdep(),
   fNumOfElectorns()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3aEventAction::~B3aEventAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B3aEventAction::BeginOfEventAction(const G4Event* /*evt*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B3aEventAction::EndOfEventAction(const G4Event* evt )
{
   //Hits collections
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  if(!HCE) return;

   // Get hits collections IDs
   G4SDManager* SDMan = G4SDManager::GetSDMpointer();
  if (fCollID_cryst < 0)
    fCollID_cryst   = SDMan->GetCollectionID("tracker_crystal/crystal_hits");

  // Get hits
  TrackerHitsCollection* hits
  = static_cast<TrackerHitsCollection*>(HCE->GetHC(fCollID_cryst));

  G4int opt_id = SDMan->GetCollectionID("cathode/cathodeHits");
  CathodeHits* optical_hits = static_cast<CathodeHits*>(HCE->GetHC(opt_id));

  ClearAllVectors();

  G4int opt_interactions = optical_hits->entries();
  //Procces hit data
  G4int nofHits = hits->entries();
  G4double edep = 0.;
  G4int nOfPhotonInteractions = 0;
  G4int lastPhotonHitIndex = 0;

  // Edep and photon interaction check in every hit; save position of each hit
  for(G4int i = 0; i < nofHits;i++){
    TrackerHit* hit = (*hits)[i];
    G4int PDG_code = hit->GetPDG();
    G4String process = hit->GetProcessName();

    edep += hit->GetEdep();

    if (PDG_code == 22){
      nOfPhotonInteractions += 1;
      lastPhotonHitIndex++;
    }

    if (PDG_code == 22 && process == "phot")
      lastPhotonHitIndex = i;
  }

  for (int i = 0; i < nofHits; i++){
      TrackerHit* hit = (*hits)[i];
      fEdep.push_back(hit->GetEdep()/keV);
  }

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  for(int i = 0; i < opt_interactions; i++){
      fLocalTime.push_back((*optical_hits)[i]->GetLocalTime()/ns);
      fGlobalTime.push_back((*optical_hits)[i]->GetGlobalTime()/ns);
  }

  fNumOfElectorns.push_back(nOfPhotonInteractions);


  if(evt->GetEventID() % 25 == 0)
    G4cout << evt->GetEventID() << G4endl;


analysisManager->AddNtupleRow(0);
  //G4cout << "---------------------------------------" << G4endl;
}

void B3aEventAction::ClearAllVectors()
{
    fGlobalTime.clear();
    fLocalTime.clear();
    fEdep.clear();
    fNumOfElectorns.clear();
}
