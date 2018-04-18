#ifndef CathodeHit_h
#define CathodeHit_h 1

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

class CathodeHit : public G4VHit
{
public:
    CathodeHit();
    CathodeHit(const CathodeHit &right);
    virtual ~CathodeHit();

    const CathodeHit& operator=(const CathodeHit &right);
    int operator==(const CathodeHit &right) const;
    
    inline void *operator new(size_t);
    inline void operator delete(void *aHit);
    
    virtual void Draw();
    virtual void Print();
    
    void SetLocalTime(G4double time) {fLocalTime = time;};
    void SetGlobalTime(G4double time) {fGlobalTime = time;};
    
    G4double GetLocalTime() {return fLocalTime;};
    G4double GetGlobalTime() {return fGlobalTime;};
    
private:
    G4double fLocalTime;
    G4double fGlobalTime;
};

typedef G4THitsCollection<CathodeHit> CathodeHits;

extern G4ThreadLocal G4Allocator<CathodeHit>* CathodeHitAllocator;

inline void* CathodeHit::operator new(size_t)
{
    if (!CathodeHitAllocator)
        CathodeHitAllocator = new G4Allocator<CathodeHit>;
    return (void*)CathodeHitAllocator->MallocSingle();
}

inline void CathodeHit::operator delete(void* aHit)
{
    CathodeHitAllocator->FreeSingle((CathodeHit*) aHit);
}

#endif


    
    
