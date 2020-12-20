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

  PowerSupply *hvTop=0;
  PowerSupply *hvBottom=0;
  UInt_t tStamp;

  gROOT->Reset();

  std::string outputFileName=argv[1];
  /*unsigned short psIndex = std::stoi(argv[2]);
  unsigned short slotIndex = std::stoi(argv[3]);
  unsigned short channelIndex = std::stoi(argv[4]);*/

  unsigned short psIndexx = 0;
  unsigned short slotIndexx = 0;
  unsigned short channelIndexx = 0;


  TFile *hvdata = new TFile(outputFileName.c_str(),"READ");;
  TTree *HVData = (TTree*)hvdata->Get("HVData");;
  HVData->SetBranchAddress("HVTop",&hvTop);
  //HVData->SetBranchAddress("HVTop_SY4527",&hvTop);
  HVData->SetBranchAddress("HVBottom",&hvBottom);
  //HVData->SetBranchAddress("HVBottom_SY5527",&hvBottom);
  HVData->SetBranchAddress("TimeStamp",&tStamp);

  Long64_t nentries = HVData->GetEntries();

  Long64_t nbytes = 0;


  std::vector<float> tStampVec;
  std::vector<float> vMonVec;
  std::vector<float> iMonVec;
  std::vector<float> xStampVec;
  int count = 0;

  std::ofstream outfile("test.csv",std::ios::app);

std::vector<std::string> psfileVector;
  psfileVector.push_back("PowerSupply1.txt");
  psfileVector.push_back("PowerSupply2.txt");

  ReadPowerSupplies psV(psfileVector);
  
unsigned psNum = 0 ;
unsigned short int tcounter = 0;
for(unsigned short int i = 0 ; i < psV.fPowerSupplyConfVector.size() ; i++){
    std::cout << "============================================" << std::endl;
    psNum++;
    psIndexx = psNum;
    PowerSupplyConfReader *psConf = psV.fPowerSupplyConfVector[i];
    for(unsigned int slotIndex = 0 ; slotIndex < psConf->fNumOfSlots ; slotIndex++){
      //std::cout << "PowerSuppy : " << psNum << " : SlotNum : " << psConf->fSlotVector[slotIndex] << std::endl;
      //std::cout << " -------------- :  Num Of Channels : " << psConf->fVectOfChannelVector[slotIndex].size() << std::endl;
      slotIndexx = psConf->fSlotVector[slotIndex];

      for(unsigned short int j = 0 ; j < psConf->fVectOfChannelVector[slotIndex].size() ; j++){
  
          channelIndexx = psConf->fVectOfChannelVector[slotIndex][j];

          std::cout << "Getting Data for PS : " << psIndexx << " : Slot : " << slotIndexx << " : Channel : " << channelIndexx << std::endl;
          std::string hv="";
          if(psIndexx==1)
            hv = "HVTop";
          if(psIndexx==2)
            hv = "HVBottom";
          std::string plotName = hv+"_Slot_"+std::to_string(slotIndexx)+"_Channel_"+std::to_string(channelIndexx);
          vMonVec.clear();
          iMonVec.clear();
          tStampVec.clear();



#if(1)
  unsigned int stepSize = 600;
  for (Long64_t i=0; i<nentries;i++) {
    count++;
    xStampVec.push_back(count);
    //std::cout << "=============Entry  : " << i << " =================" << std::endl;
    nbytes += HVData->GetEntry(i);
     tStampVec.push_back(tStamp);

    PowerSupply ps;
    PowerSupplyConfReader *conf=0;
    if(psIndexx == 1){
      ps = *hvTop;
      conf = hvTopConf;
    }
    if(psIndexx == 2){
      ps = *hvBottom;
      conf=hvBottomConf;
    }

    for(unsigned short int i = 0; i < (conf->fSlotVector).size() ; i++){
      if((conf->fSlotVector)[i] == slotIndexx){
	for(unsigned short int j = 0; j < (conf->fVectOfChannelVector)[i].size() ; j++){
	  if((conf->fVectOfChannelVector)[i][j] == channelIndexx){
	    vMonVec.push_back(ps[i][j].sVMon);
	    iMonVec.push_back(ps[i][j].sIMon);
	  }
	}
      }
    }

  }

std::cout << "Size of TStampVEc : " << tStampVec.size() << std::endl;
std::cout << "Size of VmonVec : " << vMonVec.size() << std::endl;
std::cout << "Size of ImonVec : " << iMonVec.size() << std::endl;

  outfile.setf(std::ios::fixed);
  if(tcounter == 0){
    std::cout << "Storing Timestamp vector........." <<std::endl;
  for(unsigned int i = 0 ; i < tStampVec.size() ; i++){
    
    if(!(i%stepSize))
      outfile << std::setprecision(0) << tStampVec[i]<<",";
  }
  outfile << std::endl;
}
tcounter++;

  outfile << plotName << std::endl;
  for(unsigned int i = 1 ; i < vMonVec.size()-1 ; i++){
	  if(!(i%stepSize))
		  outfile<<vMonVec[i]<<",";
  }
  outfile << std::endl;

  for(unsigned int i = 1 ; i < iMonVec.size()-1 ; i++){
          if(!(i%stepSize))
                  outfile<<iMonVec[i]<<",";
  }
  outfile << std::endl;
  #endif
}
}
} // End of PS loop
  outfile.close();
  return 0;

}
