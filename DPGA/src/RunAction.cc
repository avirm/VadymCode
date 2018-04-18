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
// $Id: RunAction.cc 71323 2013-06-13 16:54:23Z gcosmo $
//
//
// @abongrand 2017 arthur.bongrand@clermont.in2p3.fr
//
/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4GenericMessenger.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
// #include "G4ParameterManager.hh"
#include "G4RootAnalysisManager.hh"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <vector>
#include "EventAction.hh"
#include "DetectorConstruction.hh"

// #include "HistoManager.cc"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
: G4UserRunAction()
{ 
  
  //add new units for dose
  // 
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;  
  const G4double picogray  = 1.e-12*gray;
  
  new G4UnitDefinition("milligray", "milliGy" , "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy" , "Dose", microgray);
  new G4UnitDefinition("nanogray" , "nanoGy"  , "Dose", nanogray);
  new G4UnitDefinition("picogray" , "picoGy"  , "Dose", picogray);      
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;
  
  // Default settings
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstHistoId(1);
  analysisManager->SetFileName("B3");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{ 
  G4cout<<"Beginning of Run "<<run->GetRunID()<<"  ------------------"<<G4endl;
  
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  //   SetAutoSeed(true);
  // Other Method for random number seed
  G4long seed=time(0); //returns time in seconds as an integer
  CLHEP::HepRandom::setTheSeed(seed);//changes the seed of the random engine
  
  //Initalise time
  initial_time = time(NULL);
  fExecutive_Time_Run = 0;
  
  initial_time_G4 = 0 *s;
  initial_time_G4 = time(0);
  
  const DetectorConstruction* constDetectorConstruction = dynamic_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction ());
  DetectorConstruction* detectorConstruction = const_cast<DetectorConstruction*>(constDetectorConstruction);
  
  
  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in Analysis.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  
  // Get event action for Create NTuple with vector 
  //see https://bugzilla-geant4.kek.jp/show_bug.cgi?id=1724
  const EventAction* constEventAction = static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
  EventAction* eventAction = const_cast<EventAction*>(constEventAction);
  
  
  
  
  // For Add delete some values / Histo...
  fSetup_Target  = "Default";
  fSetup_Detector = "Default";
  
  fSetup_Target = detectorConstruction->GetTargetSetup();
  eventAction->SetTargetSetup(fSetup_Target);
  
  fSetup_Detector = detectorConstruction->GetDetectorSetup();
  eventAction->SetDetectorSetup(fSetup_Detector);
  
  cout << "Setup Target RunAction" << fSetup_Target << endl;
  cout << "Setup Detecteur RunAction " << fSetup_Detector << endl;
  
  // Book histograms, ntuple
  //
  // histograms
  //    for (G4int k=0; k<MaxHisto; k++) {
  //     fHistId[k] = 0;    
  //   }
  
  // Creating 1D histograms
  fHistId[1] = analysisManager
  // h1 Id = 1
  ->CreateH1("Patient_totEdep/event","TotEdep/Event", 150, 0., 1.5);
  
  fHistId[2] = analysisManager
  // h1 Id = 2
  ->CreateH1("Cristaux_totEdep/event","Cristaux_Edep/Event (MeV)", 150, 0., 1.5);
  
  fHistId[3] = analysisManager
  // h1 Id = 3
  ->CreateH1("Cristaux_Edep/PMT/Event","Cristaux_Edep/PMT/Event (MeV)", 150, 0., 1.5);
  
  fHistId[4] = analysisManager
  // h1 Id = 4
  ->CreateH1("Cristaux_Hits/Event","Cristaux_Hits/Event (MeV)", 150, 0., 150);
  
  fHistId[5] = analysisManager
  // h1 Id = 5
  ->CreateH1("LeftRight","1 sans threshold 2 avec threshold", 10, 0., 10);
  
  fHistId[6] = analysisManager
  // h1 Id = 6
  ->CreateH1("RatioB+_Run"," Nbr Beta+ / run", 100, 0, 100);
  
  // Primmary Bragg Peak 
  
  fHistId[7] = analysisManager
  // h1 Id = 7
  ->CreateH1("Primary_BraggPeak_Z","PBP inside Target Z ", 101, -5, 5);
  
  fHistId[8] = analysisManager
  // h1 Id = 8
  ->CreateH3("Primary_BraggPeak_3D","PBP inside Target 3D", 101, -5., 5, 101, -5., 5, 101, -5., 5);
  
  //        analysisManager
  //     // h1 Id = 6
  //     ->CreateH1("Primary_BraggPeak_t<30s","PBP inside Target t<30s", 10, 0., 10);
  //     
  //        analysisManager
  //     // h1 Id = 6
  //     ->CreateH1("Primary_BraggPeak","PBP inside Target ", 10, 0., 10);
  //     
  //        analysisManager
  //     // h1 Id = 6
  //     ->CreateH1("Primary_BraggPeak","PBP inside Target ", 10, 0., 10);
  
  
  
  
  // Creating ntuple
  // 
  
  // ***************************************************
  //            Patient or Mouse or Ghost
  // ***************************************************
 
    
    analysisManager->CreateNtuple("Hits_Patient", "Hits_Patient");
    
    // At begin I put quantity which need column Id
    
    // Event
    analysisManager->CreateNtupleIColumn(0,"Event");      // column Id =  0 
    
    // TotalEdp Event
    analysisManager->CreateNtupleDColumn(0,"TotalEdep");  // column Id = 1 
    
    // Setup
    // Ghost or Target
    analysisManager->CreateNtupleSColumn(0,"Setup_Target");      // column Id = 2
    // DPGA
    analysisManager->CreateNtupleSColumn(0,"Setup_DPGA");      // column Id = 3
    
    
    //     analysisManager->CreateNtupleSColumn(0,"ProcessName_S");      // column Id = 4
    
    //     analysisManager->CreateNtupleSColumn(0,"CreationProcessName_S");      // column Id =5
    
     if( fSetup_Detector == "Scoring_Target" ){cout << "Scoring No patient Tree !" << endl;}
     else{
    //***************************************
    // Quantity which don't need column Id
    
    // Energy
    // Edep
    analysisManager->CreateNtupleDColumn(0,"Edep",eventAction->GetVec_Edep());  // column Id = 5
    
    // Postion World and Local
    // World
    analysisManager->CreateNtupleDColumn(0,"World_PosX",eventAction->GetVec_World_PosX());  // column Id = 6
    analysisManager->CreateNtupleDColumn(0,"World_PosY",eventAction->GetVec_World_PosY());  // column Id = 7
    analysisManager->CreateNtupleDColumn(0,"World_PosZ",eventAction->GetVec_World_PosZ());  // column Id = 8
    
    // Time 
    //Event
    analysisManager->CreateNtupleDColumn(0,"Time_Event",eventAction->GetVec_Time_Event());  // column Id = 9
    
    // Tracking
    // Parent ID
    analysisManager->CreateNtupleIColumn(0,"ParentID",eventAction->GetVec_ParentID()); // column Id = 10
    // Track ID
    analysisManager->CreateNtupleIColumn(0,"TrackID",eventAction->GetVec_TrackID());  // column Id = 11
    // PDG Code
    analysisManager->CreateNtupleIColumn(0,"PDGCode",eventAction->GetVec_PDGCode());  // column Id = 12
    // Process Name
    analysisManager->CreateNtupleIColumn(0,"ProcessName",eventAction->GetVec_processName());  // column Id = 13
    // Creation Process name
    analysisManager->CreateNtupleIColumn(0,"CreationProcessName",eventAction->GetVec_CreationProcessName());  // column Id = 14
    
    // Other Informations
    // Vertex Postion
    analysisManager->CreateNtupleDColumn(0,"Vertex_PosX",eventAction->GetVec_Vertex_PosX());  // column Id = 15
    analysisManager->CreateNtupleDColumn(0,"Vertex_PosY",eventAction->GetVec_Vertex_PosY());  // column Id = 16
    analysisManager->CreateNtupleDColumn(0,"Vertex_PosZ",eventAction->GetVec_Vertex_PosZ());  // column Id = 17
    
    // Additional info : 
    
    // Edep Method 2
    analysisManager->CreateNtupleDColumn(0,"DeltaEnergy",eventAction->GetVec_Edep_M2());  // column Id = 18
    
    analysisManager->CreateNtupleDColumn(0,"KineticEnergy",eventAction->GetVec_KineticEnergy());  // column Id = 19
    
    analysisManager->CreateNtupleDColumn(0,"KineticEnergy_Vertex",eventAction->GetVec_KineticEnergy_Vertex());  // column Id = 20
    
    analysisManager->CreateNtupleDColumn(0,"Local_PosX",eventAction->GetVec_Local_PosX());  // column Id = 21
    analysisManager->CreateNtupleDColumn(0,"Local_PosY",eventAction->GetVec_Local_PosY());  // column Id = 22
    analysisManager->CreateNtupleDColumn(0,"Local_PosZ",eventAction->GetVec_Local_PosZ());  // column Id = 23
    
    analysisManager->CreateNtupleDColumn(0,"Local_Vertex_PosX",eventAction->GetVec_Vertex_Local_PosX());  // column Id = 24
    analysisManager->CreateNtupleDColumn(0,"Local_Vertex_PosY",eventAction->GetVec_Vertex_Local_PosY());  // column Id = 25
    analysisManager->CreateNtupleDColumn(0,"Local_Vertex_PosZ",eventAction->GetVec_Vertex_Local_PosZ());  // column Id = 26
    
    //  Secondary_BetaPlus
    analysisManager->CreateNtupleIColumn(0,"Secondary_BetaPlus",eventAction->GetVec_Secondary_BetaPlus()); // column Id = 27
    
    //  Secondary_PromptGamma
    analysisManager->CreateNtupleIColumn(0,"Secondary_PromptGamma",eventAction->GetVec_Secondary_PromptGamma()); // column Id = 28
    // Secondary_Neutron
    analysisManager->CreateNtupleIColumn(0,"Secondary_Neutron",eventAction->GetVec_Secondary_Neutron()); // column Id = 29
     }
    
    analysisManager->FinishNtuple(0);
  
  
  // ***************************************************
  //           End Patient or Mouse or Ghost
  // ***************************************************
  
  // ***************************************************
  //            DPGA Crystal
  // ***************************************************
  
  
  if( fSetup_Detector == "Without_Detector" || fSetup_Detector == "Scoring_Target" ){cout << "No Detector No Dectector Hits Tree !" << endl;}
  else{
    analysisManager->CreateNtuple("Hits_Crystal", "Hits_Crystal");
    
    // At begin I put quantity which need column Id
    
    // Event
    analysisManager->CreateNtupleIColumn(1,"Event");      // column Id =  0
    // TotalEdp Event
    analysisManager->CreateNtupleDColumn(1,"TotalEdep");  // column Id = 1 

    //     analysisManager->CreateNtupleSColumn(1,"ProcessName_S");      // column Id = 2
    //     analysisManager->CreateNtupleSColumn(1,"CreationProcessName_S");      // column Id =3
    //***************************************
    // Quantity which don't need column Id
    
    // Energy
    // Edep
    analysisManager->CreateNtupleDColumn(1,"Edep",eventAction->GetVec_Edep());  // column Id = 5
    
   
    analysisManager->CreateNtupleDColumn(1,"World_PosX",eventAction->GetVec_World_PosX());  // column Id = 6
    analysisManager->CreateNtupleDColumn(1,"World_PosY",eventAction->GetVec_World_PosY());  // column Id = 7
    analysisManager->CreateNtupleDColumn(1,"World_PosZ",eventAction->GetVec_World_PosZ());  // column Id = 8
    
    // Time 
    //Event
    analysisManager->CreateNtupleDColumn(1,"Time_Event",eventAction->GetVec_Time_Event());  // column Id = 9
    
    // Tracking
    // Parent ID
    analysisManager->CreateNtupleIColumn(1,"ParentID",eventAction->GetVec_ParentID()); // column Id = 10
    // Track ID
    analysisManager->CreateNtupleIColumn(1,"TrackID",eventAction->GetVec_TrackID());  // column Id = 11
    // PDG Code
    analysisManager->CreateNtupleIColumn(1,"PDGCode",eventAction->GetVec_PDGCode());  // column Id = 12
    // Process Name
    analysisManager->CreateNtupleIColumn(1,"ProcessName",eventAction->GetVec_processName());  // column Id = 13
    // Creation Process name
    analysisManager->CreateNtupleIColumn(1,"CreationProcessName",eventAction->GetVec_CreationProcessName());  // column Id = 14
    
    // Other Informations
    // Vertex Postion
    analysisManager->CreateNtupleDColumn(1,"Vertex_PosX",eventAction->GetVec_Vertex_PosX());  // column Id = 15
    analysisManager->CreateNtupleDColumn(1,"Vertex_PosY",eventAction->GetVec_Vertex_PosY());  // column Id = 16
    analysisManager->CreateNtupleDColumn(1,"Vertex_PosZ",eventAction->GetVec_Vertex_PosZ());  // column Id = 17
    
    // Additional info : 
    
    // Edep Method 2
    analysisManager->CreateNtupleDColumn(1,"DeltaEnergy",eventAction->GetVec_Edep_M2());  // column Id = 18
    
    analysisManager->CreateNtupleDColumn(1,"KineticEnergy",eventAction->GetVec_KineticEnergy());  // column Id = 19
    
    analysisManager->CreateNtupleDColumn(1,"KineticEnergy_Vertex",eventAction->GetVec_KineticEnergy_Vertex());  // column Id = 21
    
    //  Local Postions and associate vertex
    
    analysisManager->CreateNtupleDColumn(1,"Local_PosX",eventAction->GetVec_Local_PosX());  // column Id = 21
    analysisManager->CreateNtupleDColumn(1,"Local_PosY",eventAction->GetVec_Local_PosY());  // column Id = 22
    analysisManager->CreateNtupleDColumn(1,"Local_PosZ",eventAction->GetVec_Local_PosZ());  // column Id = 23
    
    analysisManager->CreateNtupleDColumn(1,"Local_Vertex_PosX",eventAction->GetVec_Vertex_Local_PosX());  // column Id = 24
    analysisManager->CreateNtupleDColumn(1,"Local_Vertex_PosY",eventAction->GetVec_Vertex_Local_PosY());  // column Id = 25
    analysisManager->CreateNtupleDColumn(1,"Local_Vertex_PosZ",eventAction->GetVec_Vertex_Local_PosZ());  // column Id = 26
    
    
    
    // N° PMT
    analysisManager->CreateNtupleIColumn(1,"Crystal_Number", eventAction->GetVec_No_Copy());// column Id = 
    
    analysisManager->CreateNtupleIColumn(1,"Relative_Crystal_Number", eventAction->GetVec_Relative_No_Copy());// column Id = 
    // N° Quartet 
    analysisManager->CreateNtupleIColumn(1,"PMT_Quartet", eventAction->GetVec_No_Quartet());// column Id = 17
    
    analysisManager->CreateNtupleIColumn(1,"Relative_PMT_Quartet", eventAction->GetVec_Relative_No_Quartet());// column Id = 
    // N° Line
    analysisManager->CreateNtupleIColumn(1,"PMT_Line", eventAction->GetVec_No_Line());// column Id = 19
    // If PMT is on LeftOrRight part
    analysisManager->CreateNtupleIColumn(1,"LeftOrRight", eventAction->GetVec_LeftOrRight());// column Id = 
    
    
    // Edep / PMT
    
    analysisManager->CreateNtupleDColumn(1,"Edep_Per_PMT",eventAction->GetVec_Edep_Per_Crystal() ); // column Id = 22
    
    
    analysisManager->CreateNtupleIColumn(1,"Edep_Per_PMT_Crystal_Number",eventAction->GetVec_Edep_Per_Crystal_Crystal_Number() ); // column Id = 23
    
    
    
    analysisManager->FinishNtuple(1);
  }
  // ***************************************************
  //               End DPGA Crystal
  // ***************************************************
  
  
  
  
  // Get analysis manager
  //   G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // Open an output file 
  // The default file name is set in RunAction::RunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  
  
  
    // Get event action for Create NTuple with vector 
  //see https://bugzilla-geant4.kek.jp/show_bug.cgi?id=1724
  const EventAction* constEventAction = static_cast<const EventAction*>(G4RunManager::GetRunManager()->GetUserEventAction());
  EventAction* eventAction = const_cast<EventAction*>(constEventAction);
  
  if( fSetup_Detector == "Without_Detector" || fSetup_Detector == "Scoring_Target" ){cout << "No Detector No Dectector Position !" << endl;}
  else{
  
  cout << "\n*********Crystals (or Detector) Prosition **************** " << endl;
  
  G4int Compteur_Crystal = 0;
  
  ofstream fichier("Crystals_Positions.txt", ios::out | ios::trunc);  //déclaration du flux et ouverture du fichier //ios::trunc pour remmetre a 0 // ios::app 
  
     if(fichier){fichier << "# NumeroPMT X Y Z unit" << "\n";  }
      else{ 
	cout << "Erreur à l'ouverture pour AllProcess.txt !" << endl;
      }
  
  for(G4int i=0;i<eventAction->Getmap_fPosition_Crystal().size();i++)
  {
    cout << i << " " << eventAction->Getmap_fPosition_Crystal()[i].getX()/mm << " " << eventAction->Getmap_fPosition_Crystal()[i].getY()/mm << " " << eventAction->Getmap_fPosition_Crystal()[i].getZ()/mm   << endl;
    
    
    if(eventAction->Getmap_fPosition_Crystal()[i].x() == 0) Compteur_Crystal++;
    
    if(fichier){ // Test Existing folder
	fichier << i << " " << eventAction->Getmap_fPosition_Crystal()[i].getX()/mm << " " << eventAction->Getmap_fPosition_Crystal()[i].getY()/mm << " " << eventAction->Getmap_fPosition_Crystal()[i].getZ()/mm   << " mm"<<"\n"; 
      }
      else{ 
	cout << "Erreur à l'ouverture pour AllProcess.txt !" << endl;
      }
  }
  
  if(fSetup_Detector=="Full_DPGA") cout << "Taux de Crystals DPGA touched " <<  (G4double)Compteur_Crystal/ 240 *100 << " %" << endl;
  
  fichier.close();  // on referme le fichier
  
  cout << "*********End Crystals (or Detector) Prosition**************** " << endl;
  cout << endl;
  
  }
  
  cout << "\n*********All Process**************** " << endl;

  
  std::sort (eventAction->GetVec_AllProcessName().begin(), eventAction->GetVec_AllProcessName().end());
  
  
  std::vector<G4String> AllProcess_Compare;
  G4String Read_Temp="";
  
  int nbLignes = 0;
  ifstream myfile ("AllProcess.txt");
  if (myfile.is_open())
  {
    while(!myfile.eof()){
      nbLignes++;
      myfile >> Read_Temp ;
      AllProcess_Compare.push_back(Read_Temp);
    }
    cout << "AllProcess.txt contient " << nbLignes << " lignes." << endl;
    myfile.close();
  }
  else cout << "Unable to open AllProcess.txt"; 
  
  // 	for(G4int i=0;i<AllProcess_Compare.size();i++){   
  //       cout << AllProcess_Compare[i] << endl;
  // 	}   
  // cout << "end " << endl;
  if(std::equal (AllProcess_Compare.begin(), AllProcess_Compare.end(),eventAction->GetVec_AllProcessName().begin() ) == 0 ||AllProcess_Compare.size()<2  ){
    
    ofstream fichier("AllProcess.txt", ios::out | ios::app);  //déclaration du flux et ouverture du fichier //ios::trunc pour remmetre a 0 // ios::app 
    
    for(G4int i=0;i<eventAction->GetVec_AllProcessName().size();i++){
      
      cout << eventAction->GetVec_AllProcessName()[i] << endl;
      
      if(fichier){ // Test Existing folder
	fichier << eventAction->GetVec_AllProcessName()[i] <<"\n"; 
      }
      else{ cout << "Erreur à l'ouverture pour AllProcess.txt !" << endl;}
    }
    fichier.close();  // on referme le fichier
    
  }
  cout << "*********End All Process**************** " << endl;
  cout << endl;
  
  // save histograms & ntuple
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
  
  // Print Run Time 
  //
  actual_time = time (NULL);
  actual_time_G4 = 0 *s;
  actual_time_G4 = time(0) ;
  //   cout << "TIme " << actual_time_G4 << " - "<< initial_time_G4 << endl;
  G4double TimeComputer = 0*s;
  TimeComputer = (actual_time_G4-initial_time_G4)*1e9  ;
  //   cout << "TIme " << TimeComputer <<  endl;
  G4cout<<"computing time G4Method: "<<G4BestUnit(TimeComputer, "Time")<<G4endl;
  G4cout<<"computing time : "<<(actual_time-initial_time) << " s"<<G4endl;
  //********************************************************************************
  
  G4cout<<"End of Run "<< run->GetRunID()<<"  ------------------"<<G4endl;
  G4cout<<G4endl;
  
  
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  //    const PrimaryGeneratorAction* generatorAction
  //     = static_cast<const PrimaryGeneratorAction*>(
  //         G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  //    G4String partName;
  //    if (generatorAction){
  //     G4ParticleDefinition* particle 
  //       = generatorAction->GetParticleGun()->GetParticleDefinition();
  //      partName = particle->GetParticleName();
  //    }  
  
  
  if (IsMaster()){
    G4cout<<"*******************************************************************************************************************************" << G4endl;
    G4cout<< "\n--------------------End of Global Run-----------------------" << " \n The run was " << nofEvents << " events ";
    //     G4cout<< "\n";
    G4cout<< "\n------------------------------------------------------------\n" ; 
    G4cout<< "\n";
    G4cout<<"Dans la nature, tout a toujours une raison. Si tu comprends cette raison, tu n'as plus besoin de l'expérience. Léonard De Vinci "<<G4endl;
    G4cout<< "\n";
    G4cout<<"*******************************************************************************************************************************" << G4endl;
  }
  else{
    G4cout
    << "\n--------------------End of Local Run------------------------"
    //      << " \n The run was " << nofEvents << " "<< partName;
    << " \n The run was " << nofEvents ;
  }      
  G4cout 
  << "\n------------------------------------------------------------\n"
  << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
