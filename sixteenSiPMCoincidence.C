#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <numeric>
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
#include<TGClient.h>
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
#include <TChain.h>
#include "TTreeReader.h"

void sixteenSiPMCoincidence(){

  gROOT->Reset();
  gROOT->SetStyle("Plain");

  TString infileN;
  int numberOfFiles = 5; // Number of files to be processed

  TChain *chain = new TChain("Data_R");  

  for (int i = 0; i < numberOfFiles; i++){

    TString fileName = Form("runData/SDataR_25-08-07_Tagger10_AllCh_Run51p%d.root", i);
    if (gSystem->AccessPathName(fileName)) {
        std::cerr << "Warning: File " << fileName << " does not exist." << std::endl;
        continue; // Skip this file if it does not exist
    }
    chain->Add(fileName);
    std::cout << "Adding file: " << fileName << std::endl;

    if (i == 0) {
      infileN = fileName; 
    } 
    
  }

  if (chain->GetEntries() == 0) {
      std::cerr << "Error: No entries found in the TChain!" << std::endl;
      return;
  }
  
  chain->Print();

  infileN.ReplaceAll("p0.root","");
  infileN.ReplaceAll("runData/SDataR_","");

  TString ofile_name = Form("%s_CoicidenceHistos.root", infileN.Data());  

  std::string outfilename = Form("outputHistos/%s", ofile_name.Data());
  TString OutFileName = outfilename;

  std::cout << std::endl << "Output file is : " << OutFileName << std::endl << std::endl;

  TTreeReader myReader(chain);
  TTreeReaderValue<UShort_t> Energy(myReader, "Energy");
  TTreeReaderValue<ULong64_t> Timestamp(myReader, "Timestamp");  
  TTreeReaderValue<UShort_t> Channel(myReader, "Channel");

  double noEntries = chain->GetEntries();

  myReader.SetEntry(noEntries-1);

  ULong64_t lastTimestampL = *Timestamp * numberOfFiles;

  double lastTimestamp = static_cast<double>(lastTimestampL) * 1e-9;

  // define needed histograms


  TH1D* channelE[16]; 
  TH1D* channelEC[16];

  TH1D* channelECRatio[16];

  TH1D* hitsTimeDifference = new TH1D("hitsTimeDifference", "Time difference between hits", 1000, 0.0, 50000.0);
  TH1D* totalChannelsPerCoincidenceHisto = new TH1D("totalChannelsPerCoincidenceHisto", "Channels triggered per coicidence event", 16, 1, 17);
  TH1D* channelsPerCoincidenceHisto = new TH1D("channelsPerCoincidenceHisto", "Channels triggered, trigger channel 0", 16, 0, 16);
  TH1D* totalLayersPerCoincidenceHisto = new TH1D("totalLayersPerCoincidenceHisto", "Layers triggered per coicidence event", 6, 1, 7);
  TH1D* layersPerCoincidenceHisto = new TH1D("layersPerCoincidenceHisto", "Layers triggered, trigger channel 0", 6, 1, 7);

  TH1D* totalEventEnergy = new TH1D("totalEventEnergy", "Total event energy", 500, 0, 10000);
  TH1D* totalEventEnergyN = new TH1D("totalEventEnergyN", "Total event energy normalised by number of channels triggered", 500, 0, 5000);

  TH2D* energyTime = new TH2D("energyTime", "Energy against Time", 1000, 0.0, lastTimestamp, 500, 0, 5000);
  TH2D* ENvTime = new TH2D("ENvTime", "Total Event Energy Normalised by number of channels triggered against Time", 1000, 0.0, lastTimestamp, 500, 0, 5000);
  
  TH2D* TwoDLayoutTriggered = new TH2D("TwoDLayoutTriggered", "Channels triggered, trigger channel 0", 7, 0, 7, 7, 0, 7);  
  TH2D* TwoDLayoutE = new TH2D("TwoDLayoutE", "Energy Deposited in Each Channel", 7, 0, 7, 7, 0, 7);
  TH2D* TwoDLayoutEC = new TH2D("TwoDLayoutEC", "Coincidence Energy Deposited in Each Channel", 7, 0, 7, 7, 0, 7);

  int layoutX[16] = {1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6}; // Change these to match the layout
  int layoutY[16] = {2, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3, 1, 2, 3};

  for(int channel = 0; channel < 16; channel++){
    std::string histoEName = "channel" + std::to_string(channel) + "E";
    channelE[channel] = new TH1D(histoEName.c_str(), ("Energy Deposited in Channel " + std::to_string(channel)).c_str(), 5000, 0, 5000);
    channelE[channel]->SetDirectory(0);

    std::string histoECName = "channel" + std::to_string(channel) + "EC";
    channelEC[channel] = new TH1D(histoECName.c_str(), ("Energy Deposited in Channel " + std::to_string(channel) + " for Coincidences").c_str(), 5000, 0, 5000);
    channelEC[channel]->SetDirectory(0); 

    std::string histoECRatioName = "channel" + std::to_string(channel) + "ECRatio";
    channelECRatio[channel] = new TH1D(histoECRatioName.c_str(), ("Energy Deposited in Channel " + std::to_string(channel) + " for Coincidences Normalised by Energy Deposited in Channel").c_str(), 5000, 0, 5000);
    channelECRatio[channel]->SetDirectory(0);

  }

  myReader.SetEntry(0);

  ULong64_t timestampOffset = *Timestamp;
  ULong64_t adjustedTimestamp = *Timestamp;
  TString currentFileName = myReader.GetTree()->GetCurrentFile()->GetName();
  TString newFileName = "";
  int fileNumber = 0;

  int CoincidenceCount = 0;
  int channelsPerCoincidence = 0;
  int EhistosFilled = 0;
  int EChistosFilled = 0;
  std::vector<bool> entryTriggered(noEntries, false);

  UShort_t eventEnergy = 0.0;

  UShort_t triggerChannel = 0.0;
  UShort_t triggerEnergy = 0.0;
  ULong64_t triggerTimestamp = 0.0;
  bool trigged = false;

  UShort_t channelEnergy[16]; 
  ULong64_t channelTimestamp[16];
  long channelTimeDifference[16];
  long channelEntry[16];
  bool channelTriggered[16];

  int layersPerCoincidence = 0;
  bool layerTriggered[6];

  Int_t jmin=0;
  Int_t jmax=0;

  std::cout << "Processing file: " << currentFileName << std::endl;
  std::cout << "File start timestamp: " << static_cast<double>(*Timestamp) * 1e-9 << " milliseconds" << std::endl;

  for (Long64_t irow = 0; irow < noEntries; irow++) {
    myReader.SetEntry(irow);

      newFileName = myReader.GetTree()->GetCurrentFile()->GetName();

      if (newFileName != currentFileName) {
        timestampOffset = adjustedTimestamp;
        fileNumber += 1;
        currentFileName = newFileName;
        std::cout << "Processing file: " << currentFileName << std::endl;
        adjustedTimestamp = *Timestamp + timestampOffset;
        std::cout << "File start timestamp: " << static_cast<double>(adjustedTimestamp) * 1e-9 << " milliseconds" << std::endl;
      }

      adjustedTimestamp = *Timestamp + timestampOffset;
      
      if (irow % 10 == 0){
        fprintf (stderr, "%4.2f Percent\r ", irow*100.0/noEntries);
        fflush (stderr);
      }

      for(int channel = 0; channel < 16; channel++){
        channelEnergy[channel] = 0.0;
        channelTimestamp[channel] = 0.0;
        channelTriggered[channel] = false;
        channelTimeDifference[channel] = 0.0;
        channelEntry[channel] = 0;
      }

      trigged = false;
      channelsPerCoincidence = 0;
      layersPerCoincidence = 0;
      for(int layer = 0; layer < 6; layer++){
        layerTriggered[layer] = false;
      }

      triggerChannel = *Channel;
      triggerEnergy = *Energy;
      triggerTimestamp = adjustedTimestamp;

      eventEnergy = 0.0;

      if ((*Energy >= 150) && (*Energy <= 16300)){
        channelE[int(*Channel)]->Fill(*Energy);
        energyTime->Fill(static_cast<double>(adjustedTimestamp*1e-9), static_cast<double>(*Energy));
        TwoDLayoutE->Fill(layoutX[int(*Channel)], layoutY[int(*Channel)], static_cast<double>(*Energy));
        EhistosFilled += 1;
      }

      if ((*Energy >= 1000)  && (*Energy <= 16300) && (int(*Channel) == 0)  && (entryTriggered[irow] == false)){

        jmin=irow-10;
        jmax=irow+10;
        if(jmin<0) jmin=0;
        if(jmax>noEntries) jmax=noEntries;

        for(Long64_t j=jmin;j<jmax;j++){ // Loop over up to 20 entries either side of current event
              
          myReader.SetEntry(j);
          newFileName = myReader.GetTree()->GetCurrentFile()->GetName();
          if (newFileName != currentFileName) {
            break;
          }

          adjustedTimestamp = *Timestamp + timestampOffset;

          long timeDif = 0.0;
          if (triggerTimestamp >= adjustedTimestamp) timeDif = triggerTimestamp - adjustedTimestamp;
          else timeDif = adjustedTimestamp - triggerTimestamp;
          
          if ((*Channel != triggerChannel) && (timeDif <= 25000.0)  && (*Energy >= 150)  && (*Energy <= 16300)  && (entryTriggered[j] == false)){

            //std::cout << "Trigger Channel: " << triggerChannel  << " Trigger Energy: " << triggerEnergy  << " Trigger Timestamp: "<< (triggerTimestamp) << std::endl;
            //std::cout << "Channel: " << Channel  << " Energy: " << Energy  << " Timestamp: " << Timestamp << std::endl;

            if (trigged == false){
              channelEnergy[int(triggerChannel)] = triggerEnergy;
              channelTimestamp[int(triggerChannel)] = triggerTimestamp;
              channelEntry[int(triggerChannel)] = j;

              channelsPerCoincidence += 1;
              layersPerCoincidence += 1;

              layerTriggered[0] = true;

              trigged = true;
            }

            if (channelEnergy[int(*Channel)] != 0.0){
              std::cout << "Multiple hits in channel" << *Channel << " in Time Window, Time Difference: " << timeDif <<  std::endl;
              std::cout << "Trigger Timestamp: " << triggerTimestamp << " Current Timestamp: " << adjustedTimestamp << std::endl;
            }
            else{
              channelEnergy[int(*Channel)] = *Energy;
              channelTimestamp[int(*Channel)] = adjustedTimestamp;
              channelEntry[int(*Channel)] = j;
              
              channelTimeDifference[int(*Channel)] = timeDif;
            
              channelsPerCoincidence += 1;

              if (layerTriggered[layoutX[int(*Channel)]-1] == false){
                layerTriggered[layoutX[int(*Channel)]-1] = true;
                layersPerCoincidence += 1;
              }
                      
            }  
          }
        }

        if (layersPerCoincidence >= 3 && channelEnergy[2] != 0.0){

          CoincidenceCount += 1;  

          for (int channelCounter = 0; channelCounter < 16; channelCounter++){
            if (channelEnergy[channelCounter] != 0.0){
              entryTriggered[channelEntry[channelCounter]] = true; // Mark this entry as triggered
              channelEC[channelCounter]->Fill(channelEnergy[channelCounter]);
              TwoDLayoutEC->Fill(layoutX[channelCounter], layoutY[channelCounter], static_cast<double>(channelEnergy[channelCounter]));
                
              EChistosFilled += 1;

              eventEnergy += channelEnergy[channelCounter];

              channelsPerCoincidenceHisto->Fill(channelCounter);
              TwoDLayoutTriggered->Fill(layoutX[channelCounter], layoutY[channelCounter]);

              if (channelCounter == 2 || channelCounter == 5 || channelCounter == 8 || channelCounter == 11 || channelCounter == 14){
                layersPerCoincidenceHisto->Fill(layoutX[channelCounter]);
              }

              if (channelCounter > 0){
                hitsTimeDifference->Fill(channelTimeDifference[channelCounter]);
              }

            }
          }

          totalChannelsPerCoincidenceHisto->Fill(channelsPerCoincidence);
          totalLayersPerCoincidenceHisto->Fill(layersPerCoincidence);

          totalEventEnergy->Fill(eventEnergy);
          totalEventEnergyN->Fill(eventEnergy/channelsPerCoincidence);
          ENvTime->Fill(static_cast<double>(triggerTimestamp*1e-9), eventEnergy/channelsPerCoincidence);

        }
      }
  }

  std::cout << "File counter: " << fileNumber << std::endl;

  for(int i = 0; i < 16; i++){
    channelECRatio[i] = static_cast<TH1D*>(channelEC[i]->Clone());
    channelECRatio[i]->Divide(channelE[i]);
  }

  std::cout << "Number of Coincidences: " << CoincidenceCount << std::endl;
  std::cout << "Number of Entries: " << noEntries << std::endl;

  std::cout << "Number of energy histograms filled: " << EhistosFilled << std::endl;
  std::cout << "Number of coincidence histograms filled: " << EChistosFilled << std::endl;

  TFile OutFile(OutFileName, "recreate"); // Outputname, an arg or something determined by input name?
  gDirectory->mkdir("ChannelEnergyHistos"); 
  gDirectory->cd("ChannelEnergyHistos");

  for(int channel = 0; channel < 16; channel++){
    channelE[channel]->Write();
  }

  gDirectory->cd("../");
  
  gDirectory->mkdir("CoincidenceChannelHistos"); 
  gDirectory->cd("CoincidenceChannelHistos");

  for(int channel = 0; channel < 16; channel++){
    channelEC[channel]->Write();
  }

  gDirectory->cd("../");

  gDirectory->mkdir("CoincidenceChannelERatioHistos");
  gDirectory->cd("CoincidenceChannelERatioHistos");
  for(int channel = 0; channel < 16; channel++){
    channelECRatio[channel]->Write();
  }
  gDirectory->cd("../");

  gDirectory->mkdir("TimingHistos"); 
  gDirectory->cd("TimingHistos"); 
  hitsTimeDifference->Write();
  energyTime->Write();
  ENvTime->Write();
  gDirectory->cd("../");

  gDirectory->mkdir("EnergyHistos"); 
  gDirectory->cd("EnergyHistos"); 
  totalEventEnergy->Write();
  totalEventEnergyN->Write();
  TwoDLayoutE->Write();
  TwoDLayoutEC->Write();
  gDirectory->cd("../");

  gDirectory->mkdir("ChannelsTriggerHistos"); 
  gDirectory->cd("ChannelsTriggerHistos"); 
  totalChannelsPerCoincidenceHisto->Write();
  channelsPerCoincidenceHisto->Write();
  totalLayersPerCoincidenceHisto->Write();
  layersPerCoincidenceHisto->Write();
  TwoDLayoutTriggered->Write();
  gDirectory->cd("../");
  
  OutFile.Close(); 


}
