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
// $Id: B3DetectorConstruction.hh 71079 2013-06-10 20:37:11Z ihrivnac $
//
/// \file B3DetectorConstruction.hh
/// \brief Definition of the B3DetectorConstruction class

#ifndef B3DetectorConstruction_h
#define B3DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "TrackerSD.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4UserLimits;

/// Detector construction class to define materials and geometry.
///
/// Crystals are positioned in Ring, with an appropriate rotation matrix. 
/// Several copies of Ring are placed in the full detector.

class B3DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    B3DetectorConstruction();
    virtual ~B3DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
    
    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    void SetMaxStep (G4double );
               
  private:
    void DefineMaterials();
    G4bool  fCheckOverlaps;
    G4UserLimits* fStepLimit;
    
  protected:
    G4LogicalVolume*  fScoringVolume;
    G4LogicalVolume*  fLV_crystal;
    G4LogicalVolume*  fLV_cathode;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
