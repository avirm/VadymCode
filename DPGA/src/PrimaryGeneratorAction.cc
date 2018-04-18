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
// $Id: PrimaryGeneratorAction.cc 73766 2013-09-10 12:57:13Z gcosmo $
//
/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ChargedGeantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4ParticleDefinition.hh"
#include "G4GenericMessenger.hh"

#include "G4GeneralParticleSource.hh"
  
#include "G4ios.hh"
#include <G4UnitsTable.hh>
using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),     
  m_particleGun(0), fMessenger(0),fOneTimePrintInfo(false), fParticulesCounter(0),Test(0),fDrawBunchProgress("")
{
 // define commands for this class
 DefineCommands();
 fTime_global = 0*ns;
 
 // Initialisation GPS
  m_particleGun = new G4GeneralParticleSource();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete m_particleGun;
  delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if(fOneTimePrintInfo==false){
  cout << "******************************************************" << endl;
  cout << "    Time Mod PrimaryGeneratorAction                   " << endl;
  cout << "******************************************************" << endl;
    
  }
  
  m_particleGun->GeneratePrimaryVertex(anEvent);
 
//   G4cout << "coucou Test value = " << Test << endl;
//     Test+=1;
    
//    fTimeStructure =2; // <= 0 any / 1 radioactive decay / 2 Bunched Beam / 3 MACRO Bunched Time not implemented yet
   
  
   
   // Parameters 
    //Selection 1
//      fDecayTauPrimmary = 0 *ns;
    //Selection 2 
//       G4double BurstPeriod = 0;
//       G4double BunchPerBurst = 0;
      
      // Bunch setup
//        fBunchPeriod = 40 *ns;
//        fBunchParticulesTimeDistribution = 6 *ns;
//        fParticulesPerBunch = 100; // intensity Ampere 
      
//       fTimeStructure = 0;
//       G4cout << "ParticulesCounter " << fParticulesCounter << endl;
      
   if(fTimeStructure ==1){
    
    if(fOneTimePrintInfo==false){cout << "TimeStructure = " << fTimeStructure << endl;/*fOneTimePrintInfo= true;*/ 
      
      cout << "fDecayTauPrimmary" << fDecayTauPrimmary /s << endl; 
    }
    
    fTime_global += G4RandExponential::shoot(fDecayTauPrimmary); 
     
   }
   else if(fTimeStructure==2){
     
     if(fOneTimePrintInfo==false){cout << "TimeStructure = " << fTimeStructure << " -> Beam mod "  << endl; 
       
       if(fParticulesPerBunch < 0){ 
	 cout << "You enter a negativ fParticulesPerBunch -> I consider like a Physique intensity in nA" << endl;
	 fParticulesPerBunch = ((-1*(fParticulesPerBunch *1e-9) / 1.6e-19)) / 24.8e6;
      }
       
       G4double MedycicIntensity = fParticulesPerBunch * 24.8e6 * 1.6e-19 * 1e9;
       //I = Particules per bunch * Frequence (Medycic 24.8MHz) * Coulomb constant
       cout << "###Bunch Parameters ###" << endl; 
       cout << "BunchPeriod " << G4BestUnit(fBunchPeriod, "Time")
       << "\nBunchParticulesTimeDistribution " << G4BestUnit(fBunchParticulesTimeDistribution, "Time") 
       << "\nParticulesPerBunch " << fParticulesPerBunch 
       << "\n -> Medycic Intensity (Physique | DPGA) " << std::setprecision(2) <<  MedycicIntensity << " nA"
       << "\n -> Medycic Intensity (Cyclotron Nice ) " << std::setprecision(2) <<  MedycicIntensity *2 << " nA"
       << "\n -> No of Beta+ per Bunch (65 MeV) -> "   << std::setprecision(4) <<(G4double)fParticulesPerBunch /163  
       << endl;
    }
     
     if(fOneTimePrintInfo==false){fTime_global =0.0;}
     else{
//        fTime_global += G4RandExponential::shoot(fBunchParticulesTimeDistribution);
       
       fTime_global += fBunchParticulesTimeDistribution/ fParticulesPerBunch ;
       
       if(fParticulesCounter == fParticulesPerBunch){
	 fTime_global += fBunchPeriod;
// 	 fDrawBunchProgress += "#   ";
// 	 cout << fDrawBunchProgress << endl;
	 fParticulesCounter = 0;
       }
     }
     fParticulesCounter +=1;
//      fOneTimePrintInfo= true;
     
//      cout << "Time Global  " << fTime_global << endl;
   }
    else if(fTimeStructure == 0){
     if(fOneTimePrintInfo==false){G4cout << "TimeStructure = " << fOneTimePrintInfo << endl;/*fOneTimePrintInfo= true; */
     cout << "G4 Time Event by Event (Default Mod) !" << endl;
     }
    }
   else{
     cout << "fTimeStructure selected _ " << fTimeStructure << "_ not implemented yet exit(0)" <<endl;
     exit(0);
   }

 
//  G4cout <<"Name " << m_particleGun->GetParticleDefinition()->GetParticleName() << endl;
//  G4cout << "Time before " << m_particleGun->GetParticleTime() /ns << endl;
//  G4cout <<  m_particleGun->GetNumberOfParticles() << endl;
    
    
    m_particleGun->SetParticleTime(fTime_global);
    
//  G4cout << "Calculated Time " << m_particleGun->GetParticleTime() /ns << "ns" << endl <<endl;

  if(fOneTimePrintInfo==false){
  cout << "******************************************************" << endl;
  cout  << endl;
  fOneTimePrintInfo=true;
  }
   
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::DefineCommands()
{
    // Define //generator command directory using generic messenger class
    fMessenger 
      = new G4GenericMessenger(this, 
                               "/B3/generator/", 
                               "Primary generator control");
      
    //Bunch Parameters
    
    // Bunch Period
    G4GenericMessenger::Command& fBunchPeriodCmd
      = fMessenger->DeclarePropertyWithUnit("BunchPeriod", "ns", fBunchPeriod);
    fBunchPeriodCmd.SetParameterName("BP", true);
    fBunchPeriodCmd.SetRange("BP>=0.");                                
    fBunchPeriodCmd.SetDefaultValue("40.");
      
    G4GenericMessenger::Command& fBunchParticulesTimeDistributionCmd
      = fMessenger->DeclarePropertyWithUnit("BunchParticulesTimeDistribution", "ns", fBunchParticulesTimeDistribution, 
                                    "fBunchParticules Time Distribution");
    fBunchParticulesTimeDistributionCmd.SetParameterName("BPP", true);
    fBunchParticulesTimeDistributionCmd.SetRange("BPP>=0.");                                
    fBunchParticulesTimeDistributionCmd.SetDefaultValue("5.");
    
    G4GenericMessenger::Command& fParticulesPerBunchCmd
     = fMessenger->DeclareProperty("ParticulesPerBunch", fParticulesPerBunch, 
                                    "fBunchParticules Time Distribution");
    fParticulesPerBunchCmd.SetParameterName("BPPP", true);
//     fParticulesPerBunchCmd.SetRange("BPPP>=0");                                
    fParticulesPerBunchCmd.SetDefaultValue("10");
    
        G4GenericMessenger::Command& fTimeStructureCmd
     = fMessenger->DeclareProperty("fTimeStructure", fTimeStructure);
    fTimeStructureCmd.SetParameterName("BPX", true);                             
    fTimeStructureCmd.SetDefaultValue("0");

    
        G4GenericMessenger::Command& fDecayTauPrimmaryCmd
     = fMessenger->DeclarePropertyWithUnit("DecayTauPrimmary","s" ,fDecayTauPrimmary, 
                                    "f Decay Tau Primmary // demi vie");
    fDecayTauPrimmaryCmd.SetParameterName("BPxx", true);
    fDecayTauPrimmaryCmd.SetRange("BPxx>=0");                                
    fDecayTauPrimmaryCmd.SetDefaultValue("0.0");    
    
     // Bunch Particules Time Distribution
//     G4GenericMessenger::Command& fBunchParticulesTimeDistributionCmd
//       = fMessenger->DeclarePropertyWithUnit("fBunchPeriod", "ns", fBunchPeriod, 
//                                     "Bunch Period");
//     fBunchParticulesTimeDistributionCmd.SetParameterName("BP", true);
//     fBunchParticulesTimeDistributionCmd.SetRange("BP>=0.");                                
//     fBunchParticulesTimeDistributionCmd.SetDefaultValue("40."); 
    
    
//     // momentum command
//     G4GenericMessenger::Command& momentumCmd
//       = fMessenger->DeclarePropertyWithUnit("momentum", "GeV", fMomentum, 
//                                     "Mean momentum of primaries.");
//     momentumCmd.SetParameterName("p", true);
//     momentumCmd.SetRange("p>=0.");                                
//     momentumCmd.SetDefaultValue("1.");
//     // ok
//     //momentumCmd.SetParameterName("p", true);
//     //momentumCmd.SetRange("p>=0.");                                
//     
//     // sigmaMomentum command
//     G4GenericMessenger::Command& sigmaMomentumCmd
//       = fMessenger->DeclarePropertyWithUnit("sigmaMomentum",
//           "MeV", fSigmaMomentum, "Sigma momentum of primaries.");
//     sigmaMomentumCmd.SetParameterName("sp", true);
//     sigmaMomentumCmd.SetRange("sp>=0.");                                
//     sigmaMomentumCmd.SetDefaultValue("50.");
// 
//     // sigmaAngle command
//     G4GenericMessenger::Command& sigmaAngleCmd
//       = fMessenger->DeclarePropertyWithUnit("sigmaAngle", "deg", fSigmaAngle, 
//                                     "Sigma angle divergence of primaries.");
//     sigmaAngleCmd.SetParameterName("t", true);
//     sigmaAngleCmd.SetRange("t>=0.");                                
//     sigmaAngleCmd.SetDefaultValue("2.");
// 
//     // randomizePrimary command
//     G4GenericMessenger::Command& randomCmd
//       = fMessenger->DeclareProperty("randomizePrimary", fRandomizePrimary);
//     G4String guidance
//        = "Boolean flag for randomizing primary particle types.\n";   
//     guidance
//        += "In case this flag is false, you can select the primary particle\n";
//     guidance += "  with /gun/particle command.";                               
//     randomCmd.SetGuidance(guidance);
//     randomCmd.SetParameterName("flg", true);
//     randomCmd.SetDefaultValue("true");
//     
//     // positionBeam command
//      G4GenericMessenger::Command& BeamPositionCmd
//       = fMessenger->DeclareProperty("BeamPosition", fBeamPosition);
//                                
//     BeamPositionCmd.SetParameterName("bp", true);
//     BeamPositionCmd.SetDefaultValue("0.,0.,0.*m");
//     
//       // Source Ion
//      G4GenericMessenger::Command& bool_IonCmd
//       = fMessenger->DeclareProperty("bool_Ion", fbool_Ion);
//                                
//     bool_IonCmd.SetParameterName("bion", true);
//     bool_IonCmd.SetDefaultValue("false");
    
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
