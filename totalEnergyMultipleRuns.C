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

void totalEnergyMultipleRuns(){

  std::string runNumberList[3] = {"28", "29", "30"};
  int numberOfRuns = 3;

  TH1D* totalEventEnergy[3];
  TH1D* totalEnergySum = new TH1D("totalEventEnergySum", "Total Event Energy Sum", 500, 0, 10000);

  std::string OutFileName = "outputHistos/25-08-02_Tagger12_AllCh_MultipleRuns_totalEnergyHistos.root";
  TString OutFileNameT = OutFileName;
  
  for (int runNumber = 0; runNumber < numberOfRuns; runNumber++){

    std::string InFileName = "outputHistos/25-08-02_Tagger12_AllCh_Run" + runNumberList[runNumber] + "_CoicidenceHistos.root";
    TString InFileNameT = InFileName;

    TFile *InFile = new TFile(InFileNameT);

    //InFile->ls(); 

    totalEventEnergy[runNumber] = (TH1D*) InFile->Get("EnergyHistos/totalEventEnergy");

    totalEventEnergy[runNumber]->SetXTitle("ADC Energy");

    totalEnergySum->Add(totalEventEnergy[runNumber]);

  }

  totalEnergySum->SetXTitle("ADC Energy");

  TFile OutFile(OutFileNameT,"recreate");

  TCanvas *c1 = new TCanvas("c1", "Total Event Energy", 800, 600);

  TLegend* legend = new TLegend(0.7, 0.7, 0.9, 0.9);
  legend->SetHeader("Run Number", "C");


  for (int runNumber = 0; runNumber < numberOfRuns; runNumber++) {
    legend->AddEntry(totalEventEnergy[runNumber], ("Run " + runNumberList[runNumber]).c_str(), "l");
  }

  for (int runNumber = 0; runNumber < numberOfRuns; runNumber++){
    
    std::string histoName = "totalEventEnergy_run" + runNumberList[runNumber];
    TString histoNameT = histoName;

    totalEventEnergy[runNumber]->SetName(histoNameT);
    totalEventEnergy[runNumber]->SetTitle(("Total Event Energy for Run " + runNumberList[runNumber]).c_str());
    totalEventEnergy[runNumber]->SetLineColor(runNumber + 1); // Different color for each run
    totalEventEnergy[runNumber]->SetLineWidth(2);
    totalEventEnergy[runNumber]->SetStats(kFALSE); // Disable stats box
    
    if (runNumber == 0)
      totalEventEnergy[runNumber]->Draw();
    else
      totalEventEnergy[runNumber]->Draw("SAME");

  }

  c1->Update();
  legend->Draw();

  totalEnergySum->Write();
  c1->Write();
  OutFile.Close(); 

} 
