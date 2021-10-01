/*
 * PlotVmonImon.cpp
 *
 *  Created on: 04-Nov-2020
 *      Author: rsehgal
 */

#include "utilities.h"

#include "HVDataClass.h"

#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TRint.h>
#include <TGraph.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <TGaxis.h>
#include <TChain.h>
#include <TAxis.h>

#include "PowerSupplyConfReader.h"


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <ctime>

int main(int argc, char *argv[]){

  const char *path = "..";//getenv("HVWRAPPER");

  PowerSupplyConfReader *hvTopConf    = new PowerSupplyConfReader(Form("%s/conf_files/PowerSupply1.txt",path));
  PowerSupplyConfReader *hvBottomConf = new PowerSupplyConfReader(Form("%s/conf_files/PowerSupply2.txt",path));

  TApplication *fApp = new TApplication("Test", NULL, NULL);

  PowerSupply *hvTop=0;
  PowerSupply *hvBottom=0;
  UInt_t tStamp;

  gROOT->Reset();
  //LoadStyle();
  std::string inputdir = Form("%s/rootfiles/",path);
  //std::string outputFileName=argv[1];

  //! PS 12 => Dec : 1 3 10,  Feb 2 3  6
  //! PS 40 => Dec : 1 15 6,  Feb 1 15 6 
  //! PS 50 => Dec : 2 0 2 ,  Feb 2 5 10 

  double voltMin = 600;
  double voltMax = 1400;
  
  std::vector<string> inputFileNames={
    //"HVMon-08-Dec-2020.root",
    // "HVMon-09-Dec-2020.root",
    // "HVMon-10-Dec-2020.root"
     //"HVMon-01-Feb-2021.root",
    // "HVMon-02-Feb-2021.root",
    // "HVMon-03-Feb-2021.root"
    // "HVMon-17-Feb-2021.root",
    // "HVMon-18-Feb-2021.root"
    //"HVMon-15-Mar-2021-MuonTG90.root",
    // "HVMon-19-Mar-2021-MuonTG90.root",
    // "HVMon-22-Mar-MuonTG90.root",
    //   "HVMon-07-Apr-2021.root"
    "HVMon-Dhruva-28Sep2021.root",
    "HVMon-Dhruva-29Sep2021.root"
  };
  unsigned short psIndex      = std::stoi(argv[1]);
  unsigned short slotIndex    = std::stoi(argv[2]);
  unsigned short channelIndex = std::stoi(argv[3]);

  TChain *HVData = new TChain("HVData");
  for(int i=0; i<(int)inputFileNames.size(); i++){
    HVData->AddFile((inputdir+inputFileNames[i]).c_str());
  }
  HVData->SetBranchAddress("HVTop"    ,&hvTop);
  HVData->SetBranchAddress("HVBottom" ,&hvBottom);
  HVData->SetBranchAddress("TimeStamp",&tStamp);

  Long64_t nentries = HVData->GetEntries();

  Long64_t nbytes = 0;

  std::vector<float> tStampVec;
  std::vector<float> vMonVecNear, vMonVecFar, verrMon;
  std::vector<float> iMonVecNear, iMonVecFar, ierrMon;
  int count = 0;

  double tMin = 0, tMax = 0;

  UInt_t nmins = 20;
  UInt_t deltaTime = nmins*60;

  UInt_t PrevTstamp=0;

  for (Long64_t i=0; i<nentries;i++) {

    count++;
    //std::cout << "=============Entry  : " << i << " =================" << std::endl;
    nbytes += HVData->GetEntry(i);
    /*
      std::cout << "..................Time : " << tStamp << ".............." << std::endl;
      std::cout << "................. HVTop .................." << std::endl;
      Print(hvTop);
      std::cout << "................. HVBottom .................." << std::endl;
      Print(hvBottom);
    */

    if( i == 0 ) tMin = tStamp;
    else tMax = tStamp;


    UInt_t delt = tStamp - PrevTstamp;
    if( PrevTstamp == 0 )PrevTstamp=tStamp;
    else if( delt >= deltaTime ){
      PrevTstamp = 0;

      tStampVec.push_back(tStamp);

      PowerSupply ps;
      PowerSupplyConfReader *conf=0;
      if(psIndex == 1){
	ps   = *hvTop;
	conf =  hvTopConf;
      }
      if(psIndex == 2){
	ps   = *hvBottom;
	conf =  hvBottomConf;
      }
      
      int idxNear = ( channelIndex % 2 == 0 ) ? channelIndex : channelIndex - 1;
      int idxFar  = idxNear+1;
      
      for(unsigned short int i = 0; i < (conf->fSlotVector).size() ; i++){
	if((conf->fSlotVector)[i] == slotIndex){
	  for(unsigned short int j = 0; j < (conf->fVectOfChannelVector)[i].size() ; j++){
	    if((conf->fVectOfChannelVector)[i][j] == idxNear ){
	      vMonVecNear.push_back(ps[i][j].sVMon);
	      iMonVecNear.push_back(ps[i][j].sIMon);
	    }else if((conf->fVectOfChannelVector)[i][j] == idxFar ){
	      vMonVecFar.push_back(ps[i][j].sVMon);
	      iMonVecFar.push_back(ps[i][j].sIMon);
	    }
	  }
	}
      }
    }
  }
  std::cout <<"==================================" << std::endl;
  std::cout << "DataSize : "  << tStampVec.size() << " " << tMin << "\t" << tMax << std::endl;
  
  for(int i=0; i<(int)vMonVecNear.size(); i++){
    verrMon.push_back(0);
    ierrMon.push_back(0);
  }

  TGraphErrors *grVmonNear = new TGraphErrors(tStampVec.size(),&tStampVec[0],&vMonVecNear[0],&verrMon[0]);
  TGraphErrors *grImonNear = new TGraphErrors(tStampVec.size(),&tStampVec[0],&iMonVecNear[0],&ierrMon[0]);

  TGraphErrors *grVmonFar  = new TGraphErrors(tStampVec.size(),&tStampVec[0],&vMonVecFar [0],&verrMon[0]);
  TGraphErrors *grImonFar  = new TGraphErrors(tStampVec.size(),&tStampVec[0],&iMonVecFar [0],&ierrMon[0]);

  grVmonNear->SetName("grVmonNear_PS50");
  grImonNear->SetName("grImonNear_PS50");
  grVmonFar ->SetName("grVmonFar_PS50");  
  grImonFar ->SetName("grImonFar_PS50");

  grVmonNear->SetTitle("PS50");
  grVmonNear->SetMarkerColor(kBlack);
  grVmonNear->SetMarkerStyle(20);
  grVmonNear->SetMarkerSize(0.9);
  grVmonNear->SetLineColor(kBlack);
  grVmonNear->SetLineStyle(kBlack);

  grVmonFar->SetTitle("PS50");
  grVmonFar->SetLineColor(kGray+2);
  grVmonFar->SetMarkerColor(kGray+2);
  grVmonFar->SetMarkerStyle(24);
  grVmonFar->SetMarkerSize(0.9);
  grVmonFar->SetLineColor(kGray);

  //MakeGraph(grVmonNear," Time (sec) "," Voltage (in volt)");
  //MakeGraph(grVmonFar ," Time (sec) "," Voltage (in volt)");
  
  //grVmonNear->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
  //grVmonNear->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%d-%b-%y} %F 1970-01-01 00:00:00 ");
  //grVmonFar ->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
  //grVmonFar ->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%d-%b-%y} %F 1970-01-01 00:00:00 ");
  
  grVmonNear->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
  grVmonNear->GetXaxis()->SetTimeFormat("%H");
  grVmonFar ->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
  grVmonFar ->GetXaxis()->SetTimeFormat("%H");


  grImonNear->SetTitle("PS50");
  grImonNear->SetMarkerColor(kRed);
  grImonNear->SetMarkerStyle(20);
  grImonNear->SetMarkerSize(0.9);
  grImonNear->SetLineColor(kRed);
  grImonNear->SetLineStyle(kRed);

  grImonFar->SetTitle("PS50");
  grImonFar->SetMarkerColor(kRed-7);
  grImonFar->SetMarkerStyle(24);
  grImonFar->SetMarkerSize(0.9);
  grImonFar->SetLineColor(kRed-7);
  grImonFar->SetLineStyle(kRed-7);

  //MakeGraph(grImonNear," Time (sec) "," Current (in #muA)");
  //MakeGraph(grImonFar," Time (sec) "," Current (in #muA)");
  //grImonNear->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
  //grImonNear->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%d-%b} %F 1970-01-01 00:00:00 ");
  //grImonFar->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
  //grImonFar->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%d-%b} %F 1970-01-01 00:00:00 ");
  
  
  grImonNear->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
  grImonNear->GetXaxis()->SetTimeFormat("%H");
  grImonFar->GetHistogram()->GetXaxis()->SetTimeDisplay(1);
  grImonFar->GetXaxis()->SetTimeFormat("%H");


  TCanvas *canNear =  new TCanvas("canNear","Near",/*1449,695*/898,792);
  TPad *padN = new TPad("padN","",0,0,1,1);
  padN->Draw();
  padN->cd();
  padN->SetLeftMargin(0.17);
  padN->SetBottomMargin(0.20);
  padN->SetRightMargin(0.17);

  TH1D *hrN = (TH1D*)canNear->DrawFrame(tMin,voltMin,tMax,voltMax);
  //MakeHist(hrN,"Time (sec)","Voltage (in volt)");
  hrN->GetXaxis()->SetTimeDisplay(1);
  hrN->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%d-%b-%y} %F 1970-01-01 00:00:00 ");
  //hrN->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{} %F 1970-01-01 00:00:00 ");
  hrN->GetXaxis()->SetLabelOffset(0.042);
  hrN->GetXaxis()->SetTitleOffset(2.15);
  hrN->GetXaxis()->CenterTitle();
  hrN->GetXaxis()->SetNdivisions(506);
  hrN->GetYaxis()->SetNdivisions(507);
  hrN->GetYaxis()->SetLabelSize(0.04);
  hrN->GetXaxis()->SetTitleSize(0.04);
  hrN->GetYaxis()->SetTitleSize(0.04);
  hrN->GetYaxis()->CenterTitle();
  hrN->SetXTitle("Time ");
  hrN->SetYTitle("Voltage (in volt)");

  //hr->Draw();
  grVmonNear->Draw("p");
  grVmonFar->Draw("psame");

  canNear->cd();
  TPad *overlayN = new TPad("overlayN","ovNear",0,0,1,1);
  overlayN->SetFillStyle(0);
  overlayN->SetFillColor(0);
  overlayN->SetFrameFillStyle(0);
  overlayN->Draw("FA");
  overlayN->cd();
  overlayN->SetLeftMargin(0.17);
  overlayN->SetBottomMargin(0.20);
  overlayN->SetRightMargin(0.17);

  Double_t xminN = padN->GetUxmin();
  Double_t yminN = 0;
  Double_t xmaxN = padN->GetUxmax();
  Double_t ymaxN = 300;
  cout << "xmin : " << xminN << "\t xmax : " <<  xmaxN << endl;

  TH1D *hframeN = (TH1D*)overlayN->DrawFrame(xminN,yminN,xmaxN,ymaxN);
  //MakeHist(hframeN,"Time (sec)","Voltage (in volt)");
  hframeN->GetXaxis()->SetTimeDisplay(1);
  hframeN->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%d-%b-%y} %F 1970-01-01 00:00:00 ");
  //hframeN->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{} %F 1970-01-01 00:00:00 "); 
  hframeN->GetXaxis()->SetLabelOffset(99);
  hframeN->GetXaxis()->CenterTitle();
  hframeN->GetYaxis()->SetTickLength(0);
  hframeN->GetYaxis()->SetLabelOffset(99);
  hframeN->GetXaxis()->SetTitleSize(0.04);
  hframeN->GetXaxis()->SetTitleOffset(2.15);
  hframeN->GetXaxis()->SetNdivisions(506);
  hframeN->GetXaxis()->SetLabelOffset(0.042);
  hframeN->GetYaxis()->SetLabelSize(0.04);
  hframeN->GetYaxis()->SetNdivisions(507);
  hframeN->GetYaxis()->CenterTitle();
  // hframeN->SetXTitle("Time (sec)");
  // hframeN->SetYTitle("Current (in #muA)");

  grImonNear->Draw("psame");
  grImonFar->Draw("psame");
  
  //Draw an axis on the right side
  TGaxis *axisN = new TGaxis(xmaxN,yminN,xmaxN, ymaxN,yminN,ymaxN,510,"+L");
  axisN->SetTitle("Current (in #muA)");
  axisN->CenterTitle();
  axisN->SetTitleFont(42);
  axisN->SetTitleOffset(1.98);
  axisN->SetTitleSize(0.04);
  axisN->SetLabelSize(0.04);
  axisN->SetLabelFont(42);
  axisN->SetLineColor(kRed);
  axisN->SetLabelColor(kRed);
  axisN->SetTitleColor(kRed);
  axisN->Draw();



  ///////////////////
  
 /* TCanvas *canFar =  new TCanvas("canFar","Far",898,792);
  TPad *padF = new TPad("padF","",0,0,1,1);
  padF->Draw();
  padF->cd();
  padF->SetLeftMargin(0.17);
  padF->SetBottomMargin(0.20);
  padF->SetRightMargin(0.17);

  TH1D *hrF = (TH1D*)canFar->DrawFrame(tMin,voltMin,tMax,voltMax);
  //MakeHist(hrF,"Time (sec)","Voltage (in volt)");
  hrF->GetXaxis()->SetTimeDisplay(1);
  hrF->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%d-%b} %F 1970-01-01 00:00:00 ");
  hrF->GetXaxis()->SetLabelOffset(0.042);
  hrF->GetXaxis()->SetTitleOffset(2.15);
  hrF->GetXaxis()->CenterTitle();
  hrF->GetXaxis()->SetNdivisions(508);
  hrF->GetYaxis()->SetNdivisions(507);
  hrF->GetYaxis()->SetLabelSize(0.04);
  hrF->GetYaxis()->CenterTitle();
  hrF->SetXTitle("Time (sec)");
  hrF->SetYTitle("Voltage (in volt)");

  grVmonFar->Draw("p");

  canFar->cd();
  TPad *overlayF = new TPad("overlayF","ovFar",0,0,1,1);
  overlayF->SetFillStyle(0);
  overlayF->SetFillColor(0);
  overlayF->SetFrameFillStyle(0);
  overlayF->Draw("FA");
  overlayF->cd();
  overlayF->SetLeftMargin(0.17);
  overlayF->SetBottomMargin(0.20);
  overlayF->SetRightMargin(0.17);

  Double_t xminF = padF->GetUxmin();
  Double_t yminF = 0;
  Double_t xmaxF = padF->GetUxmax();
  Double_t ymaxF = 200;
  cout << "xmin : " << xminF << "\t xmax : " <<  xmaxF << endl;

  TH1D *hframeF = (TH1D*)overlayF->DrawFrame(xminF,yminF,xmaxF,ymaxF);
  //MakeHist(hframeF,"Time (sec)","Voltage (in volt)");
  hframeF->GetXaxis()->SetTimeDisplay(1);
  hframeF->GetXaxis()->SetTimeFormat("#splitline{%H:%M}{%d-%b} %F 1970-01-01 00:00:00 ");
  hframeF->GetXaxis()->SetLabelOffset(99);
  hframeF->GetXaxis()->CenterTitle();
  hframeF->GetYaxis()->SetTickLength(0);
  hframeF->GetYaxis()->SetLabelOffset(99);
  // hframeF->GetXaxis()->SetTitleSize(0.04);
  hframeF->GetXaxis()->SetTitleOffset(2.15);
  hframeF->GetXaxis()->SetNdivisions(508);
  hframeF->GetXaxis()->SetLabelOffset(0.042);
  hframeF->GetYaxis()->SetNdivisions(507);
  hframeF->GetYaxis()->CenterTitle();
  // hframeF->SetXTitle("Time (sec)");
  // hframeF->SetYTitle("Current (in #muA)");

  grImonFar->Draw("psame");
  
  //Draw an axis on the right side
  TGaxis *axisF = new TGaxis(xmaxF,yminF,xmaxF, ymaxF,yminF,ymaxF,510,"+L");
  axisF->SetTitle("Current (in #muA)");
  axisF->CenterTitle();
  axisF->SetTitleFont(42);
  axisF->SetTitleOffset(1.98);
  axisF->SetLabelSize(0.04);
  axisF->SetLabelFont(42);
  axisF->SetLineColor(kRed);
  axisF->SetLabelColor(kRed);
  axisF->SetTitleColor(kRed);
  axisF->Draw();

*/

  TLegend *leg = new TLegend(0.6593023,0.6571429,0.9290698,0.9571429, "","brNDC");
  leg->SetHeader("");
  leg->SetTextSize(0.04);
  leg->AddEntry(grVmonNear ,"VNear" ,"p");
  leg->AddEntry(grVmonFar  ,"VFar"  ,"p");
  leg->AddEntry(grImonNear ,"INear" ,"p");
  leg->AddEntry(grImonFar  ,"IFar"  ,"p"); 
  leg->Draw();  

  TFile *fout = new TFile("outputHist.root","recreate");
  fout->cd();
  grVmonNear->Write();
  grVmonFar->Write();
  grImonNear->Write();
  grImonFar->Write();
  fout->Close();  

  fApp->Run();

  return 0;

}
