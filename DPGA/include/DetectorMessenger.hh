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
/// \file DetectorMessenger.hh
/// \brief Definition of the DetectorMessenger class

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWith3VectorAndUnit;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for B2bDetectorConstruction.
///
/// It implements commands:
/// - /B2/det/setTargetMaterial name
/// - /B2/det/setChamberMaterial name
/// - /B2/det/stepMax value unit

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction*);
    virtual ~DetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    DetectorConstruction*  fDetectorConstruction;

    G4UIdirectory*           fB2Directory;
    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithAString*      fTargMatCmd;
    G4UIcmdWith3VectorAndUnit* fTargetPositionCmd;
    
    G4UIcmdWith3VectorAndUnit* fTarget_Cube_SizeCmd;
    
    // Commands for insert in Target -> cavity //local position  
    G4UIcmdWithAString*        fOptionTargetCmd;
    G4UIcmdWithAString*        fCavityMatCmd;
    G4UIcmdWith3VectorAndUnit* fCavityPositionCmd;
    G4UIcmdWith3VectorAndUnit* fCavity_SizeCmd;
    
    // World
    // Selector of World
    G4UIcmdWithAString*      fWorldSelectorCmd;
    /// to Tube world
    G4UIcmdWithADoubleAndUnit* fLenghtWorldTubeCmd;
    G4UIcmdWithADoubleAndUnit* fRadiusWorldTubeCmd; 
    /// For Cube world
    G4UIcmdWith3VectorAndUnit* fWorld_Cube_SizeCmd;
    
     
    
    G4UIcmdWithAString*      fDetectorSetupCmd;
    G4UIcmdWithAString*      fTargetSetupCmd;
    
    G4UIcmdWithADoubleAndUnit* fLenghtCylinder_DetectorCmd;
    G4UIcmdWithADoubleAndUnit* fLenghtCylinder_TargetCmd;
    G4UIcmdWithADoubleAndUnit* fRadiusCylinder_DetectorCmd;
    G4UIcmdWithADoubleAndUnit* fRadiusCylinder_TargetCmd;
    
    //     G4UIcmdWithAString*      fChamMatCmd;
// 
//     G4UIcmdWithADoubleAndUnit* fStepMaxCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
