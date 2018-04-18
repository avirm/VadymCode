#include "CathodeSD.hh"
#include "CathodeHit.hh"

#include "G4Track.hh"
#include "G4SystemOfUnits.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CathodeSD::CathodeSD(G4String name)
  : G4VSensitiveDetector(name),fHitsCollection(0)
{
  collectionName.insert("cathodeHits");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CathodeSD::~CathodeSD() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CathodeSD::Initialize(G4HCofThisEvent* hitsCE){
  fHitsCollection = new CathodeHits(SensitiveDetectorName,collectionName[0]);
  //Store collection with event and keep ID
  static G4int hitCID = -1;
  if(hitCID<0){
    hitCID = GetCollectionID(0);
  }
  hitsCE->AddHitsCollection( hitCID, fHitsCollection );
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// G4bool CathodeSD::ProcessHits(G4Step* ,G4TouchableHistory* ){
//   return false;
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//Generates a hit and uses the postStepPoint's mother volume replica number
//PostStepPoint because the hit is generated manually when the photon is
//absorbed by the photocathode

G4bool CathodeSD::ProcessHits(G4Step* aStep, G4TouchableHistory*){ //_constStep

  //need to know if this is an optical photon
  if(aStep->GetTrack()->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  G4double time_local = aStep->GetPostStepPoint()->GetLocalTime();
  G4double time_global = aStep->GetPostStepPoint()->GetGlobalTime();

  CathodeHit* newHit = new CathodeHit();
  newHit->SetLocalTime(time_local);
  newHit->SetGlobalTime(time_global);
  
  fHitsCollection->insert(newHit);

  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CathodeSD::EndOfEvent(G4HCofThisEvent* ) {}
