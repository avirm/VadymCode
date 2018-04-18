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
// $Id: EventAction.hh 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

 #include <map>
#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4ThreeVector.hh"
#include <vector> 

class G4GenericMessenger;
class EventActionMessenger;
/// Event action

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    virtual ~EventAction();
    
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);
    virtual void ClearAllVector(void);
    
    
    virtual G4int MappingDPGA(G4int NoPMT,G4int NoCassette);
    virtual G4int MappingDPGA_Relative(G4int NoPMT,G4int NoCassette,G4String QuartetorCrystal); // Options : Relative_Crystal or Relative_Quartet
    virtual G4int LeftOrRightDPGA(G4int NoCassette); // if PMT is in left part -> 0 if in right part -> 1
    virtual G4int NoQuartet(G4int NoPMT,G4int NoCassette);
    
    virtual G4int ConvertProcessToNumber(G4String str); // To convert Process Mise à jour 09/17
    
    void SetBiasingForSaveData(G4String val){fBiasingForSaveData =val;};
   
    
    
   //******************************************************************************************************** 
    // Method for Vector ONLY !!!!
     // Postion
     // World
    std::vector<G4double>& GetVec_World_PosX() { return fVec_World_PosX; }
    std::vector<G4double>& GetVec_World_PosY() { return fVec_World_PosY; }
    std::vector<G4double>& GetVec_World_PosZ() { return fVec_World_PosZ; }
    
    // Energy
    // Edep
     std::vector<G4double>& GetVec_Edep() { return fVec_Edep; }
    // TotalEdp Event
    // Time 
    //Event
    std::vector<G4double>& GetVec_Time_Event() { return fVec_Time_Event; }
   // Tracking
     // Parent ID
     std::vector<G4int>& GetVec_ParentID() { return fVec_ParentID; }
     // Track ID
     std::vector<G4int>& GetVec_TrackID() { return fVec_TrackID; }
     // PDG Code
     std::vector<G4int>& GetVec_PDGCode() { return fVec_PDGCode; }
     // Process Name
     std::vector<G4int>& GetVec_processName() { return fVec_processName; }
     // Creation Process name
     std::vector<G4int>& GetVec_CreationProcessName() { return fVec_CreationProcessName; }
     
   // Other Informations
     // Vertex Postion
     std::vector<G4double>& GetVec_Vertex_PosX() { return fVec_Vertex_PosX; }
     std::vector<G4double>& GetVec_Vertex_PosY() { return fVec_Vertex_PosY; }
     std::vector<G4double>& GetVec_Vertex_PosZ() { return fVec_Vertex_PosZ; }

     // Logical Volum 
      
      std::vector<G4int>& GetVec_No_Quartet() { return fVec_No_Quartet; }
      std::vector<G4int>& GetVec_No_Line() { return fVec_No_Line; }
      
      std::vector<G4int>& GetVec_Relative_No_Quartet() { return fVec_Relative_No_Quartet; }
      
      std::vector<G4int>& GetVec_No_Copy() { return fVec_No_Copy; }
      std::vector<G4int>& GetVec_Relative_No_Copy() { return fVec_Relative_No_Copy; }
      
      
      
      std::vector<G4int>& GetVec_LeftOrRight() { return fVec_LeftorRight; }
      
      // Edep / PMT 
      std::vector<G4double>& GetVec_Edep_Per_Crystal() { return fVec_Edep_Per_Crystal; }
      
      std::vector<G4int>& GetVec_Edep_Per_Crystal_Crystal_Number() { return fVec_Edep_Per_Crystal_Crystal_Number; }
      
      
      std::vector<G4double>& GetVec_Edep_M2() { return fVec_Edep_M2; }
      
      std::vector<G4double>& GetVec_KineticEnergy() { return fVec_KineticEnergy; }
      std::vector<G4double>& GetVec_KineticEnergy_Vertex() { return fVec_KineticEnergy_Vertex; }
      
      std::vector<G4double>& GetVec_Local_PosX() { return fVec_Local_PosX; }
      std::vector<G4double>& GetVec_Local_PosY() { return fVec_Local_PosY; }
      std::vector<G4double>& GetVec_Local_PosZ() { return fVec_Local_PosZ; }
      
      std::vector<G4double>& GetVec_Vertex_Local_PosX() { return fVec_Local_Vertex_PosX; }
      std::vector<G4double>& GetVec_Vertex_Local_PosY() { return fVec_Local_Vertex_PosY; }
      std::vector<G4double>& GetVec_Vertex_Local_PosZ() { return fVec_Local_Vertex_PosZ; }
       
       std::vector<G4int>& GetVec_Secondary_BetaPlus() { return fVec_Secondary_BetaPlus; }
       std::vector<G4int>& GetVec_Secondary_PromptGamma() { return fVec_Secondary_PromptGamma; }
       std::vector<G4int>& GetVec_Secondary_Neutron() { return fVec_Secondary_Neutron; }
       
       std::vector<G4String>& GetVec_AllProcessName() { return fVec_AllProcessName; }
       
       // map_fPosition_Crystal
       
       std::map<G4int, G4ThreeVector>& Getmap_fPosition_Crystal() { return map_fPosition_Crystal; }
       
      
     void SetTargetSetup( G4String Config) { fSetup_Target = Config; }
     void SetDetectorSetup( G4String Config) { fSetup_Detector = Config; }
     
//      void SetHistId(G4int *tab){*fHistId = *tab;}

     
     //******************************************************************************************************** 
    
private:
    G4int fPID;
    G4int fCrystID;
//      std::vector<G4double> vector;
    
//     void DefineCommands();
    
    
    G4String fBiasingForSaveData;
    
    G4String fSetup_Target ;
    G4String fSetup_Detector;
    
    
    
    // Postion
     // World
     std::vector<G4double> fVec_World_PosX;
     std::vector<G4double> fVec_World_PosY;
     std::vector<G4double> fVec_World_PosZ;
 
    // Energy
     // Edep
     std::vector<G4double> fVec_Edep;
     // TotalEdp Event
     G4double fTotalEdep ;
    // Time 
     //Event
     std::vector<G4double> fVec_Time_Event;
     
   // Tracking
     // Parent ID
      std::vector<G4int> fVec_ParentID;
     // Track ID
     std::vector<G4int> fVec_TrackID;
     // PDG Code
     std::vector<G4int> fVec_PDGCode;
     // Process Name
     std::vector<G4int> fVec_processName;
     // Creation Process name
     std::vector<G4int> fVec_CreationProcessName;
     
   // Other Informations
     // Vertex Postion
     std::vector<G4double> fVec_Vertex_PosX;
     std::vector<G4double> fVec_Vertex_PosY;
     std::vector<G4double> fVec_Vertex_PosZ;
     // Logical Volum 
     G4String fNameVolum ;
     std::vector<G4int>  fVec_No_Copy; // PMT number with DPGA mapping
      std::vector<G4int> fVec_Relative_No_Copy; // PMT number without DPGA mapping
     
     
     std::vector<G4int> fVec_LeftorRight; // Left or Right part PMT
     std::vector<G4int>  fVec_No_Quartet;
     std::vector<G4int>  fVec_No_Line;
     
     std::vector<G4int>  fVec_Relative_No_Quartet;
     
     
   
    // Edep / PMT 
     
     std::vector<G4double>  fVec_Edep_Per_Crystal;
     std::vector<G4int>  fVec_Edep_Per_Crystal_Crystal_Number;
     
     
     // 
      // Additional info : 
     
     // Edep Method 2
     
	std::vector<G4double>  fVec_Edep_M2;
      
	std::vector<G4double>  fVec_KineticEnergy;
        std::vector<G4double>  fVec_KineticEnergy_Vertex;
	
	 std::vector<G4double>  fVec_Local_PosX;
	 std::vector<G4double>  fVec_Local_PosY;
	 std::vector<G4double>  fVec_Local_PosZ;
	   
	 std::vector<G4double>  fVec_Local_Vertex_PosX;
	 std::vector<G4double>  fVec_Local_Vertex_PosY;
	 std::vector<G4double>  fVec_Local_Vertex_PosZ;
     
	 // Attach secondary info to a hit B+ Prompt gamma neutrons
	  //  Number of B, PG, n
	 std::vector<G4int>  fVec_Secondary_BetaPlus;
	 std::vector<G4int>  fVec_Secondary_PromptGamma;
	 std::vector<G4int>  fVec_Secondary_Neutron;
	 
	 
	 // To get and print all the process name
	 std::vector<G4String>  fVec_AllProcessName;
        
	  std::map <G4int /*NoPMT*/, G4ThreeVector /*Crystal_Position*/ > map_fPosition_Crystal;
	 
//     G4int  *fHistId; 

     
     EventActionMessenger* fMessenger;
     G4String fSetup_DPGA;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
