/*
 * TestWritingVecOfStruct.cpp
 *
 *  Created on: 28-Oct-2020
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
int main(){


	Channel ch1(1,0,100.,99.,10,9.08);
	Channel ch2(1,1,102.,98.,10,9.01);
	Channel ch3(1,2,103.,97.,10,9.07);
	Channel ch4(1,3,104.,96.,9.9,9.03);

	Slot s;
	s.push_back(ch1);
	s.push_back(ch2);
	s.push_back(ch3);
	s.push_back(ch4);

	PowerSupply *ps=new PowerSupply;
	ps->push_back(s);
	ps->push_back(s);
	ps->push_back(s);

	PowerSupply *ps2 = new PowerSupply;
	ps2->push_back(s);
	ps2->push_back(s);
	ps2->push_back(s);

	std::string outputFileName = "HVData.root";
	//ULong64_t tStamp;
	UInt_t tStamp;

	gROOT->Reset();
   	TFile *hvdata;// = (TFile*)gROOT->GetListOfFiles()->FindObject("HVData.root");
	std::ifstream f(outputFileName);
	TTree *HVData;
   	//if (!hvdata) {
	if(!(f.good())){
		f.close();
		std::cout << "File does not exist, hence creating the new one...." << std::endl;
      		hvdata = new TFile("HVData.root", "CREATE");
		HVData = new TTree("HVData","HVData");
		HVData->Branch("HVTop","PowerSupply", &ps);
		HVData->Branch("HVBottom","PowerSupply", &ps2);
		HVData->Branch("TimeStamp", &tStamp, "tStamp/i");
   	}else{
		f.close();
		std::cout << "File exist, hence reopening it in UPDATE mode ...." << std::endl;
		hvdata = new TFile(outputFileName.c_str(),"UPDATE");
                HVData = (TTree*)hvdata->Get("HVData");
		// Set branch addresses.
		//PowerSupply *hvTop=0;
		//PowerSupply *hvBottom=0;
		HVData->SetBranchAddress("HVTop",&ps);
		HVData->SetBranchAddress("HVBottom",&ps2);
		HVData->SetBranchAddress("TimeStamp",&tStamp);

	}

	tStamp = time(0);
	HVData->Fill();
	HVData->Write();
	hvdata->Close();

}

