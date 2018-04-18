#include "CathodeHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<CathodeHit>* CathodeHitAllocator=0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CathodeHit::CathodeHit()
 : G4VHit(),
   fLocalTime(0.),
   fGlobalTime(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CathodeHit::~CathodeHit() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CathodeHit::CathodeHit(const CathodeHit& right)
  : G4VHit()
{
  fLocalTime      = right.fLocalTime;
  fGlobalTime      = right.fGlobalTime;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const CathodeHit& CathodeHit::operator=(const CathodeHit& right)
{
  fLocalTime      = right.fLocalTime;
  fGlobalTime      = right.fGlobalTime;

  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int CathodeHit::operator==(const CathodeHit& right) const
{
  return ( this == &right ) ? 1 : 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CathodeHit::Draw()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CathodeHit::Print()
{
}
