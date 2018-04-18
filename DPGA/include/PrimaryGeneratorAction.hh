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
// $Id: PrimaryGeneratorAction.hh 68058 2013-03-13 14:47:43Z gcosmo $
//
/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the PrimaryGeneratorAction class

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ParticleGun.hh"


class G4ParticleGun;
class G4GenericMessenger;
class G4Event;
class G4ParticleDefinition;
class G4GeneralParticleSource;
/// The primary generator action class with particle gum.
///
/// A single particle is generated.
/// User can select 
/// - the initial momentum and angle
/// - the momentum and angle spreads
/// - random selection of a particle type from proton, kaon+, pi+, muon+, e+ 

/// It defines an ion (F18), at rest, randomly distribued within a zone 
/// in a patient defined in GeneratePrimaries(). Ion F18 can be changed 
/// with the G4ParticleGun commands (see run2.mac).

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();
    
    virtual void GeneratePrimaries(G4Event*);
    
//     const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
//     
//     void SetMomentum(G4double val) { fMomentum = val; }
//     G4double GetMomentum() const { return fMomentum; }
//     
//     void SetBeamPosition(G4ThreeVector val) { fBeamPosition = val; }
//     G4ThreeVector GetBeamPosition() const { return fBeamPosition; }
// 
//     void SetSigmaMomentum(G4double val) { fSigmaMomentum = val; }
//     G4double GetSigmaMomentum() const { return fSigmaMomentum; }
// 
//     void SetSigmaAngle(G4double val) { fSigmaAngle = val; }
//     G4double GetSigmaAngle() const { return fSigmaAngle; }
// 
//     void SetRandomize(G4bool val) { fRandomizePrimary = val; }
//     G4bool GetRandomize() const { return fRandomizePrimary; }
    
private:
    void DefineCommands();

//     G4ParticleGun* fParticleGun;
    G4GenericMessenger* fMessenger;
    G4GeneralParticleSource* m_particleGun;
    G4ParticleDefinition* particle;
    G4ParticleTable* particleTable;
    G4int Test;
    G4bool fOneTimePrintInfo;
    G4int fParticulesCounter;
    
    G4double fTime_global; // var global for Time
    
    G4int fTimeStructure; // Selection du mode temporel // <= 0 any / 1 radioactive decay / 2 Bunched Beam / 3 MACRO Bunched Time 
    
    G4double fDecayTauPrimmary; // For mode 1 only default 40ns
    
    G4double fBunchPeriod;
    G4double fBunchParticulesTimeDistribution;
    G4int fParticulesPerBunch; // intensity Ampere 
    
    G4String fDrawBunchProgress;
    
//     G4ParticleDefinition* fPositron;
//     G4ParticleDefinition* fMuon;
//     G4ParticleDefinition* fPion;
//     G4ParticleDefinition* fKaon;
//     G4ParticleDefinition* fProton;
//     G4ParticleDefinition* fGamma;
//     G4ParticleDefinition* fChargedgeantino;
//     G4double fMomentum;
//     G4double fSigmaMomentum;
//     G4double fSigmaAngle;
//     G4bool fRandomizePrimary;
//     G4ThreeVector fBeamPosition;
//     G4bool  fbool_Ion; // option ion Na22...
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

