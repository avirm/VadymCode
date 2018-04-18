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
/// \file B3aEventAction.hh
/// \brief Definition of the B3aEventAction class

#ifndef B3aEventAction_h
#define B3aEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "TrackerHit.hh"
#include <vector>

class B3aRunAction;

/// Event action class
///
/// In EndOfEventAction() there is collected information event per event 
/// from Hits Collections, and accumulated statistic for 
/// B3RunAction::EndOfRunAction().

class B3aEventAction : public G4UserEventAction
{
  public:
    B3aEventAction(B3aRunAction* runAction);
    virtual ~B3aEventAction();

    virtual void  BeginOfEventAction(const G4Event*);
    virtual void    EndOfEventAction(const G4Event*);
    
    std::vector<G4double>& GetLocalTime() {return fLocalTime;};
    std::vector<G4double>& GetGlobalTime() {return fGlobalTime;};
    std::vector<G4double>& GetEdep() {return fEdep;};
    std::vector<G4int>& GetNumOfElectrons() {return fNumOfElectorns;};
    
    void ClearAllVectors();
    
  private:
    B3aRunAction*  fRunAction;
    G4int fCollID_cryst;
    
    std::vector<G4double> fLocalTime;
    std::vector<G4double> fGlobalTime;
    std::vector<G4double> fEdep;
    std::vector<G4int> fNumOfElectorns;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
