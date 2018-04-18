// TTree Reader
#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TSelector.h"

// Basic include
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TF1.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TLegend.h"

//c++ include
#include <vector>
#include <algorithm>
#include <cmath>

// Global canvas
TCanvas* canvas;

void DrawAndSaveHistogram(TH1F* histo, std::string imgName, int color = 4){
    histo->SetFillColor(color);
    histo->SetFillStyle(3001);
    histo->Scale(100 / histo->Integral());
    histo->Draw("BAR");

    std::string path = "images/" + imgName;
    canvas->SaveAs(path.c_str());
}

void DrawAndSaveWithCopy(TH1F* histo, TPad* pad, double min, double max, std::string imgName){
    DrawAndSaveHistogram(histo, imgName);

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

void processData(){
    // Global style options and canvas with pads
    canvas = new TCanvas("canvas", "canvas", 1750, 1000);
    TPad* pad = new TPad("pad", "", 0.5, 0.2, 0.9, 0.7);
    TPad* pad2 = new TPad("pad2", "", 0.5, 0.2, 0.9, 0.7);

    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);
    gStyle->SetOptFit();

    canvas->SetGrid();
    pad->SetGrid();
    pad2->SetGrid();

    // TTreeReader
    TFile* dataFile = TFile::Open("data.root");
    TTree* opticalTree = (TTree*) dataFile->Get("Optical photons");

    TTreeReader treeReader(opticalTree);

    TTreeReaderArray<Double_t> LocalTime(treeReader, "LocalTime");
    TTreeReaderArray<Double_t> GlobalTime(treeReader, "GlobalTime");
    TTreeReaderArray<Double_t> Edep(treeReader, "Edep");
    TTreeReaderArray<Int_t> NumOfElectrons(treeReader, "NumOfElectrons");

    // Histograms
    TH1F* localTimeI_hist = new TH1F("Events", "Type I; Time, ns; Events (%)",
     1000, -0.5, 3);
    TH1F* localTimeII_hist = new TH1F("Events", "Type II; Time, ns; Events (%)",
     1000, -0.5, 3);

    TH1F* globalTimeI_hist = new TH1F("Events", "Type I; Time, ns; Events (%)",
     8100, -3.75, 300);
    TH1F* globalTimeII_hist = new TH1F("Events", "Type II; Time, ns; Events (%)",
     8100, -3.75, 300);

    while(treeReader.Next()){
        double edep = 0.;
        int noElectrons = NumOfElectrons[0];
        for(int i = 0; i < Edep.GetSize(); i++)
            edep += Edep[i];

        if(edep > 510.9){
            if(noElectrons == 1){
                for(int i = 0; i < LocalTime.GetSize(); i++){
                    globalTimeI_hist->Fill(GlobalTime[i]);
                    localTimeI_hist->Fill(LocalTime[i]);
                }
            } else if (noElectrons > 1){
                for(int i = 0; i < LocalTime.GetSize(); i++){
                    globalTimeII_hist->Fill(GlobalTime[i]);
                    localTimeII_hist->Fill(LocalTime[i]);
                }
            }
        }

    }

    for(int i = 0; i < globalTimeI_hist->GetNbinsX(); i++){
        if(globalTimeI_hist->GetBinContent(i) != 0){
            std::cout << "globalI first non-empty bin#: " << i << std::endl;
            break;
        }
    }

    for(int i = 0; i < globalTimeII_hist->GetNbinsX(); i++){
        if(globalTimeII_hist->GetBinContent(i) != 0){
            std::cout << "globalII first non-empty bin#: " << i << std::endl;
            break;
        }
    }

    TF1 *my_fit = new TF1("fit", "expo", 0, 300);

    DrawAndSaveHistogram(localTimeI_hist, "localTimeI.png");
    DrawAndSaveHistogram(localTimeII_hist, "localTimeII.png");

    DrawAndSaveHistogram(globalTimeI_hist, "globalTimeI.png");
    globalTimeI_hist->Fit("fit", "R");
    DrawAndSaveWithCopy(globalTimeI_hist, pad, -0.5, 5, "globalTimeI.png");

    DrawAndSaveHistogram(globalTimeII_hist, "globalTimeII.png");
    globalTimeII_hist->Fit("fit", "R");
    DrawAndSaveWithCopy(globalTimeII_hist, pad2, -0.5, 5, "globalTimeII.png");

    canvas->SetLogy();
    DrawAndSaveHistogram(localTimeI_hist, "localTimeI_log.png");
    DrawAndSaveHistogram(localTimeII_hist, "localTimeII_log.png");
    DrawAndSaveHistogram(globalTimeI_hist, "globalTimeI_log.png");
    DrawAndSaveHistogram(globalTimeII_hist, "globalTimeII_log.png");

    canvas->Close();
}
