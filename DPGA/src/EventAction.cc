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
// $Id: EventAction.cc 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "EventActionMessenger.hh"

#include "DPGAPatientHit.hh"
#include "DPGACrystalHit.hh"
#include "Analysis.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "G4GenericMessenger.hh"
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4int NoEventgood = 0;
EventAction::EventAction()
: G4UserEventAction(),
fMessenger(0),
fPID(-1),
fCrystID(-1),
fBiasingForSaveData("No")
{
  // define commands for this class
//   DefineCommands();
  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);     
 // define commands for this class
  fMessenger = new EventActionMessenger(this);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
  if (fPID==-1) {
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();
    fPID = sdManager->GetCollectionID("DPGA_Patient/PatientColl");
    fCrystID = sdManager->GetCollectionID("DPGA_Crystal/CrystalColl");
    //       fHHC2ID = sdManager->GetCollectionID("hodoscope2/hodoscopeColl");
  }
}     

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
  G4HCofThisEvent* hce = event->GetHCofThisEvent();
  if (!hce) 
  {
    G4ExceptionDescription msg;
    msg << "No hits collection of this event found.\n"; 
    G4Exception("EventAction::EndOfEventAction()",
		"Code001", JustWarning, msg);
    return;
  }   
  
  
  // Get hits collections 
  DPGAPatientHitsCollection* hPatient 
  = static_cast<DPGAPatientHitsCollection*>(hce->GetHC(fPID));
  
  DPGACrystalHitsCollection* hCrystal 
  = static_cast<DPGACrystalHitsCollection*>(hce->GetHC(fCrystID));
  
  
  if ( (!hPatient) || (!hCrystal)  ) 
  {
    G4ExceptionDescription msg;
    msg << "Some of hits collections of this event not found.\n"; 
    G4Exception("EventAction::EndOfEventAction()",
		"Code001", JustWarning, msg);
    return;
  }   
  G4int No_event = event->GetEventID();
  
  if(No_event%10000 == 0) G4cout << "Event "<< event->GetEventID() << G4endl;
  
  //*************************************************************************************
  //
  // Fill histograms & ntuple
  // 
  //*************************************************************************************
  
  
  //*************************************************************************************
  //****
//    fBiasingForSaveData
  //***
  
  G4bool Biasing_SaveData = false;
//   if(fBiasingForSaveData != "No") SaveData = true;
  
  
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  
  
  
  
  // *******************************************************
  //
  //                    Hit Patient
  //
  // ********************************************************
  
  G4int n_hit = 0;
  
  G4int ComptAnnihl = 0;
  
  // Initialisation Variables
  G4int totalEmHit = 0;
  G4double totalEmE = 0. *MeV;
  G4double totalEmELeft = 0. ;
  G4double totalEmERight = 0. ;
  ClearAllVector(); /// vector
  fNameVolum = "";
  fTotalEdep = 0;
  
  G4String CreationProcessName_Patient_S = "";
  G4String ProcessName_Patient_S ="";
  G4String Separator = ",";
  
  if(fSetup_Detector == ""){ n_hit = hPatient->entries();}
  else{
    n_hit = hPatient->entries();  
    //     cout << "fSetup_Target EventAction " << fSetup_Target << endl;
    //     cout << "fSetup_Detector EventAction " << fSetup_Detector << endl;
    
    for (G4int i=0;i<n_hit;i++){
      DPGAPatientHit* hit = (*hPatient)[i];
      
      G4double eDep = hit->GetEdep();
      
      if (eDep>0.){totalEmHit++;totalEmE += eDep;}
      
      //       cout << "ok" << endl;
      //       const G4TrackVector* SecondaryParticleColl = hit->GetSecondaryParticleColl();
      //         cout << "ok2 SecondaryParticleColl size" << SecondaryParticleColl->size() << endl;
      //       for(G4int sec_i=0 ; sec_i<SecondaryParticleColl->size() ; sec_i++)
      // {
      //   cout << "ok36" << endl;
      //   G4Track * secPartTrack = SecondaryParticleColl->at(sec_i);
      //       
      //      
      //   G4int Secondary = secPartTrack->GetDynamicParticle()->GetPDGcode() ;
      //  cout << "ok" << endl;
      // if(Secondary == -11) cout << "ok" << endl;
      // 
      //   
      //       }
      
      
      G4ThreeVector WorldPos = hit->GetWorldPos();
      fVec_World_PosX.push_back(WorldPos.getX()/mm);
      fVec_World_PosY.push_back(WorldPos.getY()/mm);
      fVec_World_PosZ.push_back(WorldPos.getZ()/mm);
      
      
      analysisManager->FillH1(7, WorldPos.getZ()/cm,eDep);
      
      // Local Pos Doesn't exist now in GEANT44
      //      G4ThreeVector LocalPos = hit->GetLocalPos();
      //       fVec_Local_PosX.push_back(LocalPos.getX);
      //       fVec_Local_PosY.push_back(LocalPos.getY);
      //       fVec_Local_PosZ.push_back(LocalPos.getZ);
      
      // Energy
      // Edep
      fVec_Edep.push_back(eDep /MeV);
      
      // Time 
      //Event
      G4double time = hit->GetTime();
      //      cout << "Save Time " << time /ns << endl;
      fVec_Time_Event.push_back(time /ns);
      //      cout << "Save Time ***" << fVec_Time_Event.back() << endl;
      // Tracking
      // Parent ID
      G4int ParentID = hit->GetParentID();
      fVec_ParentID.push_back(ParentID);
      // Track ID
      G4int TrackID = hit->GetTrackID();
      fVec_TrackID.push_back(TrackID);
      // PDG Code
      G4int PDGCode= hit->GetPDGCode();
      fVec_PDGCode.push_back(PDGCode);
      
      // Process Name
      
      /*
       P rocessName_Patient_S += hit->GetProcessName();                           *
       CreationProcessName_Patient_S += hit->GetCreatorProcessName();
       
       CreationProcessName_Patient_S += Separator ;
       ProcessName_Patient_S += Separator ;
       */
      
      G4String ProcessName_Patient = hit->GetProcessName();
      
      // Save All Process
      
      G4bool bProcessName = 0;
      G4bool bCreationProcessName = 0;
      for(G4int i =0;i< fVec_AllProcessName.size();i++){
	if(hit->GetProcessName() == fVec_AllProcessName[i]){bProcessName =1;}
	if(hit->GetCreatorProcessName() == fVec_AllProcessName[i]){bCreationProcessName =1;}
	
	if(bProcessName == 1 && bCreationProcessName==1 ) break;
	
      }
      if(bProcessName == 0)fVec_AllProcessName.push_back(hit->GetProcessName());
      if(bCreationProcessName == 0)fVec_AllProcessName.push_back(hit->GetCreatorProcessName());
      
      //********************************************************************************
      
      if(PDGCode== -11 && ProcessName_Patient  == "annihil") ComptAnnihl+=1;
      
      G4int ConvertProcessToNumber_tmp = -3;
      ConvertProcessToNumber_tmp = ConvertProcessToNumber(ProcessName_Patient);
      fVec_processName.push_back(ConvertProcessToNumber_tmp);
      
      // Search Unknow process
      if(ConvertProcessToNumber_tmp==-1 ){
	// Find Unknow process for ConvertProcessToNumber
	G4String Temp1 = ProcessName_Patient;
	if(Temp1 != ""){
	  bProcessName = 0; G4String Unknow_Process = "zProcessToImplement "+Temp1;
	  for(G4int i =0;i< fVec_AllProcessName.size();i++){
	    if(Unknow_Process == fVec_AllProcessName[i]){bProcessName =1;}
	    if(bProcessName == 1  ) break;
	  }
	  if(bProcessName == 0)fVec_AllProcessName.push_back(Unknow_Process);
	} 
      }
      //*****************************************
      
      
      // Creation Process name
      G4String CreatorProcessName_Patient = hit->GetCreatorProcessName();
      //G4cout << "Creator Process Name " << CreatorProcessName_Patient << G4endl;
      ConvertProcessToNumber_tmp = -3;
      ConvertProcessToNumber_tmp = ConvertProcessToNumber(CreatorProcessName_Patient);
      fVec_CreationProcessName.push_back(ConvertProcessToNumber_tmp);
      
      // Search Unknow process
      if(ConvertProcessToNumber_tmp==-1 ){
	// Find Unknow process for ConvertProcessToNumber
	G4String Temp1 = CreatorProcessName_Patient;
	if(Temp1 != ""){
	  bProcessName = 0; G4String Unknow_Process = "zProcessToImplement "+Temp1;
	  for(G4int i =0;i< fVec_AllProcessName.size();i++){
	    if(Unknow_Process == fVec_AllProcessName[i]){bProcessName =1;}
	    if(bProcessName == 1  ) break;
	  }
	  if(bProcessName == 0)fVec_AllProcessName.push_back(Unknow_Process);
	} 
      }
      //*****************************************
      
      // Other Informations
      // Vertex Postion Origin Particle 
      
      G4ThreeVector WorldPos_Vertex = hit->GetWorldPos_Vertex();
      
      fVec_Vertex_PosX.push_back(WorldPos_Vertex.getX()/mm);
      fVec_Vertex_PosY.push_back(WorldPos_Vertex.getY()/mm);
      fVec_Vertex_PosZ.push_back(WorldPos_Vertex.getZ()/mm);
      
      
      fVec_Edep_M2.push_back(hit->GetEdep_M2());
      
      fVec_KineticEnergy.push_back(hit->GetKineticEnergy()/MeV);
      fVec_KineticEnergy_Vertex.push_back(hit->GetKineticEnergy_Vertex()/MeV);
      
      
      fVec_Local_PosX.push_back(hit->GetLocalPos().getX()/mm);
      fVec_Local_PosY.push_back(hit->GetLocalPos().getY()/mm);
      fVec_Local_PosZ.push_back(hit->GetLocalPos().getZ()/mm);
      
      fVec_Local_Vertex_PosX.push_back(hit->GetLocalPos_Vertex().getX()/mm);
      fVec_Local_Vertex_PosY.push_back(hit->GetLocalPos_Vertex().getY()/mm);
      fVec_Local_Vertex_PosZ.push_back(hit->GetLocalPos_Vertex().getZ()/mm);
      //       if(PDGCode>1000000000)cout << " hit->GetSecondary_BetaPlus() " << hit->GetSecondary_BetaPlus() << endl;
      
      if (hit->GetSecondary_BetaPlus() >0) Biasing_SaveData = true;
      
      fVec_Secondary_BetaPlus.push_back(hit->GetSecondary_BetaPlus());
      fVec_Secondary_PromptGamma.push_back(hit->GetSecondary_PromptGamma());
      fVec_Secondary_Neutron.push_back(hit->GetSecondary_Neutron());
      
      
      
    } // end for n_hits
    
    //**********************************************************************************************
    
    //      analysisManager->FillNtupleDColumn(0,1, vector);
    //      G4cout <<  " hits. Total Edep is "     << totalEmE/MeV << " MeV" << G4endl;
    
    //    if(ComptAnnihl>0) cout << "Annhil " << ComptAnnihl << "  event " <<  event->GetEventID() << endl;
    
    // Total Edep Event
    analysisManager->FillNtupleDColumn(0,1, totalEmE/MeV);
    
    analysisManager->FillNtupleIColumn(0,0,  event->GetEventID());
    
    analysisManager->FillNtupleSColumn(0,2,fSetup_Target);
    analysisManager->FillNtupleSColumn(0,3,fSetup_Detector);
    
    /*
     a nalysisManager->FillNtupleSColumn(0,4,ProcessName_Patient_S );             *
     analysisManager->FillNtupleSColumn(0,5,CreationProcessName_Patient_S);
     */
    
    
    //      cout <<"Setup DPGA " << fSetup_DdPGA << endl;
    
    //Fill histo
    analysisManager->FillH1(1, totalEmE/MeV);
    analysisManager->FillH1(6, ComptAnnihl);
    
    
    //          for(G4int i=0;i<fVec_AllProcessName.size();i++){cout << fVec_AllProcessName[i] << endl;  }
    
    if(fBiasingForSaveData == "No") analysisManager->AddNtupleRow(0);  
    else if(fBiasingForSaveData == "Only_Event_With_SecondaryBetaPlus" && Biasing_SaveData == true)analysisManager->AddNtupleRow(0);  
    else ;
    
  } // end else Patient 
  //**********************************************************************************************
  //                                 End Hit Patient
  //**********************************************************************************************
  
  // *******************************************************
  //
  //                    Hit  DPGA Crystal
  //
  // ********************************************************   
  
  if(fSetup_Detector == "Without_Detector" || fSetup_Detector == "Scoring_Target"){
    ;
  }
  else{
    n_hit = hCrystal->entries(); 
    // Fill histo
    analysisManager->FillH1(4, n_hit);
    
    // Initialisation Variables
    totalEmHit = 0;
    totalEmE = 0.;
    ClearAllVector(); /// vector
    fNameVolum = "";
    fTotalEdep = 0;
    
    G4String CreationProcessName_Crystal_S = "";
    G4String ProcessName_Crystal_S ="";
    G4String Separator = ",";
    
    
    // Preliminary analyse
    G4int NO_PMT = 240;
    G4double tab_Edep_PM[240]; 
    for(G4int i = 0;i<NO_PMT;i++){tab_Edep_PM[i]=0.0;}
    
    for (G4int i=0;i<n_hit;i++){
      
      DPGACrystalHit* hit_Crystal = (*hCrystal)[i];
      
      G4double eDep = hit_Crystal->GetEdep();
      
      if (eDep>0.){totalEmHit++;totalEmE += eDep;}
      
      G4ThreeVector WorldPos = hit_Crystal->GetWorldPos();
      fVec_World_PosX.push_back(WorldPos.getX()/mm);
      fVec_World_PosY.push_back(WorldPos.getY()/mm);
      fVec_World_PosZ.push_back(WorldPos.getZ()/mm);
      
      
      analysisManager->FillH1(7, WorldPos.getZ()/cm,eDep);
      
      // Local Pos Doesn't exist now in GEANT44
      //      G4ThreeVector LocalPos = hit_Crystal->GetLocalPos();
      //       fVec_Local_PosX.push_back(LocalPos.getX);
      //       fVec_Local_PosY.push_back(LocalPos.getY);
      //       fVec_Local_PosZ.push_back(LocalPos.getZ);
      
      // Energy
      // Edep
      fVec_Edep.push_back(eDep /MeV);
      
      // Time 
      //Event
      G4double time = hit_Crystal->GetTime();
      fVec_Time_Event.push_back(time /ns);
      
      // Tracking
      // Parent ID
      G4int ParentID = hit_Crystal->GetParentID();
      fVec_ParentID.push_back(ParentID);
      // Track ID
      G4int TrackID = hit_Crystal->GetTrackID();
      fVec_TrackID.push_back(TrackID);
      // PDG Code
      G4int PDGCode= hit_Crystal->GetPDGCode();
      fVec_PDGCode.push_back(PDGCode);
      
      // Process Name
      /*
       ProcessName_Crystal_S += hit_Crystal->GetProcessName();                   
       CreationProcessName_Crystal_S += hit_Crystal->GetCreatorProcessName();
       
       CreationProcessName_Crystal_S += Separator ;
       ProcessName_Crystal_S += Separator ;
       */
      
      // Save All Process
      
      G4bool bProcessName = 0;
      G4bool bCreationProcessName = 0;
      for(G4int i =0;i< fVec_AllProcessName.size();i++){
	if(hit_Crystal->GetProcessName() == fVec_AllProcessName[i]){bProcessName =1;}
	if(hit_Crystal->GetCreatorProcessName() == fVec_AllProcessName[i]){bCreationProcessName =1;}
	
	if(bProcessName == 1 && bCreationProcessName==1 ) break;
	
      }
      if(bProcessName == 0)fVec_AllProcessName.push_back(hit_Crystal->GetProcessName());
      if(bCreationProcessName == 0)fVec_AllProcessName.push_back(hit_Crystal->GetCreatorProcessName());
      //*******
      
      
      
      G4String ProcessName_Crystal = hit_Crystal->GetProcessName();
      
      
      G4int ConvertProcessToNumber_tmp = -3;
      ConvertProcessToNumber_tmp = ConvertProcessToNumber(ProcessName_Crystal);
      fVec_processName.push_back(ConvertProcessToNumber_tmp);
      
      // Search Unknow process
      if(ConvertProcessToNumber_tmp==-1 ){
	// Find Unknow process for ConvertProcessToNumber
	G4String Temp1 = ProcessName_Crystal;
	if(Temp1 != ""){
	  bProcessName = 0; G4String Unknow_Process = "zProcessToImplement "+Temp1;
	  for(G4int i =0;i< fVec_AllProcessName.size();i++){
	    if(Unknow_Process == fVec_AllProcessName[i]){bProcessName =1;}
	    if(bProcessName == 1  ) break;
	  }
	  if(bProcessName == 0)fVec_AllProcessName.push_back(Unknow_Process);
	} 
      }
      //*****************************************
      
      // Creation Process name
      G4String CreatorProcessName_Crystal = hit_Crystal->GetCreatorProcessName();
      
      ConvertProcessToNumber_tmp = -3;
      ConvertProcessToNumber_tmp = ConvertProcessToNumber(CreatorProcessName_Crystal);
      fVec_CreationProcessName.push_back(ConvertProcessToNumber_tmp);
      
      // Search Unknow process
      if(ConvertProcessToNumber_tmp==-1 ){
	// Find Unknow process for ConvertProcessToNumber
	G4String Temp1 = CreatorProcessName_Crystal;
	if(Temp1 != ""){
	  bProcessName = 0; G4String Unknow_Process = "zProcessToImplement "+Temp1;
	  for(G4int i =0;i< fVec_AllProcessName.size();i++){
	    if(Unknow_Process == fVec_AllProcessName[i]){bProcessName =1;}
	    if(bProcessName == 1  ) break;
	  }
	  if(bProcessName == 0)fVec_AllProcessName.push_back(Unknow_Process);
	} 
      }
      
      // Other Informations
      // Vertex Postion Origin Particle 
      
      G4ThreeVector WorldPos_Vertex = hit_Crystal->GetWorldPos_Vertex();
      
      fVec_Vertex_PosX.push_back(WorldPos_Vertex.getX()/mm);
      fVec_Vertex_PosY.push_back(WorldPos_Vertex.getY()/mm);
      fVec_Vertex_PosZ.push_back(WorldPos_Vertex.getZ()/mm);
      
      
      fVec_Edep_M2.push_back(hit_Crystal->GetEdep_M2());
      
      fVec_KineticEnergy.push_back(hit_Crystal->GetKineticEnergy()/MeV);
      fVec_KineticEnergy_Vertex.push_back(hit_Crystal->GetKineticEnergy_Vertex()/MeV);
      
      
      fVec_Local_PosX.push_back(hit_Crystal->GetLocalPos().getX()/mm);
      fVec_Local_PosY.push_back(hit_Crystal->GetLocalPos().getY()/mm);
      fVec_Local_PosZ.push_back(hit_Crystal->GetLocalPos().getZ()/mm);
      
      fVec_Local_Vertex_PosX.push_back(hit_Crystal->GetLocalPos_Vertex().getX()/mm);
      fVec_Local_Vertex_PosY.push_back(hit_Crystal->GetLocalPos_Vertex().getY()/mm);
      fVec_Local_Vertex_PosZ.push_back(hit_Crystal->GetLocalPos_Vertex().getZ()/mm);
      
      
      // Info about PMT
      // Number
      G4int NoPMT_copy = hit_Crystal->GetNo_Copy();
      G4int NoCassette = hit_Crystal->GetNo_Line();
      
      // No PMT
      G4int No_PMT = MappingDPGA(NoPMT_copy,NoCassette);
      fVec_No_Copy.push_back(No_PMT); // Pos absolu du crystal 0 - 239
      
     
      fVec_Relative_No_Copy.push_back(MappingDPGA_Relative(NoPMT_copy,NoCassette,"Relative_Crystal")); // Pos relative du crystal 0 -3
      
      // Line
      fVec_No_Line.push_back(NoCassette);
      // Quartet
      fVec_No_Quartet.push_back(NoQuartet(NoPMT_copy,NoCassette));
      
      fVec_Relative_No_Quartet.push_back(MappingDPGA_Relative(NoPMT_copy,NoCassette,"Relative_Quartet"));
      
      
      
      // Left or Right
      fVec_LeftorRight.push_back(LeftOrRightDPGA(NoCassette));
      
      
      // Edep / PMT 
      tab_Edep_PM[No_PMT]+= eDep /MeV;
      
      
      // Get coordinates Crystals
      
      map_fPosition_Crystal[No_PMT]= hit_Crystal->GetCrystal_Position();
      
    } // end for n_hits
    
    
    
    // Preliminary analyse Repartition de edep / PMT
    //*******************************************************************
    G4bool HitsCrystalLeft  = false;
    G4bool HitsCrystalRight = false;
    G4bool HitsCrystalThresholdLeft = false;
    G4bool HitsCrystalThresholdRight = false;
    
    G4bool HitsCrystalThresholdLeft_all = false;
    G4bool HitsCrystalThresholdRight_all = false;
    
    for(G4int i = 0;i<NO_PMT;i++){
      
      
      if(tab_Edep_PM[i]>0){
	// G4cout <<  "tab_Edep_PM[" <<  i << "] = " << tab_Edep_PM[i] << G4endl;
	analysisManager->FillH1(3, tab_Edep_PM[i]);
	
	fVec_Edep_Per_Crystal.push_back(tab_Edep_PM[i]);
	fVec_Edep_Per_Crystal_Crystal_Number.push_back(i);
	
	// Fill vector with Edep / Crystal / EventG4
	
	
	//  G4cout << "tabEnergie " << tab_Edep_PM[i] << G4endl;
	
	if(i < 120){
	  HitsCrystalLeft = true;  
	  totalEmELeft++;
	  if(tab_Edep_PM[i] > 0.465)HitsCrystalThresholdLeft=true;
	}
	if(i >= 120){ 
	  HitsCrystalRight = true;
	  totalEmERight++;
	  if(tab_Edep_PM[i] > 0.465)HitsCrystalThresholdRight=true;
	}
      }
    }
    
    if(totalEmELeft > 0.465)HitsCrystalThresholdLeft_all = true;
    if(totalEmERight > 0.465)HitsCrystalThresholdRight_all = true;
    
    //**********************************************************************
    
    
    //      G4cout << "Event "<< event->GetEventID() <<" hits. Total Edep Crystal is "     << totalEmE/MeV << " MeV" << G4endl;
    // Total Edep Event
    
    //       cout << "totalEmE " << totalEmE << endl;
    
    analysisManager->FillNtupleIColumn(1,0,  event->GetEventID());
    analysisManager->FillNtupleDColumn(1,1, totalEmE /MeV);
    
    /*
     a nalysisManager->FillNtupleSColumn(1,2,ProcessName_Patient_S );             *
     analysisManager->FillNtupleSColumn(1,3,CreationProcessName_Patient_S); 
     */
    
    analysisManager->FillH1(2, totalEmE);
    
    if(HitsCrystalLeft==true && HitsCrystalRight == true ){analysisManager->FillH1(5,1); }
    if(HitsCrystalThresholdLeft==true && HitsCrystalThresholdRight == true )analysisManager->FillH1(5,2);
    if(HitsCrystalThresholdLeft_all==true && HitsCrystalThresholdRight_all == true ) analysisManager->FillH1(5,3);
    if(totalEmE > 0.930)analysisManager->FillH1(5,4);
    
    if(fBiasingForSaveData == "No") analysisManager->AddNtupleRow(1);  
    else if(fBiasingForSaveData == "Only_Event_With_SecondaryBetaPlus" && Biasing_SaveData == true && HitsCrystalLeft==true && HitsCrystalRight == true )analysisManager->AddNtupleRow(1);
    else ;
          
  }
  
  
  //     G4cout << "NoEventgood " << NoEventgood << endl;
  // ->Draw("Event>>(1000,0,1000)")
  //**********************************************************************************************
  //                                 End Hit Crystal
  //**********************************************************************************************
  
  //     //
  //     // Print diagnostics
  //     // 
  //     
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( printModulo==0 || event->GetEventID() % printModulo != 10000) return;
  
  G4PrimaryParticle* primary = event->GetPrimaryVertex(0)->GetPrimary(0);
  G4cout << G4endl
  << ">>> Event " << event->GetEventID() << " >>> Simulation truth : "
  << primary->GetG4code()->GetParticleName()
  << " " << primary->GetMomentum() << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::ClearAllVector(void)
{
  fVec_World_PosX.clear();
  fVec_World_PosY.clear();
  fVec_World_PosZ.clear();
  // Local
  fVec_Local_PosX.clear();
  fVec_Local_PosY.clear();
  fVec_Local_PosZ.clear();
  // Energy
  // Edep
  fVec_Edep.clear();
  // TotalEdp Event
  // Time 
  //Event
  fVec_Time_Event.clear();
  
  // Tracking
  // Parent ID
  fVec_ParentID.clear();
  // Track ID
  fVec_TrackID.clear();
  // PDG Code
  fVec_PDGCode.clear();
  // Process Name
  fVec_processName.clear();
  // Creation Process name
  fVec_CreationProcessName.clear();
  
  // Other Informations
  // Vertex Postion
  fVec_Vertex_PosX.clear();
  fVec_Vertex_PosY.clear();
  fVec_Vertex_PosZ.clear();
  // Logical Volum 
  fVec_No_Copy.clear();
  fVec_Relative_No_Copy.clear();
  
  
  fVec_LeftorRight.clear();
  fVec_No_Quartet.clear(); 
  fVec_No_Line.clear();
  
  fVec_Relative_No_Quartet.clear();
  
  
  
  fVec_Edep_Per_Crystal.clear();
  fVec_Edep_Per_Crystal_Crystal_Number.clear();
  
  
  fVec_Edep_M2.clear();
  
  fVec_KineticEnergy.clear();
  fVec_KineticEnergy_Vertex.clear();
  
  fVec_Local_PosX.clear();
  fVec_Local_PosY.clear();
  fVec_Local_PosZ.clear();
  
  fVec_Local_Vertex_PosX.clear();
  fVec_Local_Vertex_PosY.clear();
  fVec_Local_Vertex_PosZ.clear();
  
  fVec_Secondary_BetaPlus.clear();
  fVec_Secondary_PromptGamma.clear();
  fVec_Secondary_Neutron.clear();
  
}     

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int EventAction::ConvertProcessToNumber(G4String str){
  
  G4int EnigmaNumber = -2;
  
  if(str == "annihil"){EnigmaNumber=0; return EnigmaNumber;} // Def 0 is annihilation
  else if(str == "CoupledTransportation"){EnigmaNumber=1;return EnigmaNumber;}
  else if(str == "Decay"){EnigmaNumber=2;return EnigmaNumber;}
  else if(str == "He3Inelastic"){EnigmaNumber=3;return EnigmaNumber;}
  else if(str == "RadioactiveDecay"){EnigmaNumber=4;return EnigmaNumber;}
  else if(str == "Rayl"){EnigmaNumber=5;return EnigmaNumber;}
  else if(str == "alphaInelastic"){EnigmaNumber=6;return EnigmaNumber;}
  else if(str == "compt"){EnigmaNumber=7;return EnigmaNumber;}
  else if(str == "conv"){EnigmaNumber=8;return EnigmaNumber;}
  else if(str == "dInelastic"){EnigmaNumber=9;return EnigmaNumber;}
  else if(str == "eBrem"){EnigmaNumber=10;return EnigmaNumber;}
  else if(str == "eIoni"){EnigmaNumber=11;return EnigmaNumber;}
  else if(str == "hIoni"){EnigmaNumber=12;return EnigmaNumber;}
  else if(str == "hadElastic"){EnigmaNumber=13;return EnigmaNumber;}
  else if(str == "ionIoni"){EnigmaNumber=14;return EnigmaNumber;}
  else if(str == "msc"){EnigmaNumber=15;return EnigmaNumber;}
  else if(str == "nCapture"){EnigmaNumber=16;return EnigmaNumber;}
  else if(str == "nKiller"){EnigmaNumber=17;return EnigmaNumber;}
  else if(str == "neutronInelastic"){EnigmaNumber=18;return EnigmaNumber;}
  else if(str == "phot"){EnigmaNumber=19;return EnigmaNumber;}
  else if(str == "photonNuclear"){EnigmaNumber=20;return EnigmaNumber;}
  else if(str == "protonInelastic"){EnigmaNumber=21;return EnigmaNumber;}
  else if(str == "tInelastic"){EnigmaNumber=22;return EnigmaNumber;}
  else {EnigmaNumber=-1;}
  
  return EnigmaNumber;
  
}





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int EventAction::MappingDPGA(G4int NoPMT,G4int NoCassette)
{
  G4int Real_NoPMT = -1;
  
  G4int tab[2]={(NoPMT%=100)/10,(NoPMT%=10)};
  
  
  
  G4int RealPosQuartet = -1; // correction de la position dans le quartet -> Pb symétrie
  
  if(NoCassette < 7){ RealPosQuartet = tab[1]; }
  else{
    if(tab[1]==3){RealPosQuartet = 0;}
    else if(tab[1]==2){RealPosQuartet = 1;}
    else if(tab[1]==1){RealPosQuartet = 2;}
    else if(tab[1]==0){RealPosQuartet = 3;}
    else{G4cout << "wrong conversion in MappingDPGA" << G4endl;}
  }
  
  Real_NoPMT = (NoCassette-1)*20 + tab[0]*4+ RealPosQuartet;
  
  if(Real_NoPMT <0){
    
    //    G4cout << "No Ligne                " << NoCassette << G4endl;
    //    G4cout << "tab[0] No Quartet       " << tab[0]+ (NoCassette-1)*5 << G4endl;
    //    G4cout << "tab[1] Position quartet " << tab[1] << G4endl;
    //    G4cout << "Real_NoPMT " << Real_NoPMT << G4endl;
  }
  
  return Real_NoPMT;
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Options : Relative_Crystal or Relative_Quartet
G4int EventAction::MappingDPGA_Relative(G4int NoPMT,G4int NoCassette,G4String QuartetorCrystal) 
{
  G4int Real_NoPMT = -1;
  
  G4int tab[2]={(NoPMT%=100)/10,(NoPMT%=10)};
  
  
  
  G4int RealPosQuartet = -1; // correction de la position dans le quartet -> Pb symétrie
  
  if(NoCassette < 7){ RealPosQuartet = tab[1]; }
  else{
    if(tab[1]==3){RealPosQuartet = 0;}
    else if(tab[1]==2){RealPosQuartet = 1;}
    else if(tab[1]==1){RealPosQuartet = 2;}
    else if(tab[1]==0){RealPosQuartet = 3;}
    else{G4cout << "wrong conversion in MappingDPGA" << G4endl;}
  }
  
  if(QuartetorCrystal == "Relative_Crystal"){ return RealPosQuartet;}
  else if(QuartetorCrystal == "Relative_Quartet"){
    //     G4cout << "tab[0] " << tab[0] << G4endl ;
    return tab[0];
    
  }
  else{G4cout << "Forbidden G4String QuartetorCrystal " << G4endl; return -1;}
  
  
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int EventAction::LeftOrRightDPGA(G4int NoCassette)
{
  G4int LeftOrRight = -1;
  
  if(NoCassette > 6) {LeftOrRight = 1;}
  else {LeftOrRight = 0;}
  
  return LeftOrRight;
  
}
G4int EventAction::NoQuartet(G4int NoPMT,G4int NoCassette)
{
  G4int Real_NoQuartet = -1;
  G4int tab[2]={(NoPMT%=100)/10,(NoPMT%=10)};
  
  Real_NoQuartet = (NoCassette-1)*5 + tab[0];
  //    G4cout << "Real_NoQuartet " << Real_NoQuartet << G4endl;
  return Real_NoQuartet;
  
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void EventAction::DefineCommands()
// {
//   // Define //detector command directory using generic messenger class
//   fMessenger = new G4GenericMessenger(this, 
// 				      "/B3/eventaction/", 
// 				      "Event control");
//   //     // Setup Target command
//   //     G4GenericMessenger::Command& Setup_TargetCmd
//   //       = fMessenger->DeclareProperty("SetupTarget", fSetup_Target, 
//   //                                     "");
//   //     Setup_TargetCmd.SetParameterName("STarget", true);                             
//   //     Setup_TargetCmd.SetDefaultValue("LackofInformationTarget");
//   //     
//   //     
//   //     // Setup DPGA command
//   //     G4GenericMessenger::Command& Setup_DPGACmd
//   //       = fMessenger->DeclareProperty("SetupDPGA", fSetup_DPGA, 
//   //                                     "");
//   //     Setup_DPGACmd.SetParameterName("SDPGA", true);                             
//   //     Setup_DPGACmd.SetDefaultValue("LackofInformationDPGA");
//   
//   
//   //Example  armAngle command
//   //     G4GenericMessenger::Command& armAngleCmd
//   //       = fMessenger->DeclareMethodWithUnit("armAngle","deg",
//   //                                   &DetectorConstruction::SetArmAngle, 
//   //                                   "Set rotation angle of the second arm.");
//   //     armAngleCmd.SetParameterName("angle", true);
//   //     armAngleCmd.SetRange("angle>=0. && angle<180.");
//   //     armAngleCmd.SetDefaultValue("30.");
//   
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......