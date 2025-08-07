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

void taggerPositionEnergy(){

  std::string fileNameList[16] = {"25-07-31_Tagger18_AllCh_Run4_CoicidenceHistos.root", "25-07-31_Tagger18_AllCh_Run7_CoicidenceHistos.root", "25-07-31_Tagger18_AllCh_Run8_CoicidenceHistos.root", "25-07-31_Tagger18_AllCh_Run13_CoicidenceHistos.root",
                                  "25-08-01_Tagger26_AllCh_Run16_CoicidenceHistos.root", "25-08-01_Tagger26_AllCh_Run17_CoicidenceHistos.root", "25-08-01_Tagger26_AllCh_Run18_CoicidenceHistos.root", "25-08-01_Tagger26_AllCh_Run19_CoicidenceHistos.root", 
                                  "25-08-02_Tagger12_AllCh_Run29_CoicidenceHistos.root", "25-08-02_Tagger12_AllCh_Run30_CoicidenceHistos.root",
                                  "25-08-02_Tagger18_AllCh_Run31_CoicidenceHistos.root", "25-08-02_Tagger18_AllCh_Run32_CoicidenceHistos.root", "25-08-02_Tagger18_AllCh_Run33_CoicidenceHistos.root",
                                  "25-08-07_Tagger10_AllCh_Run50_CoicidenceHistos.root", "25-08-07_Tagger10_AllCh_Run51_CoicidenceHistos.root", "25-08-07_Tagger10_AllCh_Run52_CoicidenceHistos.root"
                                };
  std::string runNumberList[16] = {"Run4", "Run7", "Run8", "Run13", "Run16", "Run17", "Run18", "Run19", "Run29", "Run30", "Run31", "Run32", "Run33", "Run50", "Run51", "Run52"};
  int numberOfFiles = 16;

  TH1D* totalEventEnergy[16];

  std::string OutFileName = "outputHistos/TaggerPositionComparrison_totalEnergyHistos.root";
  TString OutFileNameT = OutFileName;
  
  for (int fileNumber= 0; fileNumber< numberOfFiles; fileNumber++){

    std::string InFileName = "outputHistos/" + fileNameList[fileNumber];
    TString InFileNameT = InFileName;

    TFile *InFile = new TFile(InFileNameT);

    //InFile->ls(); 

    totalEventEnergy[fileNumber] = (TH1D*) InFile->Get("EnergyHistos/totalEventEnergy");

    totalEventEnergy[fileNumber]->SetXTitle("ADC Energy");

  }

  TFile OutFile(OutFileNameT,"recreate");

  TCanvas *c1 = new TCanvas("c1", "Total Event Energy", 800, 600);

  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->SetHeader("Run Number", "C");


  for (int fileNumber= 0; fileNumber< numberOfFiles; fileNumber++) {
    legend->AddEntry(totalEventEnergy[fileNumber], ("Run " + runNumberList[fileNumber]).c_str(), "l");
  }

  for (int fileNumber= 0; fileNumber< numberOfFiles; fileNumber++){
    
    std::string histoName = "totalEventEnergy_run" + runNumberList[fileNumber];
    TString histoNameT = histoName;

    int binMax = totalEventEnergy[fileNumber]->GetMaximumBin();
    double value = totalEventEnergy[fileNumber]->GetBinContent(binMax);
    totalEventEnergy[fileNumber]->Scale(1.0/value); // Normalize the histogram

    totalEventEnergy[fileNumber]->SetName(histoNameT);
    totalEventEnergy[fileNumber]->SetLineWidth(2);
    totalEventEnergy[fileNumber]->SetStats(kFALSE); // Disable stats box
    
    if (fileNumber <= 3) totalEventEnergy[fileNumber]->SetLineColor(kBlue + fileNumber); 
    else if (fileNumber > 3 && fileNumber <=7) totalEventEnergy[fileNumber]->SetLineColor(kGreen + (fileNumber - 4)); 
    else if (fileNumber > 7 && fileNumber <=9) totalEventEnergy[fileNumber]->SetLineColor(kRed + (fileNumber - 8)); 
    else if (fileNumber > 9 && fileNumber <= 12) totalEventEnergy[fileNumber]->SetLineColor(kCyan + (fileNumber - 10));
    else totalEventEnergy[fileNumber]->SetLineColor(kOrange + (fileNumber - 10)); 

    totalEventEnergy[fileNumber]->SetDrawOption("P"); // Set draw option to "PE" for points with error bars

    if (fileNumber== 0)
      totalEventEnergy[fileNumber]->Draw("");
    else
      totalEventEnergy[fileNumber]->Draw("SAME");

  }

  c1->Update();
  legend->Draw();

  c1->Write();
  OutFile.Close(); 

} 
