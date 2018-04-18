To activate optical physics:
============================
***
In `PhysicsList.cc` add next lines:

```c++
#include "G4OpticalPhysics.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"
```
and in `PhysicsList.cc` in function *B3PhysicsList::B3PhysicsList()
: G4VModularPhysicsList()* :
```c++
// Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics( opticalPhysics );

  opticalPhysics->SetWLSTimeProfile("delta");

  opticalPhysics->SetScintillationYieldFactor(1.0);
  opticalPhysics->SetScintillationExcitationRatio(0.0);

  opticalPhysics->SetMaxNumPhotonsPerStep(1000);
  opticalPhysics->SetMaxBetaChangePerStep(100.0);

  opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);
```
***

Set optical properties of materials and define optical surfaces:
================================================================
***

1. In `DetectorConstruction.cc` add next lines:

```c++
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
```
2. Define optical properties of scintillation material:
    (as a tables of values energy-value, **by default interpolation is linear**)
    (all values sent by Arthur)
```c++
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
```
3. Define optical properties of another material (air in this case):
```c++
  G4double energy[]    = { 1.84*eV , 6.95*eV};
  G4double RIND_air[]  = { 1.000277, 1.000277 };
  G4double ABSL_air[]  = { 50*m, 50*m };
  auto propertiesTable_world = new G4MaterialPropertiesTable();
  propertiesTable_world->AddProperty("ABSLENGTH",     energy, ABSL_air,  num);
  propertiesTable_world->AddProperty("RINDEX",        energy, RIND_air,  num);

  world_material->SetMaterialPropertiesTable(propertiesTable_world);
```
4. Define optical surfaces (for crystal wrap in this case):
```c++
  G4OpticalSurface* crystWrap = G4OpticalSurface("crystWrap");

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
```
  1. Create G4OpticalSurface.
  2. Set model, type and finish:
  ```c++
  crystWrap->SetType(dielectric_metal);
  crystWrap->SetModel(unified);
  crystWrap->SetFinish(polished);
  ```
    * Type `dielectric_metal`: only reflection or absorption (**NO REFRACTION**).
    * Type `dielectric_dielectric`: reflection, refraction or absorbtion.
  3. Set optical properties like for materials.
  4. Create new 
    * G4LogicalBorderSurface (name, physical volume1, physical volume1, optical surface) (surface between 2 physical volumes);
    * G4LogicalSkinSurface (name, LOGICAL volume, optical surface) **Important:** in this case all sides of volume have same optical properties. 
