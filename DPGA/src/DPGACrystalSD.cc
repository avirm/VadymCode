// //
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
// $Id: DPGACrystalSD.cc 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file DPGACrystalSD.cc
/// \brief Implementation of the DPGACrystalSD class

#include "DPGACrystalSD.hh"
#include "DPGACrystalHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4HCofThisEvent.hh"
#include "G4SystemOfUnits.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"

#include "G4GenericMessenger.hh"
#include <G4ParticleDefinition.hh>
#include "G4ios.hh"
#include <G4UnitsTable.hh>

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DPGACrystalSD::DPGACrystalSD(G4String name)
: G4VSensitiveDetector(name),fMessenger(0), fHitsCollection(0), fCrystID(-1)
{
  // define commands for this class
  DefineCommands();
  collectionName.insert("CrystalColl");
  //   G4RunManager::GetRunManager()->Initialize();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DPGACrystalSD::~DPGACrystalSD()
{
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DPGACrystalSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection
  = new DPGACrystalHitsCollection(SensitiveDetectorName,collectionName[0]); /// Create hits collection
  if (fCrystID<0)
  { fCrystID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); } /// Get hit collection ID and Add this collection in hitColl
  hce->AddHitsCollection(fCrystID,fHitsCollection);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool DPGACrystalSD::ProcessHits(G4Step*step, G4TouchableHistory* )
{
  //     G4double edep = step->GetTotalEnergyDeposit();
  //     if (edep==0.) return true;
  //
  //     G4TouchableHistory* touchable
  //       = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
  //     G4VPhysicalVolume* physical = touchable->GetVolume();
  //     G4int copyNo = physical->GetCopyNo();
  //
  //     DPGACrystalHit* hit = (*fHitsCollection)[copyNo];
  //     // check if it is first touch
  //     if (!(hit->GetLogV()))
  //     {
  //         // fill volume information
  //         hit->SetLogV(physical->GetLogicalVolume());
  //         G4AffineTransform transform
  //           = touchable->GetHistory()->GetTopTransform();
  //         transform.Invert();
  //         hit->SetRot(transform.NetRotation());
  //         hit->SetPos(transform.NetTranslation());
  //     }
  //     // add energy deposition
  //     hit->AddEdep(edep);
  //
  //     return true;

  //G4cout<<"--- New Hit ---"<<G4endl;
  /// Create hit
  DPGACrystalHit * hit = new DPGACrystalHit();

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
  //G4cout<<"secondaryTrackColl->size() = "<<secondaryTrackColl->size()<<G4endl;
  for(unsigned int sec_i=0 ; sec_i<secondaryTrackColl->size() ; sec_i++)
  {
    G4Track * secPartTrack = secondaryTrackColl->at(sec_i);
    const G4ParticleDefinition * secPartDef = secPartTrack->GetParticleDefinition();
    const G4DynamicParticle * secPartDyn = secPartTrack->GetDynamicParticle();

    if(postStepPoint->GetPosition().getX()==secPartTrack->GetPosition().getX() &&
      postStepPoint->GetPosition().getY()==secPartTrack->GetPosition().getY() &&
      postStepPoint->GetPosition().getZ()==secPartTrack->GetPosition().getZ())
    {
      //         G4cout<<std::setprecision(3)<<secPartDef->GetParticleName()<<" -> "
      //               <<secPartTrack->GetCreatorProcess()->GetProcessName()<<" ("
      // 	      <<secPartTrack->GetPosition().getX()<<","
      // 	      <<secPartTrack->GetPosition().getY()<<","
      // 	      <<secPartTrack->GetPosition().getZ()<<") "
      // 	      <<secPartTrack->GetKineticEnergy()*1000<<" keV, track ID ="
      // 	      <<secPartTrack->GetTrackID()<<G4endl;

      /// Attache secondary info to hit

    }
  }

  G4TouchableHistory* touchable
  = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());

  G4VPhysicalVolume* physical = touchable->GetVolume();
  G4int copyNo = physical->GetCopyNo();

  G4ThreeVector Crystal_Position = touchable -> GetTranslation(0);



  G4int copyNo_PMT_inside;
  G4int copyNo_cassette;

  //     cout << "physical " << touchable->GetVolume()->GetName() << endl;

  if(touchable->GetVolume()->GetName() == "PV_Cryst"){

    G4VPhysicalVolume* PV_WrapCryst = touchable->GetVolume(1);
    copyNo_PMT_inside = PV_WrapCryst->GetCopyNo();

    G4VPhysicalVolume* PV_Cassette_PVC = touchable->GetVolume(3);
    copyNo_cassette = PV_Cassette_PVC->GetCopyNo();
  }
  else{
    copyNo_PMT_inside=-1;
    copyNo_cassette = -1;
  }


  G4ThreeVector localPos = touchable->GetHistory()->GetTopTransform().TransformPoint(postStepPoint->GetPosition());

  G4ThreeVector localPos_Vertex = touchable->GetHistory()->GetTopTransform().TransformPoint(trackVertexPos);

//    cout << "Crystal_Position  " << Crystal_Position << endl;
//   cout << "postStepPoint->GetPosition()  " << postStepPoint->GetPosition() << endl;
//
  //     cout << "localPos " << localPos /cm << endl;

  //     cout << "localPos_Vertex " << localPos_Vertex /cm << endl;

  G4double hitTime = preStepPoint->GetGlobalTime();

  //   G4ParticleDefinition * 	step->GetTrack()->GetDefinition () const

  G4int PDGcodeArt = -1;

  //     if(step->GetTrack()->GetDefinition()->GetAtomicNumber()>1 ){
  //       PDGcodeArt= (step->GetTrack()->GetDefinition()->GetAtomicNumber()*1000 + step->GetTrack()->GetDefinition()->GetAtomicMass())*-1;
  //
  // //       cout << "Element " << step->GetTrack()->GetDefinition()->GetAtomicNumber() << endl;
  //     }
  //     else{

  PDGcodeArt =  step->GetTrack()->GetDynamicParticle()->GetPDGcode();
  //     }

  //     cout << "PDGcode " << PDGcodeArt << endl;

  //     G4int 	GetAtomicNumber () const
  // G4int 	GetAtomicMass () const

  /// Add values to hit
  hit->SetEdep(edep);
  hit->SetEdep_M2(edep_M2);  //save energy in keV

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

  hit->SetCrystal_Position(Crystal_Position);

  hit->SetKineticEnergy(track->GetKineticEnergy());
  hit->SetKineticEnergy_Vertex(track->GetVertexKineticEnergy());


  hit->SetTime(hitTime);

  hit->SetLogV(physical->GetLogicalVolume());

  hit->SetNo_Copy(copyNo_PMT_inside);
  hit->SetNo_Line(copyNo_cassette);
  /// Insert hit to collection
  fHitsCollection->insert(hit);
  return true;


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DPGACrystalSD::DefineCommands()
{
  // Define //generator command directory using generic messenger class
  // useless

  fMessenger
  = new G4GenericMessenger(this,
			   "/B3/CrystalSD/",
			   "CrystalSD control");

  //     // randomizePrimary command
  //     G4GenericMessenger::Command& DPGA_DetectorCmd
  //       = fMessenger->DeclareProperty("DPGA_Detector", fboolDPGA_Detector);
  //     G4String guidance
  //        = " flag for activate tracking of special information\n";
  //     guidance
  //        += "In case this flag is false, it not have error in other geometry\n";
  //     guidance += "  with /gun/particle command.";
  //     DPGA_DetectorCmd.SetGuidance(guidance);
  //     DPGA_DetectorCmd.SetParameterName("flg", true);
  //     DPGA_DetectorCmd.SetDefaultValue("true");
  //     DPGA_DetectorCmd.SetStates(G4State_PreInit,G4State_Idle);
}
