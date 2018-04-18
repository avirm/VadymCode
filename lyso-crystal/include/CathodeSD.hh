#ifndef CathodeSD_h
#define CathodeSD_h 1

#include "G4VSensitiveDetector.hh"
#include <G4ParticleDefinition.hh>
#include "CathodeHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class G4GenericMessenger;

class CathodeSD : public G4VSensitiveDetector
{
    
public:
    CathodeSD(G4String name);
    virtual ~CathodeSD();
    inline void DefineCommands();
    virtual void Initialize(G4HCofThisEvent*HCE);
    virtual G4bool ProcessHits(G4Step*aStep,G4TouchableHistory*ROhist);
    //virtual G4bool ProcessHits_constStep(G4Step*aStep,G4TouchableHistory*);
    void EndOfEvent(G4HCofThisEvent*);
    
private:
    CathodeHits* fHitsCollection;
};

#endif 
