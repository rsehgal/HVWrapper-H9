/*
 * PlotVmonImon.cpp
 *
 *  Created on: 04-Nov-2020
 *      Author: rsehgal
 */

#include "HVDataClass.h"
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TRint.h>
#include <ctime>
#include <iostream>
#include <TGraph.h>
#include <vector>
#include <TApplication.h>
#include <TCanvas.h>
#include "PowerSupplyConfReader.h"
#include "TAxis.h"

int main(int argc, char *argv[]){


  const char *path = getenv("HVWRAPPER");
  
  PowerSupplyConfReader *hvTopConf = new PowerSupplyConfReader(Form("%s/config_files/PowerSupply1.txt",path));
  PowerSupplyConfReader *hvBottomConf = new PowerSupplyConfReader(Form("%s/config_files/PowerSupply2.txt",path));

  TApplication *fApp = new TApplication("Test", NULL, NULL);

  PowerSupply *hvTop=0;
  PowerSupply *hvBottom=0;
  UInt_t tStamp;

  gROOT->Reset();

  std::string inputFileName     = argv[1];
  unsigned short psIndex        = std::stoi(argv[2]);
  unsigned short slotIndex      = std::stoi(argv[3]);
  unsigned short channelIndex   = std::stoi(argv[4]);

  TFile *hvdata = new TFile(inputFileName.c_str(),"READ");
  TTree *HVData = (TTree*)hvdata->Get("HVData");;
  HVData->SetBranchAddress("HVTop"     ,&hvTop);
  HVData->SetBranchAddress("HVBottom"  ,&hvBottom);
  HVData->SetBranchAddress("TimeStamp" ,&tStamp);

  Long64_t nentries = HVData->GetEntries();

  Long64_t nbytes = 0;

  TCanvas *can =  new TCanvas("can","HVMon",1520,727);
  can->Divide(2,1);

  std::vector<float> tStampVec;
  std::vector<float> vMonVec;
  std::vector<float> iMonVec;
  std::vector<float> xStampVec;
  int count = 0;

  LoadCrateMap();

  for (Long64_t i=0; i<nentries;i++) {
    count++;
    xStampVec.push_back(count);
    //std::cout << "=============Entry  : " << i << " =================" << std::endl;
    nbytes += HVData->GetEntry(i);
    /*
      std::cout << "..................Time : " << tStamp << ".............." << std::endl;
      std::cout << "................. HVTop .................." << std::endl;
      Print(hvTop);
      std::cout << "................. HVBottom .................." << std::endl;
      Print(hvBottom);
    */

    tStampVec.push_back(tStamp);

    PowerSupply ps;
    PowerSupplyConfReader *conf=0;
    if(psIndex == 1){
      ps = *hvTop;
      conf = hvTopConf;
    }
    if(psIndex == 2){
      ps = *hvBottom;
      conf=hvBottomConf;
    }

    for(unsigned short int i = 0; i < (conf->fSlotVector).size() ; i++){
      if((conf->fSlotVector)[i] == slotIndex){
	for(unsigned short int j = 0; j < (conf->fVectOfChannelVector)[i].size() ; j++){
	  if((conf->fVectOfChannelVector)[i][j] == channelIndex){
	    vMonVec.push_back(ps[i][j].sVMon);
	    iMonVec.push_back(ps[i][j].sIMon);
	  }
	}
      }
    }

    /*
      if(psIndex == 1){
      PowerSupply ps = *hvTop;
      vMonVec.push_back(Top[slotIndex][channelIndex].sVMon);
      iMonVec.push_back(Top[slotIndex][channelIndex].sIMon);
      }
      if(psIndex == 2){
      PowerSupply ps = *hvBottom;
      vMonVec.push_back(Bottom[slotIndex][channelIndex].sVMon);
      iMonVec.push_back(Bottom[slotIndex][channelIndex].sIMon);
      }*/

  }
  TGraph *grVmon=new TGraph(tStampVec.size());
  grVmon->SetTitle("VMon");
  grVmon->SetMarkerStyle(20);
  grVmon->SetMarkerSize(0.4);
  grVmon->SetLineColor(1);
  grVmon->SetMarkerColor(1);
  grVmon->GetXaxis()->LabelsOption("v");
  grVmon->GetXaxis()->SetNdivisions(508);
  grVmon->GetXaxis()->SetTimeDisplay(1);
  grVmon->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%b:%d}");
  grVmon->GetXaxis()->SetLabelOffset(0.029);

  TGraph *grImon = new TGraph(tStampVec.size());
  grImon->SetTitle("IMon");
  grImon->SetMarkerStyle(20);
  grImon->SetMarkerSize(0.4);
  grImon->SetLineColor(2);
  grImon->SetMarkerColor(2);
  grImon->GetXaxis()->LabelsOption("v");
  grImon->GetXaxis()->SetNdivisions(508);
  grImon->GetXaxis()->SetTimeDisplay(1);
  grImon->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%b:%d}");
  grImon->GetXaxis()->SetLabelOffset(0.029);
  
  std::cout <<"==================================" << std::endl;
  std::cout << "DataSize : "  << tStampVec.size() << std::endl;
  for(unsigned short i = 0 ; i < tStampVec.size() ; i++){

    //if( vMonVec[i] > 0 ){
    grVmon->SetPoint(i,tStampVec[i],vMonVec[i]);
    grImon->SetPoint(i,tStampVec[i],iMonVec[i]);
    //}
    
    //std::cout << "TStamp : " << tStampVec[i] <<" : VMon : " << vMonVec[i] << " : IMon : " << iMonVec[i] << std::endl;
  }

  can->cd(1);
  //grVmon->SetMaximum(1200.0);
  grVmon->SetMinimum(800.0);
  grVmon->Draw("alp");
  can->cd(2);
  grImon->Draw("alp");  

  // #ifdef USE_TIME_ON_AXIS
  //   TGraph *grVmon = new TGraph(tStampVec.size(),&tStampVec[0],&vMonVec[0]);
  //   TGraph *grImon = new TGraph(tStampVec.size(),&tStampVec[0],&iMonVec[0]);
  // #else
  //   TGraph *grVmon = new TGraph(xStampVec.size(),&xStampVec[0],&vMonVec[0]);
  //   TGraph *grImon = new TGraph(xStampVec.size(),&xStampVec[0],&iMonVec[0]);
  // #endif


  //   can->cd(1);
  // #ifdef USE_TIME_ON_AXIS
  //   grVmon->GetXaxis()->LabelsOption("v");
  //   grVmon->GetXaxis()->SetNdivisions(508);
  //   grVmon->GetXaxis()->SetTimeDisplay(1);
  //   grVmon->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%b:%d}");
  // #endif
  //   //grVmon->GetXaxis()->LabelsOption("v");
  //   grVmon->Draw("alp");

  //   can->cd(2);
  //   grImon->SetLineColor(4);
  //   grImon->SetMarkerColor(6);
  // #ifdef USE_TIME_ON_AXIS    
  //   grImon->GetXaxis()->LabelsOption("v");
  //   grImon->GetXaxis()->SetNdivisions(508);
  //   grImon->GetXaxis()->SetTimeDisplay(1);
  //   grImon->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%b:%d}");
  //   // grImon->GetXaxis()->SetTimeDisplay(1);
  //   // grImon->GetXaxis()->SetTimeFormat("%Y %H:%M %F 1970-01-01 00:00:00");
  // #endif
  //   grImon->Draw("alp");

  fApp->Run();

  return 0;

}
