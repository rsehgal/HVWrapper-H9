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

	PowerSupplyConfReader *hvTopConf = new PowerSupplyConfReader("PowerSupply1.txt");
	PowerSupplyConfReader *hvBottomConf = new PowerSupplyConfReader("PowerSupply2.txt");

	TApplication *fApp = new TApplication("Test", NULL, NULL);

	PowerSupply *hvTop=0;
	PowerSupply *hvBottom=0;
    UInt_t tStamp;

	gROOT->Reset();

	std::string outputFileName=argv[1];
	unsigned short psIndex = std::stoi(argv[2]);
	unsigned short slotIndex = std::stoi(argv[3]);
	unsigned short channelIndex = std::stoi(argv[4]);
   	TFile *hvdata = new TFile(outputFileName.c_str(),"READ");;
	TTree *HVData = (TTree*)hvdata->Get("HVData");;
	HVData->SetBranchAddress("HVTop",&hvTop);
	HVData->SetBranchAddress("HVBottom",&hvBottom);
	HVData->SetBranchAddress("TimeStamp",&tStamp);

	Long64_t nentries = HVData->GetEntries();

	Long64_t nbytes = 0;

	TCanvas *can =  new TCanvas();
	can->Divide(2,1);

	std::vector<float> tStampVec;
	std::vector<float> vMonVec;
	std::vector<float> iMonVec;
	std::vector<float> xStampVec;
	int count = 0;



    for (Long64_t i=0; i<nentries;i++) {
    	count++;
    	xStampVec.push_back(count);
    	 //std::cout << "=============Entry  : " << i << " =================" << std::endl;
         nbytes += HVData->GetEntry(i);
         std::cout << "..................Time : " << tStamp << ".............." << std::endl;
         std::cout << "................. HVTop .................." << std::endl;
         Print(hvTop);
         std::cout << "................. HVBottom .................." << std::endl;
         Print(hvBottom);

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
        				 vMonVec.push_back(ps[i][i].sVMon);
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

    std::cout <<"==================================" << std::endl;
    std::cout << "DataSize : "  << tStampVec.size() << std::endl;
    for(unsigned short i = 0 ; i < tStampVec.size() ; i++){
    	std::cout << "TStamp : " << tStampVec[i] <<" : VMon : " << vMonVec[i] << " : IMon : " << iMonVec[i] << std::endl;
    }

    TGraph *grVmon = new TGraph(tStampVec.size(),&tStampVec[0],&vMonVec[0]);
    TGraph *grImon = new TGraph(tStampVec.size(),&tStampVec[0],&iMonVec[0]);
    grVmon->SetTitle("VMon");
    grVmon->SetMarkerStyle(8);
    grImon->SetTitle("IMon");
    grImon->SetMarkerStyle(8);

    can->cd(1);
    grVmon->SetLineColor(4);
    grVmon->SetMarkerColor(6);
    grVmon->GetXaxis()->SetTimeDisplay(1);
    grVmon->GetXaxis()->SetTimeFormat("%Y %H:%M %F 1970-01-01 00:00:00");
    //grVmon->GetXaxis()->LabelsOption("v");
    grVmon->Draw("alp");

    can->cd(2);
    grImon->SetLineColor(4);
    grImon->SetMarkerColor(6);
    grImon->GetXaxis()->SetTimeDisplay(1);
    grImon->GetXaxis()->SetTimeFormat("%Y %H:%M %F 1970-01-01 00:00:00");
    grImon->Draw("alp");

    fApp->Run();

    return 0;

}
