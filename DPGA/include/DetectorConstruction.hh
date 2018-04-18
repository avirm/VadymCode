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
// $Id: DetectorConstruction.hh 71323 2013-06-13 16:54:23Z gcosmo $
//
//
// @abongrand 2017 arthur.bongrand@clermont.in2p3.fr
//
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"
#include "G4SystemOfUnits.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"


#include "G4RunManager.hh"

#include <vector>
using namespace std;
class MagneticField;

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;
class G4GenericMessenger;

class G4VPhysicalVolume;
class G4LogicalVolume;

class DetectorMessenger;

/// Detector construction class to define materials and geometry.
///
/// Crystals are positioned in Ring, with an appropriate rotation matrix. 
/// Several copies of Ring are placed in the full detector.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    
    void RG(){G4RunManager::GetRunManager()->ReinitializeGeometry();};
    
    // Set Commands Methods
    
    void SetTargetMaterial(G4String );
    void SetTargetPosition(G4ThreeVector );
    
    void SetOptionTarget(G4String val){fOptionTarget =val;RG();};
    void SetCavityMaterial(G4String );
    void SetCavityPosition(G4ThreeVector );
    void SetCavitySize(G4ThreeVector val){fCavityFullSize = val;RG();};
    
    void SetWorldSelector(G4String val){fWorldSelector =val;RG();};
    void SetLenghtWorldTube(G4double val){fworld_Lenght =val;};
    void SetRadiusWorldTube(G4double val){fworld_Radius =val;};
    void SetWorld_Cube_Size(G4ThreeVector val){fWorld_Cube_Size = val;RG();};
    
  
    
    void SetTarget_Cube_Size(G4ThreeVector val){fTargetCubeFullSize = val;RG();};
    
    void SetDetectorSetup(G4String val){fDetectorSetup =val;RG();};
      void SetLenghtCylinder_Detector(G4double val){fLenghtCylinder_Detector =val;RG();};
      void SetRadiusCylinder_Detector(G4double val){fRadiusCylinder_Detector =val;};
    
    void SetTargetSetup(G4String val){fTargetSetup =val;RG();};
      void SetLenghtCylinder_Target(G4double val){fLenghtCylinder_Target =val;RG(); };
      void SetRadiusCylinder_Target(G4double val){fRadiusCylinder_Target =val;RG(); };
    
      G4String GetTargetSetup() {  return fTargetSetup; }
      G4String GetDetectorSetup() {  return fDetectorSetup; }
               
  private:
    void DefineMaterials();

    // Setup
    void Setup_World_Cube();
    void Setup_World_Tubs();
    
    // Detectors
    void Full_DPGA();
    void Detector_Cylinder(); 
    
    //Targets
    
    void Target_Cube();
    void Target_Cylinder();

    //     void Target_Tubs();
    
    //MultiThreading
    static G4ThreadLocal MagneticField* fMagneticField;
    static G4ThreadLocal G4FieldManager* fFieldMgr;
    
    //
    G4LogicalVolume* fLV_Cryst;
    G4LogicalVolume* fLV_Patient; // fantom // Ghost
    G4LogicalVolume* logicWorld;
    G4LogicalVolume* fLV_Wrap_cryst;
    G4bool  fCheckOverlaps;
    
    G4Material* DPGA_crystal_mat;
    G4VPhysicalVolume* fWorldPhys;
    G4VPhysicalVolume* PV_Cryst;
    G4VPhysicalVolume* Patient;
    
    G4LogicalVolume* fLV_Cavity_Target; 
    G4VPhysicalVolume* Cavity_Target;
    
    DetectorMessenger*  fMessenger;   // detector messenger
    
    // Variables modified by commands
      
      //Target Position and Material
      G4double fTargetPosition;
      G4Material* fMaterial_Target;
    
     // Setup Selection Target
     G4String fTargetSetup;
	// Specificities for Detector_Cylinder
	// Lenght + Radius (Full Lenght)
	G4double fLenghtCylinder_Detector;
	G4double fRadiusCylinder_Detector;
      
     // Setup Selection Detector
     G4String fDetectorSetup;
	// Specificities command for Detector_Cylinder
	// Lenght + Radius (Full Lenght) 
	G4double fLenghtCylinder_Target;
	G4double fRadiusCylinder_Target;
	
	// Target_Cube
	G4ThreeVector fTargetCubeFullSize;
	     
    // Get info surface et volume 
	G4Box* solidPatient;
	G4Tubs* solidPatient_Tubs;
	
	G4Box *solidCavity_Cube;
	
    // Other method for messenger
	
    //     void DefineCommands();
    void SetPatient_translation(G4ThreeVector val);//USless now with DetectirMessenger
    //     G4GenericMessenger* fMessenger;
    
    G4VisAttributes* skyBlue;
    
    // World size
     G4double fworld_Lenght ;
     G4double fworld_Radius;
    
     G4ThreeVector fPositionXYZTarget;
      
    G4String fOptionTarget;
    G4ThreeVector fCavityFullSize;
    
    G4Material * fMaterial_Cavity;
    
//     SetCavityMaterial
//     SetCavityPosition
    G4String fWorldSelector;
    G4ThreeVector fWorld_Cube_Size;
    
    G4ThreeVector fPositionXYZCavity;
     
    // Region 
    G4Region* aRegion;
    G4Region*   fTargetRegion;
    G4Region*   fDetectorRegion;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

