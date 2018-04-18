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
// $Id: PhysicsList.cc 73290 2013-08-23 10:04:20Z gcosmo $
//
/// \file radioactivedecay/rdecay02/src/PhysicsList.cc
/// \brief Implementation of the PhysicsList class
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PhysicsList.hh"
#include "PhysicsListMessenger.hh"


#include "G4RunManager.hh"
#include "G4VPhysicsConstructor.hh"
#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"
#include "G4EmParameters.hh"
// Physic lists (contained inside the Geant4 source code, in the 'physicslists folder')
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4LossTableManager.hh"
#include "G4UnitsTable.hh"
#include "G4ProcessManager.hh"
#include "G4IonFluctuations.hh"
#include "G4IonParametrisedLossModel.hh"
#include "G4EmProcessOptions.hh"
#include "G4ParallelWorldPhysics.hh"


// #include "Particles.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
// #include "PhysListHadron.hh"
#include "G4RegionStore.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4UnitsTable.hh"
#include "G4LossTableManager.hh"

#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"

#include "G4EmExtraPhysics.hh"
#include "G4HadronElasticPhysics.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4StoppingPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4NeutronTrackingCut.hh"
#include "G4DecayPhysics.hh"

#include "G4SystemOfUnits.hh"
using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() :
G4VModularPhysicsList(),
fCutForGamma(1.*mm), fCutForElectron(1.*mm),
fCutForPositron(1.*mm), 
fPMessenger(0),fDetectorCuts(0), fTargetCuts(0)
{
  G4LossTableManager::Instance();
  defaultCutValue =1.*mm;
  
  fPMessenger = new PhysicsListMessenger(this);
  
  SetVerboseLevel(2);
  //add new units for radioActive decays
  // 
  const G4double minute = 60*second;
  const G4double hour   = 60*minute;
  const G4double day    = 24*hour;
  const G4double year   = 365*day;
  new G4UnitDefinition("minute", "min", "Time", minute);
  new G4UnitDefinition("hour",   "h",   "Time", hour);
  new G4UnitDefinition("day",    "d",   "Time", day);
  new G4UnitDefinition("year",   "y",   "Time", year);     
  
  // Default physics
  RegisterPhysics(new G4DecayPhysics());
  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());
  // EM physics
  //   RegisterPhysics(new G4EmStandardPhysics());
  //   RegisterPhysics(new G4EmPenelopePhysics());
  RegisterPhysics(new G4EmStandardPhysics_option3());
  
  
  RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP()); // hadron -> proton + neutron + He G4 or can use G4HadronPhysicsQGSP_BIC_HP 
  //Warning G4 underestimate neutron rate
  RegisterPhysics( new G4EmExtraPhysics()); 
  RegisterPhysics( new G4HadronElasticPhysics());
  RegisterPhysics( new G4StoppingPhysics());
  RegisterPhysics( new G4IonBinaryCascadePhysics()); // ions > He other hadron modele QGSP B8 C10_11
  RegisterPhysics( new G4NeutronTrackingCut());
  //     G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
  
  
  //     // Deexcitation (in case of Atomic Rearangement)
  //   //
  //   G4UAtomicDeexcitation* de = new G4UAtomicDeexcitation();
  //   de->InitialiseAtomicDeexcitation();
  //   de->SetFluo(false);
  //   de->SetAuger(false);   
  //   de->SetPIXE(false);  
  //   G4LossTableManager::Instance()->SetAtomDeexcitation(de);  
  //   
  //   G4EmParameters * theParameters = G4EmParameters::Instance();
  //   theParameters->SetDeexcitationIgnoreCut(true);
  //   
  // Atom Deextiation
  //   G4VAtomDeexcitation * de = new G4UAtomicDeexcitation(); 
  //   de->InitialiseAtomicDeexcitation();
  //   de->SetPIXE(false);
  //   G4LossTableManager::Instance()->SetAtomDeexcitation(de);
  //   
  //   G4EmParameters * theParameters = G4EmParameters::Instance();
  //   theParameters->SetDeexcitationIgnoreCut(true);
  // geant4 physiclyst reference
  
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SelectPhysicsList(const G4String& name)
{
  if (verboseLevel>1) {
    G4cout << "PhysicsList::SelectPhysicsList: <" << name << ">" << G4endl;
  }
  
  if (name == "QGSP_BIC_HP"){
    
    
    flagHP=false;
    G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
    
    cout << "Initialise " <<endl;
  }
  //    G4RunManager::GetRunManager() -> PhysicsHasBeenModified();
  
  if (name == "QGSP_BIC_HP"){;} /*RegisterPhysics(new G4EmPenelopePhysics());
  G4RunManager::GetRunManager() -> PhysicsHasBeenModified();*/
  //   // default  Had physics
  //   if (name == "Hadron" && !fHadPhysicsList) {
  //     fHadPhysicsList = new PhysListHadron("hadron");
  //   } else if (name == "QGSP_BERT") {
  //     AddExtraBuilders(false);
  //     fHadPhysicsList = new G4HadronPhysicsQGSP_BERT(verboseLevel);
  //   } else if (name == "QGSP_BIC" && !fHadPhysicsList) {
  //     AddExtraBuilders(false);
  //     fHadPhysicsList = new G4HadronPhysicsQGSP_BIC(verboseLevel);
  //   } else if (name == "QGSP_BERT_HP"  && !fHadPhysicsList) {
  //     AddExtraBuilders(true);
  //     fHadPhysicsList = new G4HadronPhysicsQGSP_BERT_HP(verboseLevel);
  //   } else if (name == "QGSP_BIC_HP"  && !fHadPhysicsList) {
  //     AddExtraBuilders(true);
  //     fHadPhysicsList = new G4HadronPhysicsQGSP_BIC_HP(verboseLevel);
  //   } else if (name == "emlivermore") {
  //       delete fEmPhysicsList;
  //       fEmPhysicsList = new G4EmLivermorePhysics(verboseLevel);
  //   } else if (name == "empenelope") {
  //       delete fEmPhysicsList;
  //       fEmPhysicsList = new G4EmPenelopePhysics(verboseLevel);
  //   } else if (name == "emstandard") {
  //       delete fEmPhysicsList;
  //       fEmPhysicsList = new G4EmStandardPhysics(verboseLevel);
  //   } else if (name == "emstandard_opt4") {
  //       delete fEmPhysicsList;
  //       fEmPhysicsList = new G4EmStandardPhysics_option4(verboseLevel);
  //   } else {
  //       G4cout << "PhysicsList WARNING wrong or unkonwn <" 
  //              << name << "> Physics " << G4endl;
  //   }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts()
{
  //   SetCutValue(fCutForGamma, "gamma");
  //   SetCutValue(fCutForElectron, "e-");
  //   SetCutValue(fCutForPositron, "e+");
  G4cout << "world cuts are set" << G4endl;
  
  if( !fTargetCuts ) SetTargetCut(fCutForElectron);
  G4Region* region = (G4RegionStore::GetInstance())->GetRegion("Target");
  //   region->SetProductionCuts(fTargetCuts);
  G4cout << "Target cuts are set" << G4endl;
  
  if( !fDetectorCuts ) SetDetectorCut(fCutForElectron);
  region = (G4RegionStore::GetInstance())->GetRegion("Detector");
  region->SetProductionCuts(fDetectorCuts);
  G4cout << "Detector cuts are set" << G4endl;
  
  if (verboseLevel>0) DumpCutValuesTable();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForGamma(G4double cut)
{
  fCutForGamma = cut;
  SetParticleCuts(fCutForGamma, G4Gamma::Gamma());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForElectron(G4double cut)
{
  fCutForElectron = cut;
  SetParticleCuts(fCutForElectron, G4Electron::Electron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCutForPositron(G4double cut)
{
  fCutForPositron = cut;
  SetParticleCuts(fCutForPositron, G4Positron::Positron());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetTargetCut(G4double cut)
{
  if( !fTargetCuts ) fTargetCuts = new G4ProductionCuts();
  
  fTargetCuts->SetProductionCut(cut, idxG4GammaCut);
  fTargetCuts->SetProductionCut(cut, idxG4ElectronCut);
  fTargetCuts->SetProductionCut(cut, idxG4PositronCut);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetDetectorCut(G4double cut)
{
  if( !fDetectorCuts ) fDetectorCuts = new G4ProductionCuts();
  
  fDetectorCuts->SetProductionCut(cut, idxG4GammaCut);
  fDetectorCuts->SetProductionCut(cut, idxG4ElectronCut);
  fDetectorCuts->SetProductionCut(cut, idxG4PositronCut);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
