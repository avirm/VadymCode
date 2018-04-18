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
// $Id: DetectorConstruction.cc 71323 2013-06-13 16:54:23Z gcosmo $
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

// include sensitive SD
#include "DPGAPatientSD.hh"
#include "DPGACrystalSD.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <G4Trd.hh>
#include "G4VisAttributes.hh"
#include <G4SubtractionSolid.hh>
#include <G4UnionSolid.hh>
#include <G4PVReplica.hh>

using namespace std;




//   G4Colour  white   ()              ;  // white
G4Colour  white   (1.0, 1.0, 1.0) ;  // white
G4Colour  gris    (0.5, 0.5, 0.5) ;  // gray confiousing with gray energy unit
G4Colour  black   (0.0, 0.0, 0.0) ;  // black
G4Colour  red     (1.0, 0.0, 0.0) ;  // red
G4Colour  green   (0.0, 1.0, 0.0) ;  // green
G4Colour  blue    (0.0, 0.0, 1.0) ;  // blue
G4Colour  cyan    (0.0, 1.0, 1.0) ;  // cyan
G4Colour  magenta (1.0, 0.0, 1.0) ;  // magenta
G4Colour  yellow  (1.0, 1.0, 0.0) ;  // yellow


//#include "MagneticField.hh"

#include "G4GeometryManager.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4AutoDelete.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"
#include "G4PSNofStep.hh"
#include "G4SDParticleFilter.hh"

#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include <G4UnitsTable.hh>

#include "G4Region.hh"
#include "G4RegionStore.hh"
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
fCheckOverlaps(true),
fMessenger(0),
fLV_Cryst(NULL), fLV_Patient(NULL),fWorldPhys(0),logicWorld(0),fLenghtCylinder_Target(40*mm),fRadiusCylinder_Target(40*mm),fTargetSetup("Target_Cube"),
fDetectorSetup("Full_DPGA"),fLenghtCylinder_Detector(80*mm),fRadiusCylinder_Detector(80*mm),aRegion(0),fworld_Lenght(0.5*m),fworld_Radius(0.5*m),fPositionXYZTarget(0,0,0),fTargetCubeFullSize(5*cm,5*cm,5*cm),fOptionTarget(""),fCavityFullSize(1,1,1),fWorldSelector("Setup_World_Tubs"),fWorld_Cube_Size(1*m,1*m,1*m),fPositionXYZCavity(0,0,0)
{
  DefineMaterials();
  SetTargetMaterial("PMMA");
  SetCavityMaterial("G4_AIR");
  // define commands for this class
  fMessenger = new DetectorMessenger(this);



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
  delete fMessenger;
  //   delete DPGA_Patient; delete DPGA_Crystal;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Gamma detector Parameters
  //       DefineMaterials();


  // Cleanup old geometry
  // 	G4GeometryManager::GetInstance()->OpenGeometry();
  // 	G4PhysicalVolumeStore::GetInstance()->Clean();
  // 	G4LogicalVolumeStore::GetInstance()->Clean();
  // 	G4SolidStore::GetInstance()->Clean();

  // Setup Parameter
  std::cout << "******************************************************" << std::endl;
  std::cout << "                      Setup Geometry                  " << std::endl;
  std::cout << "******************************************************" << std::endl;

  if(fWorldSelector=="Setup_World_Cube" || fWorldSelector=="World_Cube" || fWorldSelector=="Cube" || fWorldSelector=="WC" ){
    Setup_World_Cube();
  }
  else{

    // Adapt the world
    if(fDetectorSetup == "Detector_Cylinder" && fTargetSetup == "Target_Cylinder"){

      G4double MargeWorldRadius = 1 *cm;
      fworld_Radius = fRadiusCylinder_Detector + MargeWorldRadius;
      if(fLenghtCylinder_Detector > (fLenghtCylinder_Target +2*abs(fPositionXYZTarget.getZ()))){fworld_Lenght = fLenghtCylinder_Detector + MargeWorldRadius;}
      else{fworld_Lenght = 2*fLenghtCylinder_Target + 2* (abs(fPositionXYZTarget.getZ()))+ MargeWorldRadius;}


      std::cout << "ADAPT WORLD size \nfworld_Radius " << G4BestUnit(fworld_Radius, "Length")
      << "\nfworld_Lenght " << G4BestUnit(fworld_Lenght, "Length")
      << std::endl;
    }

    if(fDetectorSetup == "Scoring_Target"){


      std::cout << "ADAPT WORLD size \nfworld_Lenght "
      << "\nfworld_Lenght " << G4BestUnit(fworld_Lenght, "Length")
      << std::endl;
    }

    Setup_World_Tubs();
  }

  // Setup TARGET
  //       std::cout << "fTargetSetup " << fTargetSetup << std::endl;

  if(fTargetSetup == "Target_Cube" ){

    Target_Cube();

    std::cout << "\n### Target consist of a Cube" <<
    " of " << fMaterial_Target->GetName()
    << " Size " << G4BestUnit((fTargetCubeFullSize.getX()), "Length")  << " x " << G4BestUnit((fTargetCubeFullSize.getY()), "Length") << " x "<< G4BestUnit((fTargetCubeFullSize.getZ()), "Length")
    << " Position " << G4BestUnit(Patient->GetTranslation(), "Length")
    << "  ### " << std::endl ;
    std::cout << "    Volume "<< G4BestUnit(solidPatient->GetCubicVolume(), "Volume") << " Surface " << G4BestUnit(solidPatient->GetSurfaceArea(), "Surface") << std::endl;

    if(fOptionTarget=="Cavity_Cube"){
    std::cout << "\n### With a Cavity" <<
    " of " << fMaterial_Cavity->GetName()
    << " Size " << G4BestUnit((fCavityFullSize.getX()), "Length")  << " x " << G4BestUnit((fCavityFullSize.getY()), "Length") << " x "<< G4BestUnit((fCavityFullSize.getZ()), "Length")
    << " Position " << G4BestUnit(Cavity_Target->GetTranslation(), "Length")
    << "  ### " << std::endl ;
    std::cout << "    Volume "<< G4BestUnit(solidCavity_Cube->GetCubicVolume(), "Volume") << " Surface " << G4BestUnit(solidCavity_Cube->GetSurfaceArea(), "Surface") << std::endl;

    }
    else if(fOptionTarget!=""){cout << "Warning Cavity Not Implemented Yet" << endl;}
    else{cout << "No Cavity" << endl;}
  }
  else if(fTargetSetup == "Target_Cylinder"){

    Target_Cylinder();

    std::cout << "\n### Target consist" <<
    " of " << fMaterial_Target->GetName()
    << " Radius R = " << G4BestUnit(fRadiusCylinder_Target, "Length")
    << " Lenght   = " << G4BestUnit(fLenghtCylinder_Target, "Length")
    << " Position " << G4BestUnit(Patient->GetTranslation(), "Length")
    <<  "  ###" << std::endl;
    std::cout << "    Volume "<< G4BestUnit(solidPatient_Tubs->GetCubicVolume(), "Volume") << " Surface " << G4BestUnit(solidPatient_Tubs->GetSurfaceArea(), "Surface") << std::endl;

    if(fOptionTarget!=""){cout << "Warning Cavity Not Implemented Yet" << endl;}
    else{cout << "No Cavity" << endl;}

  }
  else if(fTargetSetup == "No_Target"){cout << "No target in simulation" << endl;}
  else{
    std::cout << "Geometry Target Setup NOT implemented yet" << std::endl;
    std::cout << "Programm or call someone !" << std::endl;
    std::cout << "FATAL ERROR exit(0)" << std::endl; exit(0);
  }

  // Setup Detector

  if(fDetectorSetup == "Full_DPGA" ){

    Full_DPGA();

    std::cout << "\n### Detector Full_DPGA"
    << " Crystal of " << DPGA_crystal_mat->GetName()
    << " Size dx 1.3, dy 1.3, dz 1.5 *cm"
    << "  ### \n" << std::endl ;

  }
  else if(fDetectorSetup == "Detector_Cylinder"){
    // Check if Detecteur radius > Target radius
    if(fTargetSetup == "Target_Cylinder" && fRadiusCylinder_Target > fRadiusCylinder_Detector){
      std::cout << "FATAL ERROR fRadiusCylinder_Target > fRadiusCylinder_Detector exit(0)" <<std::endl;
    }



    Detector_Cylinder();

    std::cout << "\n### Detector consist of a cylinder" <<
    " of " << fMaterial_Target->GetName()
    << " Radius R(mm)= " << G4BestUnit(fRadiusCylinder_Detector, "Length")
    << " Lenght (mm)= " << G4BestUnit(fLenghtCylinder_Detector, "Length")
    << " Position "<< G4BestUnit(Patient->GetTranslation(), "Length")
    <<  "  ### \n" << std::endl;

  }
  else if(fDetectorSetup == "Without_Detector"){
    std::cout << "\n### No Detector Selected " << std::endl;
  }
  else if(fDetectorSetup == "Scoring_Target"){
    std::cout << "\n### Scoring Target \n Think to switch on Scoring \n RadioactiveDecay don't work \n NO ROOT File" << std::endl;
  }
  else{
    std::cout << "Geometry Detector Setup NOT implemented yet" << std::endl;
    std::cout << "Programm or call someone !" << std::endl;
    std::cout << "FATAL ERROR exit(0)" << std::endl; exit(0);
  }

  // **************
  // Cut per Region
  // **************

  // A higer cut is fixed in the cylinder Detector to obtain positions for MAR
  //     if (!aRegion){
  // 	aRegion = new G4Region("DetectorLog");
  // 	if(fDetectorSetup == "Detector_Cylinder"){
  // 	fLV_Cryst -> SetRegion(aRegion); // detectorLogicalVolume
  // 	 aRegion -> AddRootLogicalVolume(fLV_Cryst);
  // 	}
  //
  // 	}

  if(!fTargetRegion){
    fTargetRegion = new G4Region("Target");
    fDetectorRegion   = new G4Region("Detector");
    fTargetRegion->AddRootLogicalVolume(fLV_Patient);
    fDetectorRegion->AddRootLogicalVolume(fLV_Cryst);
    if(fOptionTarget=="Cavity_Cube")fTargetRegion->AddRootLogicalVolume(fLV_Cavity_Target);

  }






  std::cout << "******************************************************" << std::endl;

  // Inform the kernel about the new geometry
  G4RunManager::GetRunManager() -> GeometryHasBeenModified();
  G4RunManager::GetRunManager() -> PhysicsHasBeenModified();

  return fWorldPhys;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // sensitive detectors -------------------------------------------------------

  // sensitive detectors -----------------------------------------------------

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String SDname;

  SDman->SetVerboseLevel(1);

  // declare Crystals as a MultiFunctionalDetector scorer
  //

  if(fDetectorSetup != "Without_Detector" && fDetectorSetup != "Scoring_Target"){

    G4MultiFunctionalDetector* crystal = new G4MultiFunctionalDetector("crystal");
    G4VPrimitiveScorer* primitiv1 = new G4PSEnergyDeposit("edep");
    crystal->RegisterPrimitive(primitiv1);

    G4VSensitiveDetector* DPGA_Crystal = new DPGACrystalSD(SDname="/DPGA_Crystal");
    SDman->AddNewDetector(DPGA_Crystal);
    fLV_Cryst->SetSensitiveDetector(DPGA_Crystal);
  }

  // declare Patient as a MultiFunctionalDetector scorer
  //


  G4MultiFunctionalDetector* patient = new G4MultiFunctionalDetector("patient");
  G4VPrimitiveScorer* primitiv2 = new G4PSDoseDeposit("dose");
  patient->RegisterPrimitive(primitiv2);
  //      SetSensitiveDetector("fLV_Patient",patient);

  G4VSensitiveDetector* DPGA_Patient = new DPGAPatientSD(SDname="/DPGA_Patient");
  SDman->AddNewDetector(DPGA_Patient);
  fLV_Patient->SetSensitiveDetector(DPGA_Patient);




  // magnetic field ----------------------------------------------------------
  //     fMagneticField = new MagneticField();
  //     fFieldMgr = new G4FieldManager();
  //     fFieldMgr->SetDetectorField(fMagneticField);
  //     fFieldMgr->CreateChordFinder(fMagneticField);
  //     G4bool forceToAllDaughters = true;
  //     logicWorld->SetFieldManager(fFieldMgr, forceToAllDaughters);

  // Register the field and its manager for deleting
  //G4AutoDelete::Register(fMagneticField);
  G4AutoDelete::Register(fFieldMgr);
}



// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//*****************************************************
// use less now.
//*****************************************************
void DetectorConstruction::SetPatient_translation(G4ThreeVector val)
{
  fPositionXYZTarget = val;
  Patient->SetTranslation(val);

  // tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();
}
//************************************************************
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetPosition(G4ThreeVector val){

  fPositionXYZTarget = val;
  Patient->SetTranslation(val);
  // tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  //         std::cout << "\n----> The target ~" << Patient->GetName() << " ~ is placed with your define command at " << Patient->GetTranslation() /mm << " mm" << G4endl;

}

void DetectorConstruction::SetCavityPosition(G4ThreeVector val){

  fPositionXYZCavity = val;
  cout << fPositionXYZCavity << endl;
//   Cavity_Target->SetTranslation(val);
  // tell G4RunManager that we change the geometry
  G4RunManager::GetRunManager()->GeometryHasBeenModified();

  //         std::cout << "\n----> The target ~" << Patient->GetName() << " ~ is placed with your define command at " << Patient->GetTranslation() /mm << " mm" << G4endl;

}

//**********************************************************************

void DetectorConstruction::SetTargetMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();

  if(materialName == "Air" || materialName == "AIR"  ) materialName = "G4_AIR";

  G4Material* pttoMaterial =
  nistManager->FindOrBuildMaterial(materialName);

  if (fMaterial_Target != pttoMaterial) {
    if ( pttoMaterial ) {
      fMaterial_Target = pttoMaterial;
      if (fLV_Patient) fLV_Patient->SetMaterial(fMaterial_Target);
      //         std::cout << "\n----> The target is made of " << materialName << G4endl;
    } else {
      std::cout << "\n-->  WARNING from SetTargetMaterial : "
      << materialName << " not found" << G4endl;
    }
  }

  //   std::cout << "\n----> /!\ Default Setup /!\ The target is made of " << materialName << G4endl;

}

void DetectorConstruction::SetCavityMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();
  if(materialName == "Air" || materialName == "AIR"  ) materialName = "G4_AIR";

  G4Material* pttoMaterial =
  nistManager->FindOrBuildMaterial(materialName);

  if (fMaterial_Cavity != pttoMaterial) {
    if ( pttoMaterial ) {
      fMaterial_Cavity = pttoMaterial;
      if (fLV_Cavity_Target) fLV_Cavity_Target->SetMaterial(fMaterial_Cavity);
      //         std::cout << "\n----> The target is made of " << materialName << G4endl;
    } else {
      std::cout << "\n-->  WARNING from SetCavityMaterial : "
      << materialName << " not found" << G4endl;
    }
  }

}
void DetectorConstruction::Setup_World_Tubs(){
  //-----------------------------------------------------------
  //          World
  //-----------------------------------------------------------



  G4RotationMatrix rotm  = G4RotationMatrix();
  rotm.rotateY(0);
  G4ThreeVector position(0,0,0);
  G4Transform3D transform = G4Transform3D(rotm,position);


  //// Materiaux DPGA
  G4NistManager* nist = G4NistManager::Instance();

  // Default (World ... )
  G4Material* default_mat = nist->FindOrBuildMaterial("G4_AIR");


  G4Tubs* solidWorld =
  new G4Tubs("World",                       //its name
	     0., fworld_Radius, 0.5*(fworld_Lenght), 0., twopi); //its size

  logicWorld =
  new G4LogicalVolume(solidWorld,          //its solid
		      default_mat,         //its material
		      "World");            //its name

  fWorldPhys =
  new G4PVPlacement( transform ,       //at (0,0,0)
  logicWorld,            //its logical volume
  "World",               //its name
  0,                     //its mother  volume
  false,                 //no boolean operation
  0,                     //copy number
  fCheckOverlaps);       // checking overlaps


  // SetVisAttributes

  //   logicWorld->SetVisAttributes (G4VisAttributes::Invisible);


}


void DetectorConstruction::Setup_World_Cube(){
  //-----------------------------------------------------------
  //          World
  //-----------------------------------------------------------

  //G4double world_sizeXY = 2.4*ring_R2;
  //G4double world_sizeZ  = 1.2*detector_dZ;



  //// Materiaux DPGA
  G4NistManager* nist = G4NistManager::Instance();

  // Default (World ... )
  G4Material* default_mat = nist->FindOrBuildMaterial("G4_AIR");


  G4Box* solidWorld =
  new G4Box("World",                       //its name
	    0.5*fWorld_Cube_Size.getX(), 0.5*fWorld_Cube_Size.getY(), 0.5*fWorld_Cube_Size.getZ()); //its size

  logicWorld =
  new G4LogicalVolume(solidWorld,          //its solid
		      default_mat,         //its material
		      "World");            //its name

  fWorldPhys =
  new G4PVPlacement(0,                     //no rotation
		    G4ThreeVector(),       //at (0,0,0)
  logicWorld,            //its logical volume
  "World",               //its name
  0,                     //its mother  volume
  false,                 //no boolean operation
  0,                     //copy number
  fCheckOverlaps);       // checking overlaps


  // SetVisAttributes

//   logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

}
void DetectorConstruction::Target_Cylinder(){

  G4NistManager* nist = G4NistManager::Instance();

  G4RotationMatrix rotm  = G4RotationMatrix();
  rotm.rotateY(0);
  G4Transform3D transform = G4Transform3D(rotm,fPositionXYZTarget);

  //     std::cout << "Radius Target " << fRadiusCylinder_Target /mm << " mm" << std::endl;

  solidPatient_Tubs =
  new G4Tubs("Patient", 0., fRadiusCylinder_Target, 0.5*(fLenghtCylinder_Target), 0., twopi);

  fLV_Patient =
  new G4LogicalVolume(solidPatient_Tubs,        //its solid
		      fMaterial_Target,         //its material
		      "fLV_Patient");        //its name



  //
  // place patient in world
  //
  Patient = new G4PVPlacement(transform,         //at (0,0,0)
  fLV_Patient,            //its logical volume
  "Patient",               //its name
  logicWorld,              //its mother  volume
  false,                   //no boolean operation
  0,                       //copy number
  fCheckOverlaps);         // checking overlaps

}

void DetectorConstruction::Target_Cube(){


  //*****************************************
  // Target Type Cube PMMA 5x5x5 cm3 Nice
  //*****************************************


  G4NistManager* nist = G4NistManager::Instance();




  //   G4Material* patient_mat  = nist->FindOrBuildMaterial("G4_BRAIN_ICRP");
  //   G4Material* patient_mat = nist->FindOrBuildMaterial("G4_AIR");
  //    G4Material* patient_mat = nist->FindOrBuildMaterial("PMMA");



  //    G4cout << "fMaterial " << fMaterial_Target << std::endl;

  solidPatient =
  new G4Box("Patient",                       //its name
	    0.5*(fTargetCubeFullSize.getX()), 0.5*(fTargetCubeFullSize.getY()), 0.5*(fTargetCubeFullSize.getZ())); //its size




  fLV_Patient =
  new G4LogicalVolume(solidPatient,        //its solid
		      fMaterial_Target,         //its material
		      "fLV_Patient");        //its name





  Patient = new G4PVPlacement(0,
			      fPositionXYZTarget,	  //at (0,0,0)
  fLV_Patient,            //its logical volume
  "Patient",               //its name
  logicWorld,              //its mother  volume
  false,                   //no boolean operation
  0,                       //copy number
  fCheckOverlaps);         // checking overlaps


  // Cavity
  if(fOptionTarget=="Cavity_Cube"){

    solidCavity_Cube =
  new G4Box("Cavity_Target",                       //its name
	    0.5*(fCavityFullSize.getX()), 0.5*(fCavityFullSize.getY()), 0.5*(fCavityFullSize.getZ())); //its size


  fLV_Cavity_Target =
  new G4LogicalVolume(solidCavity_Cube,        //its solid
		      fMaterial_Cavity,         //its material
		      "fLV_Cavity_Target");        //its name


  Cavity_Target = new G4PVPlacement(0,
			      fPositionXYZCavity,	  //at (0,0,0)
  fLV_Cavity_Target,            //its logical volume
  "Cavity_Target",               //its name
  fLV_Patient,              //its mother  volume
  false,                   //no boolean operation
  0,                       //copy number
  true);         // checking overlaps



  }





}
void DetectorConstruction::Detector_Cylinder(){


  G4NistManager* nist = G4NistManager::Instance();

  G4Material* Cylinder_Detector_material   = nist->FindOrBuildMaterial("G4_AIR");

  G4RotationMatrix rotm  = G4RotationMatrix();
  rotm.rotateY(0);
  G4ThreeVector position(0,0,0);
  G4Transform3D transform = G4Transform3D(rotm,position);

  //     std::cout << "Radius Target " << fRadiusCylinder_Detector /mm << " mm" << std::endl;

  G4double Cylinder_Thickness = 0.1 *mm;

  G4Tubs* solidCylinder =
  new G4Tubs("Detector", fRadiusCylinder_Detector, fRadiusCylinder_Detector+Cylinder_Thickness, 0.5*(fLenghtCylinder_Detector), 0., twopi);

  fLV_Cryst =
  new G4LogicalVolume(solidCylinder,        //its solid
		      Cylinder_Detector_material,         //its material
		      "fLV_Cryst");        //its name



  //
  // place patient in world
  //
  PV_Cryst = new G4PVPlacement(transform,         //at (0,0,0)
  fLV_Cryst,            //its logical volume
  "Cylinder_Detector",               //its name
  logicWorld,              //its mother  volume
  false,                   //no boolean operation
  0,                       //copy number
  fCheckOverlaps);         // checking overlaps



  // Visualisation attributes of the detector
  skyBlue = new G4VisAttributes( G4Colour(135/255. , 206/255. ,  235/255. ));
  skyBlue -> SetVisibility(true);
  skyBlue -> SetForceSolid(true);
  //skyBlue -> SetForceWireframe(true);
  fLV_Cryst -> SetVisAttributes(skyBlue);




}

void DetectorConstruction::Full_DPGA(){

  /// Devellopement
  //// Materiaux DPGA
  G4NistManager* nist = G4NistManager::Instance();

  // Default (World ... )
  G4Material* default_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Cassette DPGA
  // Crystal
  DPGA_crystal_mat = nist->FindOrBuildMaterial("LYSO_Ce");

  G4Material* cryst_mat   = nist->FindOrBuildMaterial("LYSO");
  G4Material* Wrap_crystal_mat = nist->FindOrBuildMaterial("G4_TEFLON");

  // "Capot PVC"
  G4Material* DPGA_Capot_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE"); // PVC
  // Parties laterales de la cassette
  G4Material* DPGA_LateralPart_mat = nist->FindOrBuildMaterial("POM_C");
  // Barre Barre_support_PM
  G4Material* DPGA_BarreSupportPM_mat = nist->FindOrBuildMaterial("POM_C");

  // PMT body
  G4Material* PMT_Body_mat = nist->FindOrBuildMaterial("G4_Pyrex_Glass");


  /*
   * G4Material* cryst_mat   = nist->FindOrBuildMaterial("LYSO");
   *
   * //G4Material * Xe = nist->FindOrBuildMaterial("G4_Xe");
   * G4Material * PVC_mat = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
   * G4Material * TEFLON_mat = nist->FindOrBuildMaterial("G4_TEFLON");
   * G4Material * SS04_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
   * G4Material * POM_C_mat = nist->FindOrBuildMaterial("POM_C");
   */

  // Option to switch on/off checking of volumes overlaps
  //
  fCheckOverlaps = false;







  //******* DPGA ***************** /////

  /// Cassette du DPGA

  // "Capot PVC" Trapezoid @@@ Full lenght @@@

  G4double Cassette_epaisseurPVC = 0.15 *cm; //
  G4double Cassette_epaisseur_lateral = 0.8 *cm; //
  G4double CassettePVC_air_dx1 = 3.5*cm; // full lenght
  G4double CassettePVC_air_dx2 = 6.65*cm; // full lenght
  G4double CassettePVC_air_dy = 20.4*cm; // full lenght
  G4double CassettePVC_air_dz = 12.30*cm; // hauteur de la cassette


  G4Trd* CassettePVC =
  new G4Trd("CassettePVC",(CassettePVC_air_dx1+(2*Cassette_epaisseurPVC))/2,(CassettePVC_air_dx2+(2*Cassette_epaisseurPVC))/2,(CassettePVC_air_dy+(2*(Cassette_epaisseurPVC+Cassette_epaisseur_lateral)))/2,(CassettePVC_air_dy+(2*(Cassette_epaisseurPVC+Cassette_epaisseur_lateral)))/2,(CassettePVC_air_dz+(2*Cassette_epaisseurPVC))/2) ;

  G4LogicalVolume* LV_CassettePVC =
  new G4LogicalVolume(CassettePVC,           //its solid
		      DPGA_Capot_mat,         //its material
		      "LV_CassettePVC");             //its name



  //      G4VPhysicalVolume* PV_CassettePVC =
  //       new G4PVPlacement(0,
  // 		       G4ThreeVector(),            //rotation,position
  //                       LV_CassettePVC,            //its logical volume
  //                       "PV_CassettePVC",             //its name
  //                       logicWorld,             //its mother  volume
  //                       false,                 //no boolean operation
  //                       0,fCheckOverlaps);
  //
  //
  // Capot lateral

  G4Trd* Cassette_lateral_Part1 =
  new G4Trd("Cassette_lateral_Part1",CassettePVC_air_dx1/2,CassettePVC_air_dx2/2,(Cassette_epaisseur_lateral)/2,(Cassette_epaisseur_lateral)/2,CassettePVC_air_dz/2) ;


  G4LogicalVolume* LV_Cassette_lateral_Part1 =
  new G4LogicalVolume(Cassette_lateral_Part1,           //its solid
		      DPGA_LateralPart_mat,         //its material
		      "LV_Cassette_lateral_Part1");             //its name



  G4VPhysicalVolume* PV_Cassette_lateral_Part1 =
  new G4PVPlacement(0,
		    G4ThreeVector(0,-(CassettePVC_air_dy+Cassette_epaisseur_lateral)/2,0),            //rotation,position
		    LV_Cassette_lateral_Part1,            //its logical volume
		    "PV_Cassette_lateral_Part1",             //its name
		    LV_CassettePVC,             //its mother  volume
		    false,                 //no boolean operation
		    0,fCheckOverlaps);


  G4Trd* Cassette_lateral_Part2 =
  new G4Trd("Cassette_lateral_Part2",CassettePVC_air_dx1/2,CassettePVC_air_dx2/2,(Cassette_epaisseur_lateral)/2,(Cassette_epaisseur_lateral)/2,CassettePVC_air_dz/2) ;


  G4LogicalVolume* LV_Cassette_lateral_Part2 =
  new G4LogicalVolume(Cassette_lateral_Part2,           //its solid
		      DPGA_LateralPart_mat,         //its material
		      "LV_Cassette_lateral_Part2");             //its name



  G4VPhysicalVolume* PV_Cassette_lateral_Part2 =
  new G4PVPlacement(0,
		    G4ThreeVector(0,+(CassettePVC_air_dy+Cassette_epaisseur_lateral)/2,0),            //rotation,position
		    LV_Cassette_lateral_Part2,            //its logical volume
		    "PV_Cassette_lateral_Part2",             //its name
		    LV_CassettePVC,             //its mother  volume
		    false,                 //no boolean operation
		    0,fCheckOverlaps);

  // surface vide de la cassette /// Trapezoid @@@ Full lenght @@@


  G4Trd* CassettePVC_air =
  new G4Trd("CassettePVC_air",CassettePVC_air_dx1/2,CassettePVC_air_dx2/2,(CassettePVC_air_dy)/2,(CassettePVC_air_dy)/2,CassettePVC_air_dz/2) ;


  G4LogicalVolume* LV_CassettePVC_air =
  new G4LogicalVolume(CassettePVC_air,           //its solid
		      default_mat,         //its material
		      "LV_CassettePVC_air");             //its name



  G4VPhysicalVolume* PV_CassettePVC_air =
  new G4PVPlacement(0,
		    G4ThreeVector(),            //rotation,position
		    LV_CassettePVC_air,            //its logical volume
		    "PV_CassettePVC_air",             //its name
		    LV_CassettePVC,             //its mother  volume
		    false,                 //no boolean operation
		    0,fCheckOverlaps);



  /// Cristal + Teflon


  G4double cryst_dx = 1.3*cm, cryst_dy = 1.3*cm, cryst_dz = 1.5*cm;
  G4double Wrap_thickness = 0.06*cm;        //a gap for wrapping DPGA cristal in teflon
  G4double gap_capot_surface_cristal = 0.4 *cm ;// a gap for wrapping DPGA -> teflon

  G4Box* Wrap_cryst = new G4Box("Wrap_cryst", (cryst_dx+(2*Wrap_thickness))/2, (cryst_dy+(2*Wrap_thickness))/2, (cryst_dz+Wrap_thickness)/2);

  fLV_Wrap_cryst =
  new G4LogicalVolume(Wrap_cryst,          //its solid
		      Wrap_crystal_mat,           //its material
		      "fLV_Wrap_cryst");        //its name

  // 	     G4VPhysicalVolume* PV_Wrap_cryst =
  //              new G4PVPlacement(0,
  // 		       G4ThreeVector(0,0,(-CassettePVC_air_dz/2)+gap_capot_surface_cristal+(cryst_dz+Wrap_thickness)/2),            //rotation,position
  //                       fLV_Wrap_cryst,            //its logical volume
  //                       "PV_Wrap_cryst",             //its name
  //                       LV_CassettePVC_air,             //its mother  volume
  //                       false,                 //no boolean operation
  //                       0,fCheckOverlaps);
  //

  G4Box* Cryst = new G4Box("Cryst", (cryst_dx)/2, (cryst_dy)/2, (cryst_dz)/2);

  fLV_Cryst =
  new G4LogicalVolume(Cryst,          //its solid
		      DPGA_crystal_mat,           //its material
		      "fLV_Cryst");        //its name

  PV_Cryst =
  new G4PVPlacement(0,
		    G4ThreeVector(0,0,0.5*Wrap_thickness),            //rotation,position
		    fLV_Cryst,            //its logical volume
		    "PV_Cryst",             //its name
		    fLV_Wrap_cryst,             //its mother  volume
		    false,                 //no boolean operation
		    0,fCheckOverlaps);



  /// Barre support 5 blocs

  G4double Barre_support_PM_dx1 = 3.9*cm; // full lenght Reality ???
  G4double Barre_support_PM_dx2 = 4.5*cm; // full lenght
  G4double Barre_support_PM_dy = 20.4*cm; // full lenght
  G4double Barre_support_PM_dz = 3.5*cm; // hauteur de la cassette


  G4Trd* Barre_support_PM =
  new G4Trd("Barre_support_PM",Barre_support_PM_dx1/2,Barre_support_PM_dx2/2,Barre_support_PM_dy/2,Barre_support_PM_dy/2,Barre_support_PM_dz/2);

  G4LogicalVolume* LV_Barre_support_PM =
  new G4LogicalVolume(
    Barre_support_PM,           //its solid
    DPGA_BarreSupportPM_mat,         //its material
    "LV_Barre_support_PM");             //its name

  G4VPhysicalVolume* PV_Barre_support_PM =
  new G4PVPlacement(0,
		    G4ThreeVector(0,0, (-CassettePVC_air_dz/2)+gap_capot_surface_cristal+(cryst_dz+Wrap_thickness)+Barre_support_PM_dz/2),            //rotation,position
		    LV_Barre_support_PM,            //its logical volume
		    "PV_Barre_support_PM",             //its name
		    LV_CassettePVC_air,             //its mother  volume
		    false,                 //no boolean operation
		    0,fCheckOverlaps);

  /// PM



  G4double Rmin_PMT_Body = 1.82/2 *cm ; // ! rayon
  G4double Rmax_PMT_Body = 1.85/2 *cm ; //  ! rayon

  // part PM in Barre support Part 1

  G4double hmax_PMT_Body_Part1 = Barre_support_PM_dz ;

  G4Tubs* PMT_Body_Part1 =
  new G4Tubs("PMT_Body_Part1", Rmin_PMT_Body, Rmax_PMT_Body, hmax_PMT_Body_Part1/2, 0., twopi) ;


  G4LogicalVolume* LV_PMT_Body_Part1 =
  new G4LogicalVolume(
    PMT_Body_Part1,           //its solid
    PMT_Body_mat,         //its material
    "LV_PMT_Body_Part1");             //its name

  // 		      G4VPhysicalVolume* PV_PMT_Body_Part1 =
  // 			new G4PVPlacement(0,
  // 			  G4ThreeVector(),            //rotation,position
  // 			  LV_PMT_Body_Part1,            //its logical volume
  // 			  "PV_PMT_Body_Part1",             //its name
  // 			  LV_Barre_support_PM,             //its mother  volume
  // 			  true,                 //no boolean operation
  // 			  0,fCheckOverlaps);
  //

  // part PM without Barre support Part 2
  G4double hmax_PMT_Body = 8.8 *cm;
  G4double hmax_PMT_Body_Part2 = (hmax_PMT_Body-hmax_PMT_Body_Part1);

  G4Tubs* PMT_Body =
  new G4Tubs("PMT_Body", Rmin_PMT_Body, Rmax_PMT_Body, hmax_PMT_Body_Part2/2, 0., twopi) ;

  //
  G4LogicalVolume* LV_PMT_Body_Part2 =
  new G4LogicalVolume(
    PMT_Body,           //its solid
    PMT_Body_mat,         //its material
    "LV_PMT_Body_Part2");             //its name

  // 		      G4VPhysicalVolume* PV_PMT_Body =
  // 			new G4PVPlacement(0,
  // 			  G4ThreeVector(0,0,(-CassettePVC_air_dz/2)+gap_capot_surface_cristal+(cryst_dz+Wrap_thickness)+Barre_support_PM_dz+hmax_PMT_Body_Part2/2),            //rotation,position
  // 			  LV_PMT_Body_Part2,            //its logical volume
  // 			  "PV_PMT_Body",             //its name
  // 			  LV_CassettePVC_air,             //its mother  volume
  // 			  true,                 //no boolean operation
  // 			  0,fCheckOverlaps);
  //


  // place crystal and PMT_Body
  //

  G4double SpaceBetweenEachPMT_Y = 1.95 *cm;

  G4double FirstPosition_PMT_Body_Part1_X = 0 * cm;
  G4double FirstPosition_PMT_Body_Part1_Y = (-10.2 + 1.425) *cm;
  G4double FirstPosition_PMT_Body_Part1_Z= 0*cm;

  G4int Counter_copyNumber = 0;

  for (G4int iquartet = 0; iquartet < /* nb_cryst*/ 5 ; iquartet++){

    // Disposition en ligne

    G4double CoordoneesY_PMT_Body_Part1_Quartet =   FirstPosition_PMT_Body_Part1_Y + iquartet * 2 * SpaceBetweenEachPMT_Y;


    // Disposition quartet Reality
    for (G4int i_intra_quartet = 0; i_intra_quartet < 4 ; i_intra_quartet++){

      G4double CoordoneesY_PMT_Body_Part1 = 0 *cm ;
      G4double CoordoneesX_PMT_Body_Part1 = 0 *cm ;


      Counter_copyNumber = iquartet * 10 + i_intra_quartet;

      if(i_intra_quartet==0){
	CoordoneesX_PMT_Body_Part1 = -0.975 *cm;
	CoordoneesY_PMT_Body_Part1 = CoordoneesY_PMT_Body_Part1_Quartet;
      }
      else if(i_intra_quartet==1){
	CoordoneesX_PMT_Body_Part1 = -0.975 *cm;
	CoordoneesY_PMT_Body_Part1 = CoordoneesY_PMT_Body_Part1_Quartet + SpaceBetweenEachPMT_Y;
      }
      else if (i_intra_quartet==2){
	CoordoneesX_PMT_Body_Part1 = 0.975 *cm;
	CoordoneesY_PMT_Body_Part1 = CoordoneesY_PMT_Body_Part1_Quartet ;
      }
      else if (i_intra_quartet==3){
	CoordoneesX_PMT_Body_Part1 = 0.975 *cm;
	CoordoneesY_PMT_Body_Part1 = CoordoneesY_PMT_Body_Part1_Quartet + SpaceBetweenEachPMT_Y;
      }
      else {G4cout << " error quartet placement check i_intra_quartet "  << G4endl;}


      ///@@  Place PM part in PMT support


      new G4PVPlacement(0,
			G4ThreeVector(CoordoneesX_PMT_Body_Part1,CoordoneesY_PMT_Body_Part1,0) ,            //rotation,position
			LV_PMT_Body_Part1,            //its logical volume
			"PV_PMT_Body_Part1",             //its name
			LV_Barre_support_PM,             //its mother  volume
			false,                 //no boolean operation
			Counter_copyNumber,                 //copy number
			fCheckOverlaps);       // checking overlaps

      ///@@  Place scintillator cristaux

      new G4PVPlacement(0,
			G4ThreeVector(CoordoneesX_PMT_Body_Part1,CoordoneesY_PMT_Body_Part1,(-CassettePVC_air_dz/2)+gap_capot_surface_cristal+(cryst_dz+Wrap_thickness)/2),            //rotation,position
			fLV_Wrap_cryst,            //its logical volume
			"PV_Wrap_cryst",             //its name
			LV_CassettePVC_air,             //its mother  volume
			false,                 //no boolean operation
			Counter_copyNumber,fCheckOverlaps);

      ///@@  Place the rest of PMT_body

      new G4PVPlacement(0,
			G4ThreeVector(CoordoneesX_PMT_Body_Part1,CoordoneesY_PMT_Body_Part1,(-CassettePVC_air_dz/2)+gap_capot_surface_cristal+(cryst_dz+Wrap_thickness)+Barre_support_PM_dz+hmax_PMT_Body_Part2/2),            //rotation,position
			LV_PMT_Body_Part2,            //its logical volume
			"PV_PMT_Body",             //its name
			LV_CassettePVC_air,             //its mother  volume
			true,                 //no boolean operation
			Counter_copyNumber,fCheckOverlaps);

      // 			std::cout << "detector construction copy number " << Counter_copyNumber << std::endl;
    }

  } // end for


  G4double Rayon_centreDPGA_bordcristal = 14.84 *cm;

  G4double RayonDPGA = (CassettePVC_air_dz+(2*Cassette_epaisseurPVC))/2 + Rayon_centreDPGA_bordcristal - gap_capot_surface_cristal -Wrap_thickness ;
  G4double dPhi = 15 *deg, half_dPhi = 0.5*dPhi;
  G4double Init_Phi = 142.5 *deg;

  G4int Compteur_Shift_Cassette = 1;
  G4double Shift_Z_Cassette = 0.65 *cm;


  for (G4int i_Cassette_Number = 0; i_Cassette_Number < 12 ; i_Cassette_Number++) {

    //      if(i_Cassette_Number == 0) {dPhi = Init_Phi;}
    //      else{dPhi = 15*deg;}

    G4double Phi = i_Cassette_Number*dPhi + Init_Phi;

    G4bool Shift_Cassette_Z_Bool = true;

    G4double Shift_Cassette_Real = 0*cm;

    if (Shift_Cassette_Z_Bool == true) Shift_Cassette_Real = Shift_Z_Cassette * Compteur_Shift_Cassette ;

    Compteur_Shift_Cassette -= 1;
    if(Compteur_Shift_Cassette < -1) Compteur_Shift_Cassette = 1;

    if(i_Cassette_Number>5){

      G4double OpenPET_Angle = 90 *deg;
      Phi = i_Cassette_Number*dPhi + Init_Phi+ OpenPET_Angle ;
      //       G4cout << OpenPET_Angle *180 / 3.14157 << G4endl;
    }

    G4cout << "Angle Cassette " << Phi * (180 / 3.14157) -90 << "°"  << G4endl;

    G4RotationMatrix rotm  = G4RotationMatrix();
    rotm.rotateX((90)*deg);
    rotm.rotateY(0*deg);
    G4double RotationZ = 90 *deg;
    rotm.rotateZ((RotationZ+Phi));




    G4ThreeVector uz = G4ThreeVector(std::cos(Phi)*RayonDPGA, std::sin(Phi)*RayonDPGA ,Shift_Cassette_Real);
    G4ThreeVector position = uz;
    G4Transform3D transform = G4Transform3D(rotm,position);

    new G4PVPlacement(transform,             //rotation,position
		      LV_CassettePVC,            //its logical volume
		      "CassettePVC",             //its name
		      logicWorld,             //its mother  volume
		      false,                 //no boolean operation
		      i_Cassette_Number+1,                 //copy number
		      fCheckOverlaps);       // checking overlaps
  }



  // Visualization attributes
  //
  /*  logicRing->SetVisAttributes (G4VisAttributes::Invisible);
   *logicDetector->SetVisAttributes (G4VisAttributes::Invisible);   */




  // PVC revetement

  G4VisAttributes* Vis_CassettePVC = new G4VisAttributes(white); //Black
  Vis_CassettePVC->SetVisibility(true);
  LV_CassettePVC->SetVisAttributes(Vis_CassettePVC);

  // Cassette


  G4VisAttributes* Vis_Cassette_lateral_Part1= new G4VisAttributes(magenta);
  Vis_Cassette_lateral_Part1->SetVisibility(true);
  Vis_Cassette_lateral_Part1->SetForceSolid(true);
  LV_Cassette_lateral_Part1->SetVisAttributes(Vis_Cassette_lateral_Part1);

  G4VisAttributes* Vis_Cassette_lateral_Part2 = new G4VisAttributes(magenta);
  Vis_Cassette_lateral_Part2->SetVisibility(true);
  Vis_Cassette_lateral_Part2->SetForceSolid(true);
  LV_Cassette_lateral_Part2->SetVisAttributes(Vis_Cassette_lateral_Part2);

  G4VisAttributes* Vis_CassettePVC_air= new G4VisAttributes(blue); //Blue
  Vis_CassettePVC_air->SetVisibility(true);
  LV_CassettePVC_air->SetVisAttributes(Vis_CassettePVC_air);


  // PMT Barre_support_PM

  G4VisAttributes* Vis_Barre_support_PM = new G4VisAttributes(green);
  Vis_Barre_support_PM->SetVisibility(true);
  Vis_Barre_support_PM->SetForceSolid(true);
  LV_Barre_support_PM->SetVisAttributes(Vis_Barre_support_PM);



  // Scintillator + Teflon
  G4VisAttributes* Vis_Wrap_cryst = new G4VisAttributes( G4Colour(255/255. ,255/255. ,0/255.   )); // Yellow
  Vis_Wrap_cryst->SetVisibility(true);
  Vis_Wrap_cryst->SetForceSolid(true);
  fLV_Wrap_cryst->SetVisAttributes(Vis_Wrap_cryst);

  G4VisAttributes* Vis_Cryst = new G4VisAttributes( G4Colour(204/255. ,0/255.   ,51/255.  )); // Roux
  Vis_Cryst->SetVisibility(true);
  Vis_Cryst->SetForceSolid(true);
  fLV_Cryst->SetVisAttributes(Vis_Cryst);

  // PMT_Body

  G4VisAttributes* Vis_PMT_Body_Part1 = new G4VisAttributes(white); //Black
  Vis_PMT_Body_Part1->SetVisibility(true);
  Vis_PMT_Body_Part1->SetForceSolid(true);
  LV_PMT_Body_Part1->SetVisAttributes(Vis_PMT_Body_Part1);

  G4VisAttributes* Vis_PMT_Body_Part2 = new G4VisAttributes(white); //Black
  Vis_PMT_Body_Part2->SetVisibility(true);
  Vis_PMT_Body_Part2->SetForceSolid(true);
  LV_PMT_Body_Part2->SetVisAttributes(Vis_PMT_Body_Part2);



  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;


}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Useless now //
// void DetectorConstruction::DefineCommands()
// {
//     // Define //detector command directory using generic messenger class
//     fMessenger = new G4GenericMessenger(this,
//                                         "/B3/detector/",
//                                         "Detector control");
//
//
//
//     //Example  armAngle command
// //     G4GenericMessenger::Command& armAngleCmd
// //       = fMessenger->DeclareMethodWithUnit("armAngle","deg",
// //                                   &DetectorConstruction::SetArmAngle,
// //                                   "Set rotation angle of the second arm.");
// //     armAngleCmd.SetParameterName("angle", true);
// //     armAngleCmd.SetRange("angle>=0. && angle<180.");
// //     armAngleCmd.SetDefaultValue("30.");
//
// //      positionBeam command
//      G4GenericMessenger::Command& TargetPositionCmd
//       = fMessenger->DeclareMethod("TargetPosition",&DetectorConstruction::SetPatient_translation);
//
//     TargetPositionCmd.SetParameterName("bp", true);
//     TargetPositionCmd.SetDefaultValue("100.");
//
// // don't work !!!
//     G4GenericMessenger::Command& TargetMaterialCmd
//       = fMessenger->DeclareProperty("Material_Target",fMaterial_Target);
//
//     TargetMaterialCmd.SetParameterName("bpp", true);
//     TargetMaterialCmd.SetDefaultValue(1);
//
//
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::DefineMaterials()
{
  // 2 Methods to define G4Element
  // via G4NistManager

  G4NistManager* man = G4NistManager::Instance();

  G4bool isotopes = false;


  G4Element*  O = man->FindOrBuildElement("O" , isotopes);
  G4Element* Si = man->FindOrBuildElement("Si", isotopes);
  G4Element* Lu = man->FindOrBuildElement("Lu", isotopes);
  G4Element* Y = man->FindOrBuildElement("Y", isotopes);
  G4Element* Ce = man->FindOrBuildElement("Ce", isotopes);
  G4Element* H = man->FindOrBuildElement("H", isotopes);
  G4Element* C = man->FindOrBuildElement("C", isotopes);
  G4Element* N = man->FindOrBuildElement("N", isotopes);   //Define Nitrogen
  G4Element* B = man->FindOrBuildElement("B", isotopes);

  //   G4Material* mat = manager->ConstructNewMaterial("name" ,const std::vector<G4int>& Z, const std::vector<G4double>& weight, G4double density, G4bool isotopes);
  //   G4double isotopeMass = manager->GetMass(G4int Z, G4int N);

  // By       Define each individual element

  //Define each individual element not use here

  G4double A = 14.01 * g/mole;
  G4double Z = 7;
  G4Element* elN = new G4Element ("Nitrogen", "N", Z, A);

  //Define Adamantium // Non-physical heavy test material
  A = 207 * g/mole;
  Z = 82;
  G4Element* elAdamantium = new G4Element ("Adm", "Ad", Z, A);


  G4Material* LSO = new G4Material("Lu2SiO5", 7.4*g/cm3, 3);
  LSO->AddElement(Lu, 2);
  LSO->AddElement(Si, 1);
  LSO->AddElement(O , 5);


  G4Material *ADM = new G4Material("ADM", 10000*g/cm3,  1);
  ADM->AddElement(elAdamantium, 100*perCent);

  /*
   * G4Material *LYSO = new G4Material("LYSO", density=7.4*g/cm3, nel = 4);
   * LYSO->AddElement(Lu, 71.45*perCent);
   * LYSO->AddElement(Si, 6.37*perCent);
   * LYSO->AddElement(O, 18.15*perCent);
   * LYSO->AddElement(Y, 4.03*perCent);
   */

  G4Material *LYSO = new G4Material("LYSO", 7.4*g/cm3,  4);
  LYSO->AddElement(Lu, 71*perCent);
  LYSO->AddElement(Si, 7*perCent);
  LYSO->AddElement(O, 18*perCent);
  LYSO->AddElement(Y, 4*perCent);

  G4Material *LYSO_Ce = new G4Material("LYSO_Ce",7.4*g/cm3, 2);
  LYSO_Ce->AddMaterial(LYSO, 99.81*perCent);
  LYSO_Ce->AddElement(Ce, 0.19*perCent);

  G4Material *POM_C = new G4Material("POM_C", 1.41*g/cm3,  3);
  POM_C->AddElement(C, 25*perCent);
  POM_C->AddElement(H, 50*perCent);
  POM_C->AddElement(O, 25*perCent);

  //Define PMMA (C502H8)
  // NIST reference
  G4Material* PMMA = new G4Material("PMMA", 1.19*g/cm3, 3);
  PMMA -> AddElement(C, 5);
  PMMA -> AddElement(H, 8);
  PMMA -> AddElement(O, 2);

  //define water
  G4Material* water = new G4Material("water", 1*g/cm3, 2);
  water -> AddElement(H, 2);
  water -> AddElement(O, 1);

  // Bore
  G4double a; //masse atomique
  a=10.010*g/mole;
  G4Isotope* B10=new G4Isotope("B10", 5., 10, a);
  a=11.009*g/mole;
  G4Isotope* B11=new G4Isotope("B11", 5., 11, a);

  G4Element* elB=new G4Element("bore","B",2);
  elB->AddIsotope(B10, 96.*perCent);
  elB->AddIsotope(B11, 4.*perCent);

  G4Material* BORE = new G4Material("BORE", 3*g/cm3, 1);
  BORE->AddElement(B, 100*perCent);

  G4Material* BORE_PMMA = new G4Material("BORE_PMMA", 1*g/cm3, 2);
  BORE_PMMA->AddElement(B, 50*perCent);
  BORE_PMMA->AddMaterial(PMMA, 50*perCent);

  //   // Example to denifne difference abundance
  //   G4Isotope* isoU235 = new G4Isotope("U235", z=92, a=235, 235.044*g/mole);
  //   G4Isotope* isoU238 = new G4Isotope("U238", z=92, a=238, 238.051*g/mole);
  //   //Then define the corresponding enriched uranium element from the two isotopes
  //   G4Element* el_enrichedU = new G4Element("enriched U", "U", ncomp=2);
  //   el_enrichedU->AddIsotope(isoU235, abundance=5.*perCent);
  //   el_enrichedU->AddIsotope(isoU238, abundance=95.*perCent);
  //
  //   G4Element* elF = new G4Element(”fluorine", “F", 9.,18.998*g/mole);
  //   G4Material* fuel =  new G4Material(”nuclear fuel”, density=5.09*g/cm3, ncomp=2,    kStateSolid, 640*kelvin, 1.5e7*pascal);
  //   fuel->AddElement(el_enrichedU, 1);
  //   fuel->AddElement(elF, 6);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......




//--------- Visualization attributes -------------------------------
/*
 0 **0547 //   G4VisAttributes * Roux       = new G4VisAttributes( G4Colour(204/255. ,0/255.   ,51/255.  ));
 00548
 00549 //   G4VisAttributes * Brown      = new G4VisAttributes( G4Colour(255/255. ,153/255. ,153/255. ));
 00550
 00551 //   G4VisAttributes * Metal      = new G4VisAttributes( G4Colour(204/255. ,204/255. ,255/255. ));
 00552
 00553 //   G4VisAttributes * Bleu       = new G4VisAttributes( G4Colour(0/255.   ,0/255.   ,255/255. ));
 00554
 00555 //   G4VisAttributes * Violet     = new G4VisAttributes( G4Colour(153/255. ,0/255.   ,255/255. ));
 00556
 00557 //   G4VisAttributes * Gray       = new G4VisAttributes( G4Colour(153/255. ,153/255. ,153/255. ));
 00558
 00559   G4VisAttributes * Red        = new G4VisAttributes( G4Colour(255/255. ,0/255.   ,0/255.   ));
 00560
 00561   G4VisAttributes * Yellow     = new G4VisAttributes( G4Colour(255/255. ,255/255. ,0/255.   ));
 00562
 00563   G4VisAttributes * LightBleu  = new G4VisAttributes( G4Colour(0/255.   ,204/255. ,204/255. ));
 00564
 00565   G4VisAttributes * LightGreen = new G4VisAttributes( G4Colour(153/255. ,255/255. ,153/255. ));
 00566


 G4Colour  white   ()              ;  // white
 G4Colour  white   (1.0, 1.0, 1.0) ;  // white
 G4Colour  gray    (0.5, 0.5, 0.5) ;  // gray
 G4Colour  black   (0.0, 0.0, 0.0) ;  // black
 G4Colour  red     (1.0, 0.0, 0.0) ;  // red
 G4Colour  green   (0.0, 1.0, 0.0) ;  // green
 G4Colour  blue    (0.0, 0.0, 1.0) ;  // blue
 G4Colour  cyan    (0.0, 1.0, 1.0) ;  // cyan
 G4Colour  magenta (1.0, 0.0, 1.0) ;  // magenta
 G4Colour  yellow  (1.0, 1.0, 0.0) ;  // yellow



 */
