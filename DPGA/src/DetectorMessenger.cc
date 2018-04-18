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
/// \file DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "G4ThreeVector.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
// #include <iostream.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
: G4UImessenger(),
fDetectorConstruction(Det)
{
  fB2Directory = new G4UIdirectory("/B3/");
  fB2Directory->SetGuidance("UI commands specific to this example.");
  
  fDetDirectory = new G4UIdirectory("/B3/detector/");
  fDetDirectory->SetGuidance("Detector construction control");
  
  fTargMatCmd = new G4UIcmdWithAString("/B3/detector/setTargetMaterial",this);
  fTargMatCmd->SetGuidance("Select Material of the Target.");
  fTargMatCmd->SetParameterName("choice",false);
  fTargMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fTargetPositionCmd = new G4UIcmdWith3VectorAndUnit("/B3/detector/TargetPosition",this);
  fTargetPositionCmd->SetGuidance("Select Position of the Target");
  fTargetPositionCmd->SetGuidance("Position needs not to be a unit vector! ");
  fTargetPositionCmd->SetParameterName("X","Y","Z",true,true);
  //   fTargetPositionCmd->SetRange("X>=0 && Y >=0 && Z>=0");
  fTargetPositionCmd->SetDefaultUnit("cm");
  fTargetPositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fTarget_Cube_SizeCmd = new G4UIcmdWith3VectorAndUnit("/B3/detector/TargetFullSize",this);
  fTarget_Cube_SizeCmd->SetGuidance("Select Full Size of the Target");
  fTarget_Cube_SizeCmd->SetGuidance("Size needs not to be a unit vector! ");
  fTarget_Cube_SizeCmd->SetParameterName("X","Y","Z",true,true);
  fTarget_Cube_SizeCmd->SetRange("X>=0 && Y >=0 && Z>=0");
  fTarget_Cube_SizeCmd->SetDefaultUnit("cm");
  fTarget_Cube_SizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  // Cavity Commands
  
  fOptionTargetCmd = new G4UIcmdWithAString("/B3/detector/OptionTarget",this);
  fOptionTargetCmd->SetGuidance("Select option of the Target.");
  fOptionTargetCmd->SetParameterName("choice",false);
  fOptionTargetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fCavityMatCmd = new G4UIcmdWithAString("/B3/detector/setCavityTargetMaterial",this);
  fCavityMatCmd->SetGuidance("Select Material of Cavity Target.");
  fCavityMatCmd->SetParameterName("choice",false);
  fCavityMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fCavityPositionCmd = new G4UIcmdWith3VectorAndUnit("/B3/detector/CavityTargetPosition",this);
  fCavityPositionCmd->SetGuidance("Select Position of the cavity Target");
  fCavityPositionCmd->SetGuidance("Position needs not to be a unit vector! ");
  fCavityPositionCmd->SetParameterName("X","Y","Z",true,true);
  fCavityPositionCmd->SetDefaultUnit("cm");
  fCavityPositionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fCavity_SizeCmd = new G4UIcmdWith3VectorAndUnit("/B3/detector/CavityTargetFullSize",this);
  fCavity_SizeCmd->SetGuidance("Select Full Size of the Cavity Target");
  fCavity_SizeCmd->SetGuidance("Size needs not to be a unit vector! ");
  fCavity_SizeCmd->SetParameterName("X","Y","Z",true,true);
  fCavity_SizeCmd->SetRange("X>=0 && Y >=0 && Z>=0");
  fCavity_SizeCmd->SetDefaultUnit("cm");
  fCavity_SizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  // World Commands
  
  fWorldSelectorCmd = new G4UIcmdWithAString("/B3/detector/setWorld",this);
  fWorldSelectorCmd->SetGuidance("Select a type of World.");
  fWorldSelectorCmd->SetParameterName("choice",false);
  fWorldSelectorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fLenghtWorldTubeCmd = new G4UIcmdWithADoubleAndUnit("/B3/detector/LenghtCylinder_World",this);
  fLenghtWorldTubeCmd->SetGuidance("Define a LenghtCylinder_World (>0) Default unit -> cm");
  fLenghtWorldTubeCmd->SetParameterName("LenghtCylinder_Target",false);
  fLenghtWorldTubeCmd->SetUnitCategory("Length");
  fLenghtWorldTubeCmd->SetRange("LenghtCylinder_Target > 0");
  fLenghtWorldTubeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fLenghtWorldTubeCmd->SetDefaultUnit("cm");
  
  fRadiusWorldTubeCmd = new G4UIcmdWithADoubleAndUnit("/B3/detector/RadiusCylinder_World",this);
  fRadiusWorldTubeCmd->SetGuidance("Define a RadiusCylinder_World (>0) Default unit -> cm");
  fRadiusWorldTubeCmd->SetParameterName("LenghtCylinder_Target",false);
  fRadiusWorldTubeCmd->SetUnitCategory("Length");
  fRadiusWorldTubeCmd->SetRange("LenghtCylinder_Target > 0");
  fRadiusWorldTubeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fRadiusWorldTubeCmd->SetDefaultUnit("cm");
  
  fWorld_Cube_SizeCmd = new G4UIcmdWith3VectorAndUnit("/B3/detector/WorldCubeFullSize",this);
  fWorld_Cube_SizeCmd->SetGuidance("Select Full Size of the World");
  fWorld_Cube_SizeCmd->SetGuidance("Size needs not to be a unit vector! ");
  fWorld_Cube_SizeCmd->SetParameterName("X","Y","Z",true,true);
  fWorld_Cube_SizeCmd->SetRange("X>=0 && Y >=0 && Z>=0");
  fWorld_Cube_SizeCmd->SetDefaultUnit("cm");
  fWorld_Cube_SizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  fDetectorSetupCmd = new G4UIcmdWithAString("/B3/detector/setDetectorSetup",this);
  fDetectorSetupCmd->SetGuidance("Select Detector Setup. Example : Full_DPGA Detector_Cylinder ");
  fDetectorSetupCmd->SetParameterName("choice",false);
  fDetectorSetupCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  fTargetSetupCmd = new G4UIcmdWithAString("/B3/detector/setTargetSetup",this);
  fTargetSetupCmd->SetGuidance("Select Target Setup Example : Target_Cylinder Target_Cube ");
  fTargetSetupCmd->SetParameterName("choice",false);
  fTargetSetupCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  
  fLenghtCylinder_DetectorCmd = new G4UIcmdWithADoubleAndUnit("/B3/detector/LenghtCylinder_Detector",this);
  fLenghtCylinder_DetectorCmd->SetGuidance("Define a LenghtCylinder_Detector (>0) Default unit -> cm");
  fLenghtCylinder_DetectorCmd->SetParameterName("LenghtCylinder_Detector",false);
  fLenghtCylinder_DetectorCmd->SetUnitCategory("Length");
  fLenghtCylinder_DetectorCmd->SetRange("LenghtCylinder_Detector > 0");
  fLenghtCylinder_DetectorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fLenghtCylinder_DetectorCmd->SetDefaultUnit("cm");
  
  fLenghtCylinder_TargetCmd = new G4UIcmdWithADoubleAndUnit("/B3/detector/LenghtCylinder_Target",this);
  fLenghtCylinder_TargetCmd->SetGuidance("Define a LenghtCylinder_Target (>0) Default unit -> cm");
  fLenghtCylinder_TargetCmd->SetParameterName("LenghtCylinder_Target",false);
  fLenghtCylinder_TargetCmd->SetUnitCategory("Length");
  fLenghtCylinder_TargetCmd->SetRange("LenghtCylinder_Target > 0");
  fLenghtCylinder_TargetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fLenghtCylinder_TargetCmd->SetDefaultUnit("cm");
  
  
  fRadiusCylinder_DetectorCmd = new G4UIcmdWithADoubleAndUnit("/B3/detector/RadiusCylinder_Detector",this);
  fRadiusCylinder_DetectorCmd->SetGuidance("Define a RadiusCylinder_Detector (>0) Default unit -> cm");
  fRadiusCylinder_DetectorCmd->SetParameterName("RadiusCylinder_Detector",false);
  fRadiusCylinder_DetectorCmd->SetUnitCategory("Length");
  fRadiusCylinder_DetectorCmd->SetRange("RadiusCylinder_Detector > 0");
  fRadiusCylinder_DetectorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fRadiusCylinder_DetectorCmd->SetDefaultUnit("cm");
  
  fRadiusCylinder_TargetCmd = new G4UIcmdWithADoubleAndUnit("/B3/detector/RadiusCylinder_Target",this);
  fRadiusCylinder_TargetCmd->SetGuidance("Define a RadiusCylinder_Target (>0) Default unit -> cm");
  fRadiusCylinder_TargetCmd->SetParameterName("RadiusCylinder_Target",false);
  fRadiusCylinder_TargetCmd->SetUnitCategory("Length");
  fRadiusCylinder_TargetCmd->SetRange("RadiusCylinder_Target > 0");
  fRadiusCylinder_TargetCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  fRadiusCylinder_TargetCmd->SetDefaultUnit("cm");
  
  
  /*  
   *      G4UIcmdWithAString*      fDetectorSetup;
   *    G4UIcmdWithAString*      fTargetSetup;
   *    
   *    G4UIcmdWithADoubleAndUnit* fLenghtCylinder_Detector;
   *    G4UIcmdWithADoubleAndUnit* fLenghtCylinder_Target;
   *    
   *    G4UIcmdWithADoubleAndUnit* fRadiusCylinder_Detector;
   *    G4UIcmdWithADoubleAndUnit* fRadiusCylinder_Target;
   *    
   *    
   *     G4UIcmdWithAString*      fDetectorSetupCmd;
   *    G4UIcmdWithAString*      fTargetSetupCmd;
   *    
   *    G4UIcmdWithADoubleAndUnit* fLenghtCylinder_DetectorCmd;
   *    G4UIcmdWithADoubleAndUnit* fLenghtCylinder_TargetCmd;
   *    
   *    G4UIcmdWithADoubleAndUnit* fRadiusCylinder_DetectorCmd;
   *    G4UIcmdWithADoubleAndUnit* fRadiusCylinder_TargetCmd;
   *    
   *    
   *    
   *    
   *    
   */
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fTargMatCmd;
  delete fTargetPositionCmd;
  //   delete fChamMatCmd;
  //   delete fStepMaxCmd;
  delete fB2Directory;
  delete fDetDirectory;
  
  
  delete fTarget_Cube_SizeCmd;
  
  delete fOptionTargetCmd;
  delete fCavityMatCmd;
  delete fCavityPositionCmd;
  delete fCavity_SizeCmd;
  
  delete fWorldSelectorCmd;
  delete fLenghtWorldTubeCmd;
  delete fRadiusWorldTubeCmd;
  delete fWorld_Cube_SizeCmd;
  
  
  delete fLenghtCylinder_DetectorCmd;
  delete fLenghtCylinder_TargetCmd;
  delete fRadiusCylinder_DetectorCmd;
  delete fRadiusCylinder_TargetCmd;
  
  
  delete fDetectorSetupCmd;
  delete fTargetSetupCmd;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  
  // Basic command Target Position and Material
  if(command == fTargMatCmd ){ fDetectorConstruction->SetTargetMaterial(newValue);}
  if(command == fTargetPositionCmd ){fDetectorConstruction->SetTargetPosition(fTargetPositionCmd->GetNew3VectorValue(newValue));}
  
  
  // Setup Selection Target
  if(command == fDetectorSetupCmd ){ fDetectorConstruction->SetDetectorSetup(newValue);} 
  // Specificities command for Detector_Cylinder
  // Lenght + Radius (Full Lenght)
  if(command == fLenghtCylinder_DetectorCmd){fDetectorConstruction->SetLenghtCylinder_Detector(fLenghtCylinder_DetectorCmd->GetNewDoubleValue(newValue));}
  if(command == fRadiusCylinder_DetectorCmd){fDetectorConstruction->SetRadiusCylinder_Detector(fRadiusCylinder_DetectorCmd->GetNewDoubleValue(newValue));}
  // For Cube Target
  if(command == fTarget_Cube_SizeCmd ){fDetectorConstruction->SetTarget_Cube_Size(fTarget_Cube_SizeCmd->GetNew3VectorValue(newValue));}
  
  // For Cavity 
  
  if(command == fOptionTargetCmd ){ fDetectorConstruction->SetOptionTarget(newValue);} 
  if(command == fCavityMatCmd ){ fDetectorConstruction->SetCavityMaterial(newValue);} 
  if(command == fCavityPositionCmd ){fDetectorConstruction->SetCavityPosition(fCavityPositionCmd->GetNew3VectorValue(newValue));}
  if(command == fCavity_SizeCmd ){fDetectorConstruction->SetCavitySize(fCavity_SizeCmd->GetNew3VectorValue(newValue));}
  
  // World
  if(command == fWorldSelectorCmd ){ fDetectorConstruction->SetWorldSelector(newValue);} 
  
  if(command == fLenghtWorldTubeCmd){fDetectorConstruction->SetLenghtWorldTube(fLenghtWorldTubeCmd->GetNewDoubleValue(newValue));}
  
  if(command == fRadiusWorldTubeCmd){fDetectorConstruction->SetRadiusWorldTube(fRadiusWorldTubeCmd->GetNewDoubleValue(newValue));}
  
  if(command == fWorld_Cube_SizeCmd ){fDetectorConstruction->SetWorld_Cube_Size(fWorld_Cube_SizeCmd->GetNew3VectorValue(newValue));}
  
  
  //Selection Selection Detector
  if(command == fTargetSetupCmd){ fDetectorConstruction->SetTargetSetup(newValue);} 
  // Specificities command for Target_Cylinder
  // Lenght + Radius (Full Lenght)
  if(command == fLenghtCylinder_TargetCmd){fDetectorConstruction->SetLenghtCylinder_Target(fLenghtCylinder_TargetCmd->GetNewDoubleValue(newValue));}
  if(command == fRadiusCylinder_TargetCmd){fDetectorConstruction->SetRadiusCylinder_Target(fRadiusCylinder_TargetCmd->GetNewDoubleValue(newValue));}
  
  
  
  //   if( command == fChamMatCmd )
  //    { fDetectorConstruction->SetChamberMaterial(newValue);}
  // 
  //   if( command == fStepMaxCmd ) {
  //     fDetectorConstruction
  //       ->SetMaxStep(fStepMaxCmd->GetNewDoubleValue(newValue));
  //   }   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
