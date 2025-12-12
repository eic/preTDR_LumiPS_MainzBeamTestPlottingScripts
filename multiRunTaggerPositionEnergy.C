#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <TFile.h>
#include <TH1.h>
#include <TH3.h>
#include <TBranch.h>
#include <TH2.h>
#include <TTree.h>
#include <TChain.h>
#include <TCut.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TRandom.h>
#include <TEventList.h>
#include <TMultiLayerPerceptron.h>
#include <TComplex.h>
#include <TVirtualGeoPainter.h>
#include <TFile.h>
#include <TSystem.h>
#include <TClassTree.h>
#include <TPaveLabel.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TGClient.h>
#include <RQ_OBJECT.h>
#include <TApplication.h>
#include <TRint.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TF1.h>
#include <TF2.h>
#include <TGenPhaseSpace.h>
#include <TLorentzVector.h>
#include <Riostream.h>
#include <TObjString.h>
#include <TColor.h>

#include "ePICStyle.C"

void multiRunTaggerPositionEnergy()
{
  //gROOT->SetBatch(kTRUE);
  gROOT->ProcessLine("SetePICStyle()");

  // New Colour Scheme palette
  std::vector<int> sixColourScheme = {
    TColor::GetColor("#7021dd"),     // violet
    TColor::GetColor("#964a8b"),     // grape
    TColor::GetColor("#e42536"),     // red
    TColor::GetColor("#f89c20"),     // yellow
    TColor::GetColor("#5790fc"),     // blue
    TColor::GetColor("#9c9ca1"),     // grey
  };

  std::string fileNameList[4] = {"25-08-01_Tagger26_AllCh_MultipleRuns_totalEnergyHistos.root",
                                  "25-08-02_Tagger18_AllCh_MultipleRuns_totalEnergyHistos.root",
                                  "25-08-02_Tagger12_AllCh_MultipleRuns_totalEnergyHistos.root",
                                  "25-08-07_Tagger10_AllCh_MultipleRuns_totalEnergyHistos.root"
                                };
  std::string runNumberList[4] = {"Tagger26","Tagger18", "Tagger12", "Tagger10"};
  std::string runEnergiesList[4] = {"288.5-300 MeV", "410-425 MeV", "520-540 MeV", "545-563.5 MeV"};
  int numberOfFiles = 4;

  TH1D* totalEventEnergy[4];

  std::string OutFileName = "outputHistos/TaggerPositionComparrison_totalEnergyHistos.root";
  TString OutFileNameT = OutFileName;
  
  for (int fileNumber= 0; fileNumber< numberOfFiles; fileNumber++){

    std::string InFileName = "outputHistos/" + fileNameList[fileNumber];
    TString InFileNameT = InFileName;

    TFile *InFile = new TFile(InFileNameT);

    //InFile->ls(); 

    totalEventEnergy[fileNumber] = (TH1D*) InFile->Get("totalEventEnergySum");

    totalEventEnergy[fileNumber]->SetXTitle("ADC Energy");
    totalEventEnergy[fileNumber]->SetYTitle("Normalised Counts");
  }

  TFile OutFile(OutFileNameT,"recreate");

  TCanvas *c1 = new TCanvas("c1", "Total Event Energy", 800, 600);

  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->SetHeader("Run Number", "C");


  for (int fileNumber= 0; fileNumber< numberOfFiles; fileNumber++) 
  {
    std::string histoTitle = runNumberList[fileNumber] + ": " + runEnergiesList[fileNumber];
    TString histoTitleT = histoTitle;
    legend->AddEntry(totalEventEnergy[fileNumber], histoTitleT, "l");
  }

  for (int fileNumber= 0; fileNumber< numberOfFiles; fileNumber++){
    
    std::string histoName = "totalEventEnergy_run" + runNumberList[fileNumber];
    TString histoNameT = histoName;

    int binMax = totalEventEnergy[fileNumber]->GetMaximumBin();
    double value = totalEventEnergy[fileNumber]->GetBinContent(binMax);
    std::cout << "Maximum value bin for " << histoNameT << " was: " << binMax << std::endl;
    totalEventEnergy[fileNumber]->Scale(1.0/value); // Normalize the histogram

    totalEventEnergy[fileNumber]->SetName(histoNameT);
    totalEventEnergy[fileNumber]->SetLineWidth(2);
    totalEventEnergy[fileNumber]->SetStats(kFALSE); // Disable stats box
    
    if (fileNumber == 0)
    {
      totalEventEnergy[fileNumber]->SetLineColor(sixColourScheme[0]); 
      totalEventEnergy[fileNumber]->SetMarkerColor(sixColourScheme[0]); 
      totalEventEnergy[fileNumber]->SetMarkerStyle(20);
    }
    else if (fileNumber == 1)
    {
      totalEventEnergy[fileNumber]->SetLineColor(sixColourScheme[1]); 
      totalEventEnergy[fileNumber]->SetMarkerColor(sixColourScheme[1]);  
      totalEventEnergy[fileNumber]->SetMarkerStyle(22);
    }
    else if (fileNumber == 2) 
    {
      totalEventEnergy[fileNumber]->SetLineColor(sixColourScheme[2]);
      totalEventEnergy[fileNumber]->SetMarkerColor(sixColourScheme[2]);  
      totalEventEnergy[fileNumber]->SetMarkerStyle(23);
    }
    else if (fileNumber == 3) 
    {
      totalEventEnergy[fileNumber]->SetLineColor(sixColourScheme[3]); 
      totalEventEnergy[fileNumber]->SetMarkerColor(sixColourScheme[3]);  
      totalEventEnergy[fileNumber]->SetMarkerStyle(20);
    }
    else 
    {
    }
    //totalEventEnergy[fileNumber]->SetDrawOption("P"); // Set draw option to "PE" for points with error bars

    if (fileNumber== 0)
      totalEventEnergy[fileNumber]->Draw("PE");
    else
      totalEventEnergy[fileNumber]->Draw("PE SAME");

  }

  c1->Update();
  legend->Draw();

  c1->Draw();

  c1->Write();
  OutFile.Close(); 

} 
