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
// $Id: DPGACrystalHit.hh 76474 2013-11-11 10:36:34Z gcosmo $
//
/// \file DPGACrystalHit.hh
/// \brief Definition of the DPGACrystalHit class

#ifndef DPGACrystalHit_h
#define DPGACrystalHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4GenericMessenger;
class G4AttDef;
class G4AttValue;

/// EM Calorimeter hit
///
/// It records:
/// - the cell ID
/// - the energy deposit 
/// - the cell logical volume, its position and rotation

class DPGACrystalHit : public G4VHit
{
public:
    DPGACrystalHit();
    DPGACrystalHit(G4int z);
    DPGACrystalHit(const DPGACrystalHit &right);
    virtual ~DPGACrystalHit();

    const DPGACrystalHit& operator=(const DPGACrystalHit &right);
    int operator==(const DPGACrystalHit &right) const;
    
    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    
    virtual void Draw();
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void Print();
    
    // Edep ! warning 2 methods post - pre step or GetTotalEnergyDeposit
    void SetEdep(G4double de) { fEdep = de; }
    void AddEdep(G4double de) { fEdep += de; }
    G4double GetEdep() const { return fEdep; }
    
    void SetEdep_M2(G4double de2) { fEdep_M2 = de2; }
    G4double GetEdep_M2() const { return fEdep_M2; }
    
    // info about Geometry
    void SetPhysicalName(G4String PhysicalName) { fPhysicalName = PhysicalName; }
    G4String GetPhysicalName() const { return fPhysicalName; }
    
    void SetLogV(G4LogicalVolume* val) { fPLogV = val; }
    const G4LogicalVolume* GetLogV() const { return fPLogV; }
    
    // Full DPGA Only ! 
    void SetNo_Copy(G4int No_Copy) { fNo_Copy = No_Copy; }
    G4int GetNo_Copy() const { return fNo_Copy; }
    
    void SetNo_Line(G4int No_Line) { fNo_Line = No_Line; }
    G4int GetNo_Line() const { return fNo_Line; }
    
    
    //  Local and World Positions
    void SetLocalPos(G4ThreeVector xyz) { fLocalPos = xyz; }
    G4ThreeVector GetLocalPos() const { return fLocalPos; }
    void SetWorldPos(G4ThreeVector xyz) { fWorldPos = xyz; }
    G4ThreeVector GetWorldPos() const { return fWorldPos; }
    
    //  Local and World Vertex Positions
   void SetLocalPos_Vertex(G4ThreeVector xyz) { fLocalPos_Vertex = xyz; }
   G4ThreeVector GetLocalPos_Vertex() const { return fLocalPos_Vertex; }
    void SetWorldPos_Vertex(G4ThreeVector xyz) { fWorldPos_Vertex = xyz; }
    G4ThreeVector GetWorldPos_Vertex() const { return fWorldPos_Vertex; }
    
    void SetCrystal_Position(G4ThreeVector xyz) { fCrystal_Position = xyz; }
    G4ThreeVector GetCrystal_Position() const { return fCrystal_Position; }
    
    
    
    // Time Info
    void SetTime(G4double t) { fTime = t; }
    G4double GetTime() const { return fTime; }
    
    // Tracking info
    
	// KineticEnergy 
	
	void SetKineticEnergy(G4double KineticEnergy) { fKineticEnergy = KineticEnergy; }
	G4double GetKineticEnergy() const { return fKineticEnergy; }
	
	void SetKineticEnergy_Vertex(G4double KineticEnergyV) { fKineticEnergy_Vertex = KineticEnergyV; }
	G4double GetKineticEnergy_Vertex() const { return fKineticEnergy_Vertex; }
	
        /// methods to handle data 
	  inline void SetParentID(G4int parentID){fParentID = parentID;}
	  inline void SetTrackID(G4int trackID){fTrackID = trackID;}
	  inline void SetPDGCode(G4int PDGCode){fPDGCode = PDGCode;}
	  inline void SetProcessName(G4String ProcessName){fProcessName = ProcessName;}
	  inline void SetParentProcessName(G4String parentProcessName){fParentProcessName = parentProcessName;}
	  inline void SetCreatorProcessName(G4String creatorProcessName){fCreatorProcessName = creatorProcessName;}
	  inline void SetParticleName(G4String particleName){fParticleName = particleName;}
	/// get methods
	  inline G4int GetParentID() const{return fParentID;}
	  inline G4int GetTrackID() const{return fTrackID;}
	  inline G4int GetPDGCode() const{return fPDGCode;}
	  inline G4String GetProcessName() const{return fProcessName;}
	  inline G4String GetParentProcessName() const{return fParentProcessName;}
	  inline G4String GetCreatorProcessName() const{return fCreatorProcessName;}
	  inline G4String GetParticleName() const{return fParticleName;}
	    
	   // SecondaryParticleColl
           //void SetIncidenteParticle(Particle * incidenteParticle);
           //void SetScatteredParticle(Particle * scatteredParticle);
           //void SetSecondaryParticle(Particle * secondaryParticle);
           
	    //inline Particle * GetIncidenteParticle(){return fIncidenteParticle;}
	    //inline Particle * GetScatteredParticle(){return fScatteredParticle;}
	    //inline vector<Particle*> GetSecondaryParticleColl(){return fSecondaryParticleColl;}
    
    
    // useless at this time
    void SetCellID(G4int z) { fCellID = z; }
    G4int GetCellID() const { return fCellID; }
    void SetRot(G4RotationMatrix rmat) { fRot = rmat; }
    G4RotationMatrix GetRot() const { return fRot; }
    

    
    
private:
    G4int fCellID;
    
    G4double fEdep;
    G4double fEdep_M2; 
    
    G4double fKineticEnergy; // After A process (ie At postStep Point)
    G4double fKineticEnergy_Vertex;
    
    G4ThreeVector fPos;
    G4RotationMatrix fRot;
    const G4LogicalVolume* fPLogV;
    
    G4double fTime;
    
    G4ThreeVector fLocalPos; 
    G4ThreeVector fWorldPos;
    
    G4ThreeVector fCrystal_Position;
    
    G4ThreeVector fLocalPos_Vertex; 
    G4ThreeVector fWorldPos_Vertex;
    
    G4String   fPhysicalName; // name of sensitive volume
    
    G4int   fParentID;   
    G4int   fTrackID;
    G4int   fPDGCode;
    G4String   fProcessName;
    G4String   fParentProcessName;
    G4String   fCreatorProcessName;
    
    G4String   fParticleName;
    
    //Particle * fIncidenteParticle;
    //Particle * fScatteredParticle;
    //vector<Particle*> fSecondaryParticleColl;
    
    G4int	fNo_Copy; // No copy PMT
    G4int	fNo_Line; // No line Cassette
};

typedef G4THitsCollection<DPGACrystalHit> DPGACrystalHitsCollection;

extern G4ThreadLocal G4Allocator<DPGACrystalHit>* DPGACrystalHitAllocator;

inline void* DPGACrystalHit::operator new(size_t)
{
    if (!DPGACrystalHitAllocator)
        DPGACrystalHitAllocator = new G4Allocator<DPGACrystalHit>;
    return (void*)DPGACrystalHitAllocator->MallocSingle();
}

inline void DPGACrystalHit::operator delete(void* aHit)
{
    DPGACrystalHitAllocator->FreeSingle((DPGACrystalHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

