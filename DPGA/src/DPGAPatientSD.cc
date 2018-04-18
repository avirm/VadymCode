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
// $Id: DPGAPatientSD.cc 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file DPGAPatientSD.cc
/// \brief Implementation of the DPGAPatientSD class

#include "DPGAPatientSD.hh"
#include "DPGAPatientHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include <G4ParticleDefinition.hh>
#include "G4PrimaryParticle.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DPGAPatientSD::DPGAPatientSD(G4String name)
: G4VSensitiveDetector(name), fHitsCollection(0), fPID(-1)
{
  collectionName.insert("PatientColl");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DPGAPatientSD::~DPGAPatientSD()
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DPGAPatientSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection 
  = new DPGAPatientHitsCollection(SensitiveDetectorName,collectionName[0]); /// Create hits collection
  if (fPID<0)
  { fPID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); } /// Get hit collection ID and Add this collection in hitColl
  hce->AddHitsCollection(fPID,fHitsCollection);
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DPGAPatientSD::ProcessHits(G4Step*step, G4TouchableHistory*)
{
  
  //G4cout<<"--- New Hit ---"<<G4endl;
  
  /// Create hit 
  DPGAPatientHit * hit = new DPGAPatientHit();
  
  /// Get post-step point 
  G4StepPoint * postStepPoint = step->GetPostStepPoint();
  
  /// Get pre-step point 
  G4StepPoint * preStepPoint = step->GetPreStepPoint();
  
  /// Get track of hit
  G4Track * track = step->GetTrack();
  /*G4cout<<"track pos = ("
   *	<<track->GetPosition().getX()<<","
   *	<<track->GetPosition().getY()<<","
   *	<<track->GetPosition().getZ()<<"), "
   *	<<track->GetParticleDefinition()->GetParticleName()<<G4endl;*/
  
  //G4cout<<"track step nb = "<<track->GetCurrentStepNumber()<<G4endl;
  
  /// Get energy deposit (loss by particle)
  // 2 Methods : 1 use GetTotalEnergyDeposit /!\ sum doesn't count secondary
  //             2 use E preStepPoint - E postStepPoint /!\ local approach ie all ionising particules depose energy in the same area of interaction 
  
  G4double edep = step->GetTotalEnergyDeposit();
  
  // Same as GetDeltaEnergy
  G4double edep_M2 = preStepPoint->GetKineticEnergy() - postStepPoint->GetKineticEnergy();  
  
  
  /// Get particle info that create hit
  G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
  
  /// Get process name
  const G4VProcess * process = postStepPoint->GetProcessDefinedStep();
  G4String processName = process->GetProcessName();
  
  /// Get parent process name
  const G4VProcess * parentProcess = postStepPoint->GetProcessDefinedStep();
  G4String parentProcessName = ((parentProcess != NULL) ? parentProcess->GetProcessName() : G4String());
  
  /// Get track creator process name
  const G4VProcess * creatorProcess = track->GetCreatorProcess();;
  G4String creatorProcessName = ((creatorProcess != NULL) ? creatorProcess->GetProcessName() : G4String());
  
  /// Get track vertex position
  G4ThreeVector trackVertexPos = track->GetVertexPosition();
  
  /// Get secondary tracks
  const G4TrackVector * secondaryTrackColl = step->GetSecondary();
  
  //   G4cout<<"secondaryTrackColl = "<<secondaryTrackColl<<G4endl;
  //   
  //   G4cout<<"secondaryTrackColl->size() = "<<secondaryTrackColl->size()<<G4endl;
  
  G4int Secondary_BetaPlus =0;
  G4int Secondary_PromptGamma=0;
  G4int Secondary_Neutron= 0;
  
  for(unsigned int sec_i=0 ; sec_i<secondaryTrackColl->size() ; sec_i++)
  {
    G4Track * secPartTrack = secondaryTrackColl->at(sec_i);
    const G4ParticleDefinition * secPartDef = secPartTrack->GetParticleDefinition();
    const G4DynamicParticle * secPartDyn = secPartTrack->GetDynamicParticle();
    
    
    G4int Secondary = secPartTrack->GetDynamicParticle()->GetPDGcode() ;
    
    if(Secondary == -11) Secondary_BetaPlus+=1;
    if(Secondary == 22 &&  step->GetTrack()->GetDynamicParticle()->GetPDGcode() != 11 )Secondary_PromptGamma +=1;
    if(Secondary == 2112)Secondary_Neutron+=1;
    
    if(postStepPoint->GetPosition().getX()==secPartTrack->GetPosition().getX() &&
      postStepPoint->GetPosition().getY()==secPartTrack->GetPosition().getY() &&
      postStepPoint->GetPosition().getZ()==secPartTrack->GetPosition().getZ())
    {
      
      
      //        G4cout<<std::setprecision(3)<<secPartDef->GetParticleName()<<" -> "
      //               <<secPartTrack->GetCreatorProcess()->GetProcessName()<<" ("
      // 	      <<secPartTrack->GetPosition().getX()<<","
      // 	      <<secPartTrack->GetPosition().getY()<<","
      // 	      <<secPartTrack->GetPosition().getZ()<<") "
      // 	      <<secPartTrack->GetKineticEnergy()*1000<<" keV, track ID ="
      // 	      <<secPartTrack->GetTrackID()<<G4endl;
      
      /// Attache secondary info to hit
      
    }
  }
  
  if(Secondary_BetaPlus>1) cout << "NbrSecondaryBeta " << Secondary_BetaPlus << endl;
  G4TouchableHistory* touchable
  = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  G4VPhysicalVolume* physical = touchable->GetVolume();
  G4int copyNo = physical->GetCopyNo();
  
  //     G4cout << "CopyNumber " << copyNo << G4endl;
  //     G4cout << "physical name  " << physical->GetName() << G4endl;
  
  G4double hitTime = preStepPoint->GetGlobalTime();
  
  G4ThreeVector localPos = touchable->GetHistory()->GetTopTransform().TransformPoint(postStepPoint->GetPosition());
  
  G4ThreeVector localPos_Vertex = touchable->GetHistory()->GetTopTransform().TransformPoint(trackVertexPos);
  
  //     cout << "localPos " << localPos /cm << endl;
  
  //     cout << "localPos_Vertex " << localPos_Vertex /cm << endl;
  
  G4int PDGcodeArt = -1;
  PDGcodeArt =  step->GetTrack()->GetDynamicParticle()->GetPDGcode();
  
  
  
  if(PDGcodeArt == -11 ){ 
    // 	  G4PrimaryParticle* PrimaryParticle = track->GetDynamicParticle()->GetPrimaryParticle();
    //          G4double momentumx = PrimaryParticle->GetMomentum().x();
    //          G4double primaryy = PrimaryParticle->GetVertexPosition().y();
    //          G4double primaryz = PrimaryParticle->GetVertexPosition().z();
    
    // 	 G4int PDGcodeParent = step->GetTrack()->GetDynamicParticle()->GetPrimaryParticle()->GetTrackID();
    // 	 
    // 	 cout << "Parent " << momentumx << "  PDGCode " << PDGcodeArt << endl; 
    
    
  }
  //     if(step->GetTrack()->GetDefinition()->GetAtomicNumber()>1 ){
  // // //       PDGcodeArt= (step->GetTrack()->GetDefinition()->GetAtomicNumber()*1000 + step->GetTrack()->GetDefinition()->GetAtomicMass())*-1;
  // //     
  //       cout << "Element " << step->GetTrack()->GetDefinition()->GetAtomicNumber() << endl;
  //       cout << "PDGCode " << step->GetTrack()->GetDynamicParticle()->GetPDGcode() << endl;
  //     }
  //     else{
  //       
  //       PDGcodeArt =  step->GetTrack()->GetDynamicParticle()->GetPDGcode();
  //     }
  //     
  //     cout << "PDGcode " << PDGcodeArt << endl;
  
  // 	 if(PDGcodeArt == -11 &&  processName == "annihil") { cout << "GetKineticEnergy B+ " <<  track->GetKineticEnergy()*1000<<" keV" << endl;
  // // 	 cout << " Position B+ X Y Z" << postStepPoint->GetPosition().getX() << postStepPoint->GetPosition().getY() <<postStepPoint->GetPosition().getZ() << endl;
  // 	 
  // 	   cout << "Process name " << processName << endl;
  // 	}
  // // 	  /*track->GetKineticEnergy()==0*/
  // 	 if(PDGcodeArt == 22   ){  cout << "GetKineticEnergy gamma " <<  track->GetKineticEnergy()*1000<<" keV" << endl;
  // 	   cout << " parentProcessName " << parentProcessName << endl;
  // 	   
  // 	}
  // 	   cout << "GetKineticEnergy + edep gamma " <<  (track->GetKineticEnergy()+ edep )*1000<<" keV" << endl;
  // 	    cout << "Process name " << processName << endl;
  // 	   cout << "GetVertexKineticEnergy () " <<  (track->GetVertexKineticEnergy())*1000<<" keV" << endl;
  // 	   
  // 	   
  // // 	   cout << "vertex postion gamma X Y Z " << trackVertexPos.getX() << " " << trackVertexPos.getY() << " " << trackVertexPos.getZ() << endl;
  // 	   
  // 	   
  // 	}
  
  /// Add values to hit
  hit->SetEdep(edep);
  hit->SetEdep_M2(edep_M2);
  
  hit->SetParentID(step->GetTrack()->GetParentID());
  hit->SetTrackID(step->GetTrack()->GetTrackID());
  hit->SetPDGCode(PDGcodeArt);
  hit->SetProcessName(processName);
  hit->SetParentProcessName(parentProcessName);
  hit->SetCreatorProcessName(creatorProcessName);
  
  hit->SetParticleName(particleName);
  
  hit->SetWorldPos(postStepPoint->GetPosition());
  hit->SetLocalPos(localPos);
  hit->SetWorldPos_Vertex(trackVertexPos);
  hit->SetLocalPos_Vertex(localPos_Vertex);
  
  hit->SetKineticEnergy(track->GetKineticEnergy());
  hit->SetKineticEnergy_Vertex(track->GetVertexKineticEnergy());
  
  
  hit->SetTime(hitTime);
  
  hit->SetLogV(physical->GetLogicalVolume());
  
  //     G4cout<<"secondaryTrackColl = "<<secondaryTrackColl<<G4endl;
  //   
  //   G4cout<<"secondaryTrackColl->size() = "<<secondaryTrackColl->size()<<G4endl;
  
  hit->SetSecondary_BetaPlus(Secondary_BetaPlus);
  hit->SetSecondary_PromptGamma(Secondary_PromptGamma);
  hit->SetSecondary_Neutron(Secondary_Neutron);
  
  
  //   hit->SetSecondaryParticle(secondaryTrackColl);
  
  
  
  /// Insert hit to collection
  fHitsCollection->insert(hit);
  return true;
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
