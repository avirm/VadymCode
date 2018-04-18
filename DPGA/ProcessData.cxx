// TTree Reader
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TSelector.h"

// Basic include
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TFrame.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TBenchmark.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TLegend.h"

//c++ include
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>

// Global canvas
TCanvas* canvas;

void DrawAndSaveHistorgam(TH1F* histo, bool save = false, std::string imgName = "", double scale = 1.,
    std::string drawOpt = "BAR", int color = -1){
    if(color == -1)
        color = 4;

    histo->SetFillColor(color);
    histo->SetFillStyle(3001);

    if(scale == 1.)
        scale = histo->Integral();
    histo->Scale(100 / scale);
    histo->Draw(drawOpt.c_str());

    std::string path = "images/" + imgName;
    if(save)
        canvas->SaveAs(path.c_str());
}

void DrawAndSaveWithCopy(TH1F* histo, TPad* pad, double min, double max, std::string imgName){
    auto copyHist = (TH1F*) histo->Clone();
    copyHist->SetName("copy");
    copyHist->GetXaxis()->SetRangeUser(min, max);
    pad->Draw();
    pad->cd();
    copyHist->Draw("BAR");
    std::string path = "images/" + imgName;
    canvas->SaveAs(path.c_str());
    canvas->cd();
}

void DrawAndSave2DHist(TH2F* hist, std::string imgName){
    hist->Scale(100. / hist->Integral());
    hist->SetFillColor(4);
    hist->SetFillStyle(3001);
    hist->Draw("COLZ");
    std::string path = "images/" + imgName;
    canvas->SaveAs(path.c_str());
}

void ProcessData(){

    // Global style options and canvas with pads
    canvas = new TCanvas("canvas", "canvas", 1750, 1000);
    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);

    TPad* pad = new TPad("pad", "", 0.5, 0.3, 0.9, 0.8);
    TPad* pad2 = new TPad("pad2", "", 0.5, 0.3, 0.9, 0.8);
    TPad* pad3 = new TPad("pad3", "", 0.3, 0.2, 0.9, 0.8);
    TPad* pad4 = new TPad("pad4", "", 0.3, 0.2, 0.9, 0.8);
    TPad* pad5 = new TPad("pad5", "", 0.3, 0.2, 0.9, 0.8);
    canvas->SetGrid();
    pad->SetGrid();
    pad2->SetGrid();
    pad3->SetGrid();
    pad4->SetGrid();
    pad5->SetGrid();

    // TTreeReader
    TFile* dataFile = TFile::Open("data.root");
    TTree* crystalTree = (TTree*) dataFile->Get("Hits_Crystal");

    TTreeReader treeReader(crystalTree);

    std::cout << "Reading data..." << std::endl;

    // Edep
    TTreeReaderValue<Double_t> TotalEdep(treeReader, "TotalEdep");
    TTreeReaderArray<Double_t> Edep(treeReader, "Edep");
    TTreeReaderArray<Double_t> DeltaEnergy(treeReader, "DeltaEnergy");

    // Position
    TTreeReaderArray<Double_t> World_PosX(treeReader, "World_PosX");
    TTreeReaderArray<Double_t> World_PosY(treeReader, "World_PosY");
    TTreeReaderArray<Double_t> World_PosZ(treeReader, "World_PosZ");

    TTreeReaderArray<Double_t> Local_PosX(treeReader, "Local_PosX");
    TTreeReaderArray<Double_t> Local_PosY(treeReader, "Local_PosY");
    TTreeReaderArray<Double_t> Local_PosZ(treeReader, "Local_PosZ");

    // Parent ID // Track ID // PDG Code // Process Name //
    // Creation Process name // Crystal number //
    // PMT number
    TTreeReaderArray<Int_t> ParentID(treeReader, "ParentID");
    TTreeReaderArray<Int_t> TrackID(treeReader, "TrackID");
    TTreeReaderArray<Int_t> PDGCode(treeReader, "PDGCode");
    TTreeReaderArray<Int_t> ProcessName(treeReader, "ProcessName");
    TTreeReaderArray<Int_t> CreationProcessName(treeReader, "CreationProcessName");
    TTreeReaderArray<Int_t> CrystalNumber(treeReader, "Crystal_Number");
    TTreeReaderArray<Int_t> PMT_Quartet(treeReader, "PMT_Quartet");

    ifstream crystalsPositionFile("Crystals_Positions.txt");
    std:string line;
    std::vector<double> crystalX, crystalY, crystalZ;
    if(crystalsPositionFile.is_open())
        while(getline(crystalsPositionFile, line)){
            if (line == "# NumeroPMT X Y Z unit")
                continue;

            int i = 0;
            std::string arr[4];
            stringstream str_stream(line);
            while(str_stream.good() && i < 4){
                str_stream >> arr[i];
                i++;
            }

            crystalX.push_back(stod(arr[1]));
            crystalY.push_back(stod(arr[2]));
            crystalZ.push_back(stod(arr[3]));
        }

    std::cout << "End reading data" << std::endl;

    // Histograms
    TH1F* Edep_hist = new TH1F("", "Energy deposit in all crystals (Log scale); Energy (keV); Entries",
        1100, 0., 550.);
    TH1F* hitsNumber_hist = new TH1F("", "Type I and II; Number of hits; Events(%)",
        10, -0.5, 9.5);

    TH1F* xTypeI_hist = new TH1F("", "Type I; X axis(mm); Events(%)", 56, -7, 7);
    TH1F* yTypeI_hist = new TH1F("", "Type I; Y axis(mm); Events(%)", 56, -7, 7);
    TH1F* zTypeI_hist = new TH1F("", "Type I; Z axis(mm); Events(%)", 64, -8, 8);

    TH1F* xTypeII_hist = new TH1F("", "Type II; X axis(mm); Events(%)", 56, -7, 7);
    TH1F* yTypeII_hist = new TH1F("", "Type II; Y axis(mm); Events(%)", 56, -7, 7);
    TH1F* zTypeII_hist = new TH1F("", "Type II; Z axis(mm); Events(%)", 64, -8, 8);

    TH1F* xTypeIIa_hist = new TH1F("", "Type IIa; X axis(mm); Events(%)", 56, -7, 7);
    TH1F* yTypeIIa_hist = new TH1F("", "Type IIa; Y axis(mm); Events(%)", 56, -7, 7);
    TH1F* zTypeIIa_hist = new TH1F("", "Type IIa; Z axis(mm); Events(%)", 64, -8, 8);


    TH1F* crystals_hist = new TH1F("", "Type IV and V; Number of crystals; Events(%)",
        4, 0.5, 4.5);
    TH1F* EdepInTwoCrystals_hist = new TH1F("", "Type IV and V; Energy(keV); Events(%)", 6, 0, 600);

    TH1F* FullEdepCrystalsIIba_hist = new TH1F("", "Type IIba; Energy in first crystal(keV);Events(%)",
        550, 0, 550);
    TH1F* FullEdepCrystalsIIbaWBrem_hist = new TH1F("", "Type IIba with Brem; Energy in first crystal(keV);Events(%)",
        550, 0, 550);
    TH1F* FullEdepCrystalsIIbaBel30 = new TH1F("", "Type IIba; Energy in first crystal(keV);Events(%)",
        550, 0, 550);
    TH1F* FullEdepCrystalsIIbaAbove30 = new TH1F("", "Type IIba; Energy in first crystal(keV);Events(%)",
        550, 0, 550);
    TH1F* FullEdepCrystalsIIbaWoBrem_hist = new TH1F("", "Type IIba without Brem; Energy in first crystal(keV);Events(%)",
        550, 0, 550);
    TH1F* FullEdepCrystalsIIbb_hist = new TH1F("", "Type IIbb; Energy in first crystal(keV);Events(%)",
        550, 0, 550);
    TH2F* PartEdepCrystals_hist = new TH2F("", "Type V; Edep in first crystal(keV);Edep in second crystal(keV)",
        550, 0, 550, 550, 0, 550);

    TH1F* distanceQuartets2Cryst_histo = new TH1F("", "Type IIba; Diff in Quartets; Events(%)",
        60, -0.5 , 59.5);
    TH1F* distanceQuartets_histo = new TH1F("", "Type IIbb; Diff in Quartets; Events(%)",
        60, -0.5 , 59.5);
    TH1F* distance_histo = new TH1F("", "Type IIb; Length(mm); Events(%)",
        300, -0.5 , 299.5);

    TH1F* X_hit = new TH1F("", "Type IIa; X-axis(mm); Events(%)", 56, -7 , 7);
    TH1F* Y_hit = new TH1F("", "Type IIa; Y-asix(mm); Events(%)", 56, -7 , 7);
    TH1F* Z_hit = new TH1F("", "Type IIa; Z-axis(mm); Events(%)", 64, -8 , 8);

    TH1F* X_hit_weighted = new TH1F("", "Type IIa; X-axis(mm); Events(%)", 56, -7 , 7);
    TH1F* Y_hit_weighted = new TH1F("", "Type IIa; Y-asix(mm); Events(%)", 56, -7 , 7);
    TH1F* Z_hit_weighted = new TH1F("", "Type IIa; Z-axis(mm); Events(%)", 64, -8 , 8);


    TLegend* legend_distance = new TLegend(.57, .55, .9, .9);
    legend_distance->AddEntry(FullEdepCrystalsIIba_hist, "Total", "f");
    legend_distance->AddEntry(FullEdepCrystalsIIbaBel30, "Distance below 30 quartets", "f");
    legend_distance->AddEntry(FullEdepCrystalsIIbaAbove30, "Distance above 30 quartets", "f");

    TLegend* legend_edep_per_hitX = new TLegend(0.8, 0.8, 1, 1);
    legend_edep_per_hitX->AddEntry(X_hit, "Non-weighted", "f");
    legend_edep_per_hitX->AddEntry(X_hit_weighted, "Weighted", "f");

    TLegend* legend_edep_per_hitY = new TLegend(0.8, 0.8, 1, 1);
    legend_edep_per_hitY->AddEntry(Y_hit, "Non-weighted", "f");
    legend_edep_per_hitY->AddEntry(Y_hit_weighted, "Weighted", "f");

    TLegend* legend_edep_per_hitZ = new TLegend(0.8, 0.8, 1, 1);
    legend_edep_per_hitZ->AddEntry(Z_hit, "Non-weighted", "f");
    legend_edep_per_hitZ->AddEntry(Z_hit_weighted, "Weighted", "f");

    // Variables
    int typeI_events = 0, typeII_events = 0, typeIIa_events = 0, typeIIb_events = 0, typeIIba_events = 0,
        typeIIbb_events = 0, typeIII_events = 0, noInteractionInOneCrystal = 0,
        noInteractioninDiffCrystals = 0, lastPhotonInteractionIndex = -1, passThroughCrystal = 0,
        FullwoBrem = 0, FullwBrem = 0;

    std::cout << "Filling histograms..." << std::endl;

    while(treeReader.Next()){
        double event_Edep = 0;
        int electronsProduced = 0;

        // for(int i = 0; i < Edep.GetSize(); i++)
        //     event_Edep += Edep[i] * 1000; // {E*1000} because energy stored in MeV

        if(CrystalNumber.GetSize() != 0)
            passThroughCrystal++;

        // Number of produced e by photon and
        // Check interact photon in one crystal on in more than one
        for(int i = 0; i < DeltaEnergy.GetSize(); i++)
            if(PDGCode[i] == 22 && CreationProcessName[i] == -1)
                if(ProcessName[i] == 7 || ProcessName[i] == 19){
                    electronsProduced += 1;

                    lastPhotonInteractionIndex++;

                    if(ProcessName[i] == 19)
                        lastPhotonInteractionIndex = i;
                }

        int noCrystalsInteraction = 10, currentCrystal = 0;
        std::vector<double> EdepByCrystal(noCrystalsInteraction, 0.0);
        std::vector<int> Quartets;
        std::vector<int> photonCrystals;

        std::map <int, int> CrystalsMap;
        // MAP: Crystal connected with EdepByCrystal
        // Crystal number <=> index of EdepByCrystal
        for(int i = 0; i < CrystalNumber.GetSize(); i++){
            if(CrystalsMap.find(CrystalNumber[i]) == CrystalsMap.end())
                CrystalsMap.insert(pair <int, int> (CrystalNumber[i], CrystalsMap.size()));

            if (PDGCode[i] == 22 && CreationProcessName[i] == -1 &&
            std::find(photonCrystals.begin(), photonCrystals.end(),CrystalNumber[i]) == photonCrystals.end())
                photonCrystals.push_back(CrystalNumber[i]);

            Quartets.push_back(PMT_Quartet[i]);
        }

        // Edep in each crystal
        for(int i = 0; i < Edep.GetSize(); i++)
            if(CrystalNumber[i])
                EdepByCrystal[CrystalsMap[CrystalNumber[i]]] += Edep[i] * 1000;

        std::vector<double> Edep_copy;
        for(int i = 0; i < EdepByCrystal.size(); i++)
            Edep_copy.push_back(EdepByCrystal[i]);

        //Remove crystals with Edep = 0
        std::vector<double>::iterator it;
        double event_Edep_M2 = 0.;
        event_Edep = 0;
        for (it = EdepByCrystal.begin(); it != EdepByCrystal.end(); )
            if (*it < 1)
                it = EdepByCrystal.erase(it);
            else{
                event_Edep += *it;
                ++it;
            }

        noCrystalsInteraction = EdepByCrystal.size();

        // Distance
        double diffQuartets = 0.;
        if(Quartets.size() != 0)
            diffQuartets = *(std::max_element(Quartets.begin(), Quartets.end()))
            - *(std::min_element(Quartets.begin(), Quartets.end()));

        int maxCrystalNumber = 0, minCrystalNumber = 0;
        if(CrystalNumber.GetSize() != 0){
            maxCrystalNumber = *(std::max_element(CrystalNumber.begin(), CrystalNumber.end()));
            minCrystalNumber = *(std::min_element(CrystalNumber.begin(), CrystalNumber.end()));
        }

        if(event_Edep > 510.9 && noCrystalsInteraction > 1 && electronsProduced > 1){
            double vector[3] = {crystalX[maxCrystalNumber] - crystalX[minCrystalNumber],
                                crystalY[maxCrystalNumber] - crystalY[minCrystalNumber],
                                crystalZ[maxCrystalNumber] - crystalZ[minCrystalNumber]};

            double distance = 0.;
            for(int i = 1; i < 3; i++)
                distance += vector[i] * vector[i];

            distance = sqrt(distance);

            distance_histo->Fill(distance);
        }

        if(event_Edep > 1)
            Edep_hist->Fill(event_Edep);

        bool Brem = false;
        for(int i = 0; i < ProcessName.GetSize(); i++){
            if(ProcessName[i] == 10 || CreationProcessName[i] == 10){
                Brem = true;
                break;
            }
        }

        if(event_Edep > 510.9 && noCrystalsInteraction == 1 && electronsProduced > 1)
            for(int i = 0; i < DeltaEnergy.GetSize(); i++)
                if(PDGCode[i] == 22 && CreationProcessName[i] == -1){
                    X_hit->Fill(Local_PosX[i]);
                    X_hit_weighted->Fill(Local_PosX[i], DeltaEnergy[i] * 1000 / 511);

                    Y_hit->Fill(Local_PosY[i]);
                    Y_hit_weighted->Fill(Local_PosY[i], DeltaEnergy[i] * 1000 / 511);

                    Z_hit->Fill(Local_PosZ[i]);
                    Z_hit_weighted->Fill(Local_PosZ[i], DeltaEnergy[i] * 1000 / 511);
                }


        if(event_Edep > 0 && noCrystalsInteraction > 1)
            PartEdepCrystals_hist->Fill(EdepByCrystal[0], EdepByCrystal[1]);

        if(noCrystalsInteraction > 1)
            EdepInTwoCrystals_hist->Fill(EdepByCrystal[0] + EdepByCrystal[1]);

        if(noCrystalsInteraction == 1)
            noInteractionInOneCrystal++;
        else if(noCrystalsInteraction >1)
            noInteractioninDiffCrystals++;

        if (noCrystalsInteraction == 2)
            distanceQuartets2Cryst_histo->Fill(diffQuartets);
        else if (noCrystalsInteraction > 2)
            distanceQuartets_histo->Fill(diffQuartets);

        if(noCrystalsInteraction != 0)
            crystals_hist->Fill(noCrystalsInteraction);

        if(event_Edep > 510.9 && electronsProduced == 1){
            typeI_events++;
            hitsNumber_hist->Fill(electronsProduced);
            xTypeI_hist->Fill(Local_PosX[lastPhotonInteractionIndex]);
            yTypeI_hist->Fill(Local_PosY[lastPhotonInteractionIndex]);
            zTypeI_hist->Fill(Local_PosZ[lastPhotonInteractionIndex]);
        }else if(event_Edep > 510.9){
            typeII_events++;
            hitsNumber_hist->Fill(electronsProduced);
            xTypeII_hist->Fill(Local_PosX[lastPhotonInteractionIndex]);
            yTypeII_hist->Fill(Local_PosY[lastPhotonInteractionIndex]);
            zTypeII_hist->Fill(Local_PosZ[lastPhotonInteractionIndex]);

            if(noCrystalsInteraction == 1){
                typeIIa_events++;
                xTypeIIa_hist->Fill(Local_PosX[lastPhotonInteractionIndex]);
                yTypeIIa_hist->Fill(Local_PosY[lastPhotonInteractionIndex]);
                zTypeIIa_hist->Fill(Local_PosZ[lastPhotonInteractionIndex]);
            }else{
                typeIIb_events++;
                if(noCrystalsInteraction == 2){
                    typeIIba_events++;
                    FullEdepCrystalsIIba_hist->Fill(EdepByCrystal[0]);

                    if(diffQuartets < 30)
                        FullEdepCrystalsIIbaBel30->Fill(EdepByCrystal[0]);
                    else
                        FullEdepCrystalsIIbaAbove30->Fill(EdepByCrystal[0]);

                    if(Brem){
                        FullwBrem++;
                        FullEdepCrystalsIIbaWBrem_hist->Fill(EdepByCrystal[0]);
                    }else{
                        FullwoBrem++;
                        FullEdepCrystalsIIbaWoBrem_hist->Fill(EdepByCrystal[0]);
                    }
                }else
                    typeIIbb_events++;
                    FullEdepCrystalsIIbb_hist->Fill(EdepByCrystal[0]);
                }
        }else if(event_Edep < 510.9 && electronsProduced != 0){
            typeIII_events++;
        }

        //std::cout << "---------------------------------------" << std::endl;
    }
    std::cout << "End filling histograms" << std::endl;

    // Draw and save histograms
    std::cout << "Saving histograms..." << std::endl;

    // Edep hist
    canvas->SetLogy();
    //DrawAndSaveHistorgam(Edep_hist, true, "Edep.png", 1., "L");
    Edep_hist->Draw("L");
    canvas->SaveAs("images/Edep.png");
    canvas->SetLogy(0);

    // Hits hist
    DrawAndSaveHistorgam(hitsNumber_hist);
    DrawAndSaveWithCopy(hitsNumber_hist, pad, 5, 8, "Hits.png");

    // Position of interaction for type I photons
    DrawAndSaveHistorgam(xTypeI_hist, true, "Xposition_typeI.png");
    DrawAndSaveHistorgam(yTypeI_hist, true, "Yposition_typeI.png");
    DrawAndSaveHistorgam(zTypeI_hist, true, "Zposition_typeI.png");

    // Position of interaction for type II phorons
    DrawAndSaveHistorgam(xTypeII_hist, true, "Xposition_typeII.png");
    DrawAndSaveHistorgam(yTypeII_hist, true, "Yposition_typeII.png");
    DrawAndSaveHistorgam(zTypeII_hist, true, "Zposition_typeII.png");

    // Position of interaction for type IIa phorons
    DrawAndSaveHistorgam(xTypeIIa_hist, true, "Xposition_typeIIa.png");
    DrawAndSaveHistorgam(yTypeIIa_hist, true, "Yposition_typeIIa.png");
    DrawAndSaveHistorgam(zTypeIIa_hist, true, "Zposition_typeIIa.png");

    //Crystal hist
    DrawAndSaveHistorgam(crystals_hist, false, "", passThroughCrystal);
    DrawAndSaveWithCopy(crystals_hist, pad2, 2, 4, "crystals_interaction.png");

    //Edep for different types of events
    DrawAndSaveHistorgam(EdepInTwoCrystals_hist, true, "EdepInTwoCrystals.png");
    DrawAndSaveHistorgam(FullEdepCrystalsIIbb_hist, true, "FullEdepIn2CrystalsIIbb.png");
    DrawAndSave2DHist(PartEdepCrystals_hist, "PartEdepIn2Crystals.png");
    DrawAndSaveHistorgam(FullEdepCrystalsIIbaWBrem_hist, true, "EdepIIbaWBrem.png");
    DrawAndSaveHistorgam(FullEdepCrystalsIIbaWoBrem_hist, true, "EdepIIbaWoBrem.png");
    DrawAndSaveHistorgam(FullEdepCrystalsIIba_hist, false, "", typeIIba_events);
    DrawAndSaveHistorgam(FullEdepCrystalsIIbaBel30, false, "", typeIIba_events, "SAME BAR", 2);
    legend_distance->Draw();
    DrawAndSaveHistorgam(FullEdepCrystalsIIbaAbove30, true, "FullEdepIn2CrystalsIIba.png", typeIIba_events, "SAME BAR", 3);

    //Distance
    DrawAndSaveHistorgam(distanceQuartets2Cryst_histo);
    DrawAndSaveWithCopy(distanceQuartets2Cryst_histo, pad3, 0, 8, "DistanceQuartets2Cryst.png");
    canvas->SetLogy();
    pad3->Close();
    canvas->SaveAs("images/DistanceQuartets2CrystLOG.png");
    canvas->SetLogy(0);
    DrawAndSaveHistorgam(distanceQuartets_histo);
    DrawAndSaveWithCopy(distanceQuartets_histo, pad4, 12, 59, "DistanceQuartets.png");
    DrawAndSaveHistorgam(distance_histo);
    DrawAndSaveWithCopy(distance_histo, pad5, 50, 100, "Distance.png");

    //Edep per hit
    DrawAndSaveHistorgam(X_hit);
    legend_edep_per_hitX->Draw();
    DrawAndSaveHistorgam(X_hit_weighted, true, "EdepPerHitX.png", 1., "SAME BAR", 3);

    DrawAndSaveHistorgam(Y_hit);
    legend_edep_per_hitY->Draw();
    DrawAndSaveHistorgam(Y_hit_weighted, true, "EdepPerHitY.png", 1., "SAME BAR", 3);

    DrawAndSaveHistorgam(Z_hit);
    legend_edep_per_hitZ->Draw();
    DrawAndSaveHistorgam(Z_hit_weighted, true, "EdepPerHitZ.png", 1., "SAME BAR", 3);

    canvas->Close();

    //Distance < 10, % of event
    double closeDistance = 0.;
    for(int i = 0; i <= 10; i++){
        closeDistance += distanceQuartets2Cryst_histo->GetBinContent(i);
    }

    //Print general data
    double scaleFactor = 100. / treeReader.GetEntries(true);
    double scaleFactor_cryst = 100. / passThroughCrystal;
    double scaleFactor_fullAbs = 100. / (typeI_events + typeII_events);

    std::cout << "==================================================================================" << std::endl;
    std::cout << "===                                  General Data                              ===" << std::endl;
    std::cout << "==================================================================================" << std::endl;
    std::cout << "Type I events: " << typeI_events << " (" <<
        typeI_events * scaleFactor << " %), (" <<
        typeI_events * scaleFactor_cryst << " %), (" <<
        typeI_events * scaleFactor_fullAbs << " %)" << std::endl;
    std::cout << "Type II events: " << typeII_events << " (" <<
        typeII_events * scaleFactor << " %), (" <<
        typeII_events * scaleFactor_cryst << " %), (" <<
        typeII_events * scaleFactor_fullAbs << " %)" << std::endl;
    std::cout << "Type IIa events: " << typeIIa_events << " (" <<
        typeIIa_events * scaleFactor << " %), (" <<
        typeIIa_events * scaleFactor_cryst << " %), (" <<
        typeIIa_events * scaleFactor_fullAbs << " %)" << std::endl;
    std::cout << "Type IIb events: " << typeIIb_events << " (" <<
        typeIIb_events * scaleFactor << " %), (" <<
        typeIIb_events * scaleFactor_cryst << " %), (" <<
        typeIIb_events * scaleFactor_fullAbs << " %)" << std::endl;
    std::cout << "Type IIba events: " << typeIIba_events << " (" <<
        typeIIba_events * scaleFactor << " %), (" <<
        typeIIba_events * scaleFactor_cryst << " %), (" <<
        typeIIba_events * scaleFactor_fullAbs << " %)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Type IIba events with Bremsstrahlung: " << FullwBrem << " (" <<
        FullwBrem * scaleFactor << " %), (" <<
        FullwBrem * scaleFactor_cryst << " %), (" <<
        FullwBrem * scaleFactor_fullAbs << " %)" << std::endl;
    std::cout << "Type IIba events without Bremsstrahlung: " << FullwoBrem << " (" <<
        FullwoBrem * scaleFactor << " %), (" <<
        FullwoBrem * scaleFactor_cryst << " %), (" <<
        FullwoBrem * scaleFactor_fullAbs << " %)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::cout << "Type IIbb events: " << typeIIbb_events << " (" <<
        typeIIbb_events * scaleFactor << " %), (" <<
        typeIIbb_events * scaleFactor_cryst << " %), (" <<
        typeIIbb_events * scaleFactor_fullAbs << " %)" << std::endl;
    std::cout << "Type III events: " << typeIII_events << " (" <<
        typeIII_events * scaleFactor << " %), (" <<
        typeIII_events * scaleFactor_cryst << " %)" << std::endl;
    std::cout << "Total absorbtion events: " << typeI_events + typeII_events +
        typeIII_events << " (" << (typeI_events + typeII_events + typeIII_events) * scaleFactor <<
        " %)" << std::endl;
    std::cout << "Number of events: " << treeReader.GetEntries(true) << std::endl;
    std::cout << "Number of interactions in one crystal: " <<
        noInteractionInOneCrystal << " (" <<
        noInteractionInOneCrystal * scaleFactor << " %), (" <<
        noInteractionInOneCrystal * scaleFactor_cryst << " %)" << std::endl;
    std::cout << "Number of interactions in more than one crystal: " <<
        noInteractioninDiffCrystals << " (" <<
        noInteractioninDiffCrystals * scaleFactor << " %), (" <<
        noInteractioninDiffCrystals * scaleFactor_cryst << " %)" << std::endl;
    std::cout << "Number of photons passed through crystal: " <<
        passThroughCrystal << " (" <<
        passThroughCrystal * scaleFactor  << " %), (" <<
        passThroughCrystal * scaleFactor_cryst << " %)" << std::endl;
    std::cout << "Distance < 10: " << closeDistance << " %" << std::endl;
    std::cout << "==================================================================================" << std::endl;
    std::cout << "==================================================================================" << std::endl;
}
