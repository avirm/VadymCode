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
// $Id: B3DetectorConstruction.cc 101905 2016-12-07 11:34:39Z gunter $
//
/// \file B3DetectorConstruction.cc
/// \brief Implementation of the B3DetectorConstruction class

#include "B3DetectorConstruction.hh"
#include "TrackerSD.hh"
#include "CathodeSD.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"
#include "G4UserLimits.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
//#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
//#include "G4RotationMatrix.hh"
//#include "G4Transform3D.hh"
//#include "G4MultiFunctionalDetector.hh"
//#include "G4VPrimitiveScorer.hh"
//#include "G4PSEnergyDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3DetectorConstruction::B3DetectorConstruction()
: G4VUserDetectorConstruction(),
  fCheckOverlaps(true),
  fStepLimit(NULL),
  fLV_crystal(NULL),
  fLV_cathode(NULL)
{
  DefineMaterials();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B3DetectorConstruction::~B3DetectorConstruction()
{
  delete fStepLimit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B3DetectorConstruction::DefineMaterials()
{
  G4NistManager* man = G4NistManager::Instance();

  G4bool isotopes = false;

  G4Element*  O = man->FindOrBuildElement("O" , isotopes);
  G4Element* Si = man->FindOrBuildElement("Si", isotopes);
  G4Element* Lu = man->FindOrBuildElement("Lu", isotopes);
  G4Element* Y  = man->FindOrBuildElement("Y",  isotopes);
  G4Element* Ga  = man->FindOrBuildElement("Ga",  isotopes);
  G4Element* As  = man->FindOrBuildElement("As",  isotopes);


  G4Material *LYSO = new G4Material("LYSO", 7.4*g/cm3,  4);
  LYSO->AddElement(Lu, 71*perCent);
  LYSO->AddElement(Si, 7*perCent);
  LYSO->AddElement(O, 18*perCent);
  LYSO->AddElement(Y, 4*perCent);

  G4Material* GaAs = new G4Material("GaAs", 5.3176 * g/cm3, 2);
  GaAs->AddElement(Ga, 1);
  GaAs->AddElement(As, 1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B3DetectorConstruction::Construct()
{
  // Parameters
  // Crystal
  G4double crystal_XY = 13*mm, crystal_Z = 15*mm;
  //
  // Materials
  //
  G4NistManager* nist = G4NistManager::Instance();

  G4Material* crystal_material  = nist->FindOrBuildMaterial("LYSO");
  G4Material* world_material    = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* teflon = nist->FindOrBuildMaterial("G4_TEFLON");
  G4Material* photoCathode_mat = nist->FindOrBuildMaterial("GaAs");

  // Optical properties
  G4double energy[]    = { 1.84*eV , 6.95*eV};
  const G4int num = sizeof(energy)/sizeof(G4double);

  G4double RIND[]  = { 1.82, 1.82 };
  G4double ABSL[]  = { 1.49*cm, 1.49*cm };

  G4double fast_comp[] = {1., 1.};

  auto propertiesTable = new G4MaterialPropertiesTable();
  propertiesTable->AddProperty("RINDEX",        energy, RIND,  num);
  propertiesTable->AddProperty("ABSLENGTH",     energy, ABSL,  num);
  propertiesTable->AddProperty("FASTCOMPONENT", energy, fast_comp,  num);
  propertiesTable->AddConstProperty("SCINTILLATIONYIELD",33000/MeV);
  propertiesTable->AddConstProperty("RESOLUTIONSCALE",4.1);
  propertiesTable->AddConstProperty("FASTTIMECONSTANT",40.*ns);
  propertiesTable->AddConstProperty("YIELDRATIO",1.0);

  crystal_material->SetMaterialPropertiesTable(propertiesTable);
  //crystal_material->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  G4double RIND_air[]  = { 1.000277, 1.000277 };
  G4double ABSL_air[]  = { 50*m, 50*m };
  auto propertiesTable_world = new G4MaterialPropertiesTable();
  propertiesTable_world->AddProperty("ABSLENGTH",     energy, ABSL_air,  num);
  propertiesTable_world->AddProperty("RINDEX",        energy, RIND_air,  num);

  world_material->SetMaterialPropertiesTable(propertiesTable_world);

  G4double RIND_cat[]  = { 1.64, 1.64 };
  G4double ABSL_cat[]  = { 0.1*mm, 0.1*mm };

  auto propertiesTable_cat_mat = new G4MaterialPropertiesTable();
  propertiesTable_cat_mat->AddProperty("RINDEX",        energy, RIND_cat,  num);
  propertiesTable_cat_mat->AddProperty("ABSLENGTH",     energy, ABSL_cat,  num);
  photoCathode_mat->SetMaterialPropertiesTable(propertiesTable_cat_mat);

  //
  // World
  //
  G4double world_sizeXY = 13*cm;
  G4double world_sizeZ  = 15*cm;
  G4double PMT_sizeXY = 13*mm;
  G4double PMT_sizeZ = 15*mm;

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ); //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        world_material,      //its material
                        "WorldLV");          //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      fCheckOverlaps);       // checking overlaps

    //Teflon wrap(seperate block for each side of crystal)
    G4Box* solidTeflon = new G4Box("Teflon",
                    2.5*cm, 2.5*cm, 2.5*cm);

    G4LogicalVolume* logicTeflon =
                            new G4LogicalVolume(solidTeflon,        //its solid
                                teflon,    //its material
                                "TeflonLV");        //its name

    G4VPhysicalVolume* TeflonPV =  new G4PVPlacement(0,                       //no rotation
                                         G4ThreeVector(0, 0, 0),         //at (0,0,-10mm)
                                         logicTeflon,            //its logical volume
                                         "TeflonPV", //its name
                                         logicWorld,              //its mother  volume
                                         false,                   //no boolean operation
                                         0,                       //copy number
                                         fCheckOverlaps);         // checking overlaps


  //
  // define crystal
  //
  G4Box* solidCrystal = new G4Box("LYSO_crystal",    //its name
                            crystal_XY/2, crystal_XY/2, crystal_Z/2); //its size

  G4LogicalVolume* logicCrystal =
    new G4LogicalVolume(solidCrystal,        //its solid
                        crystal_material,    //its material
                        "CrystalLV");        //its name

  fLV_crystal = logicCrystal;

  // place crystal in world
  //
  G4VPhysicalVolume* crystalPV =  new G4PVPlacement(0,                       //no rotation
                                    G4ThreeVector(),         //at (0,0,0)
                                    logicCrystal,            //its logical volume
                                    "Solo crystal detector", //its name
                                    logicTeflon,              //its mother  volume logicWorld
                                    false,                   //no boolean operation
                                    0,                       //copy number
                                    fCheckOverlaps);         // checking overlaps

  G4Box* photoCathode = new G4Box("PT", PMT_sizeXY/2, PMT_sizeXY/2, PMT_sizeZ/5);

  G4LogicalVolume* logicPT = new G4LogicalVolume(photoCathode,
                                photoCathode_mat,
                                "PhotoCathode_L");
  fLV_cathode = logicPT;

  G4VPhysicalVolume* cathodePV = new G4PVPlacement(0,
                                 G4ThreeVector(0, 0, 10.5*mm),
                                 logicPT,
                                 "Photocathode",
                                 logicTeflon,
                                 false,
                                 0,
                                 fCheckOverlaps);

  //Print materials
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  G4OpticalSurface* cathodeSurface = new G4OpticalSurface("Cathode surface");

  cathodeSurface->SetType(dielectric_dielectric);
  cathodeSurface->SetModel(unified);
  cathodeSurface->SetFinish(polished);

  G4double RINDEX_PC[] = {1.64, 1.64};
  G4double REFLECTIVITY_PC[] = {0, 0};

  auto propertiesTable_photoCathode = new G4MaterialPropertiesTable();
  propertiesTable_photoCathode->AddProperty("RINDEX", energy, RINDEX_PC,  num);
  propertiesTable_photoCathode->AddProperty("REFLECTIVITY", energy, REFLECTIVITY_PC,  num);

  cathodeSurface->SetMaterialPropertiesTable(propertiesTable_photoCathode);

  new G4LogicalBorderSurface("Cathode surface", cathodePV,
                             crystalPV, cathodeSurface);

  G4OpticalSurface* crystWrap = new G4OpticalSurface("crystWrap");

  crystWrap->SetType(dielectric_metal);
  crystWrap->SetModel(unified);
  crystWrap->SetFinish(polished);

  G4double SPECULARLOBECONSTANT[] = {0, 0};
  G4double SPECULARSPIKECONSTANT[] = {0, 0};
  G4double BACKSCATTERCONSTANT[] = {0, 0};
  G4double RINDEX[] = {1, 1};
  G4double REFLECTIVITY[] = {0.95, 0.95};
  G4double EFFICIENCY[] = {0, 0};

  auto propertiesTable_wrap = new G4MaterialPropertiesTable();
  propertiesTable_wrap->AddProperty("SPECULARLOBECONSTANT", energy, SPECULARLOBECONSTANT,  num);
  propertiesTable_wrap->AddProperty("SPECULARSPIKECONSTANT", energy, SPECULARSPIKECONSTANT,  num);
  propertiesTable_wrap->AddProperty("BACKSCATTERCONSTANT", energy, BACKSCATTERCONSTANT,  num);
  propertiesTable_wrap->AddProperty("RINDEX", energy, RINDEX,  num);
  propertiesTable_wrap->AddProperty("REFLECTIVITY", energy, REFLECTIVITY,  num);
  propertiesTable_wrap->AddProperty("EFFICIENCY", energy, EFFICIENCY,  num);

  crystWrap->SetMaterialPropertiesTable(propertiesTable_wrap);
  new G4LogicalBorderSurface("crystWrap_left", crystalPV,
                             TeflonPV, crystWrap);

  // Set crystal color
  G4VisAttributes *crystalColor = new G4VisAttributes(G4Colour(1, 0, 0, 1));
  logicCrystal->SetVisAttributes(crystalColor);

  G4VisAttributes *cathodeColor = new G4VisAttributes(G4Colour(1, 1, 0, 1));
  logicPT->SetVisAttributes(cathodeColor);

  G4VisAttributes *teflonColor = new G4VisAttributes(G4Colour(1, 0, 1, 1));
  logicTeflon->SetVisAttributes(teflonColor);

  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B3DetectorConstruction::ConstructSDandField()
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->SetVerboseLevel(1);

  // declare crystal as a sensitive detector
  //
  G4VSensitiveDetector* crystal = new TrackerSD("tracker_crystal", "crystal_hits");
  SDman->AddNewDetector(crystal);
  fLV_crystal->SetSensitiveDetector(crystal);

  G4VSensitiveDetector* cath = new CathodeSD("cathode");
  SDman->AddNewDetector(cath);
  fLV_cathode->SetSensitiveDetector(cath);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
