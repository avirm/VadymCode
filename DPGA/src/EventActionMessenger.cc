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
/// \file EventActionMessenger.cc
/// \brief Implementation of the EventAction class

#include "EventActionMessenger.hh"
#include "EventAction.hh"
#include "G4ThreeVector.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
// #include <iostream.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventActionMessenger::EventActionMessenger(EventAction* Det)
: G4UImessenger(),
fEventAction(Det)
{
  fB2Directory = new G4UIdirectory("/B3/");
  fB2Directory->SetGuidance("UI commands specific to this example.");
  
  fDetDirectory = new G4UIdirectory("/B3/EventAction/");
  fDetDirectory->SetGuidance("EventAction control");
  
  fBiasingForSaveDataCmd = new G4UIcmdWithAString("/B3/EventAction/BiasingForSaveData",this);
  fBiasingForSaveDataCmd->SetGuidance("Select Mode For Save Only some Event Default none.");
  fBiasingForSaveDataCmd->SetParameterName("choice",false);
  fBiasingForSaveDataCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
 
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventActionMessenger::~EventActionMessenger()
{
  delete fBiasingForSaveDataCmd;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  
  // Basic command Target Position and Material
  if(command == fBiasingForSaveDataCmd ){ fEventAction->SetBiasingForSaveData(newValue);}
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
