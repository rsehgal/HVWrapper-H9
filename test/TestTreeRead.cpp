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
int main(int argc, char *argv[]){


	PowerSupply *ps=0;
	PowerSupply *ps2=0;
    //ULong64_t tStamp;
	UInt_t tStamp;

	gROOT->Reset();
   	TFile *hvdata;// = (TFile*)gROOT->GetListOfFiles()->FindObject("HVData.root");
	TTree *HVData;
		std::string outputFileName=argv[1];//"HVData.root";
		hvdata = new TFile(outputFileName.c_str(),"READ");
                HVData = (TTree*)hvdata->Get("HVData");
		// Set branch addresses.
		//PowerSupply *hvTop=0;
		//PowerSupply *hvBottom=0;
		HVData->SetBranchAddress("HVTop",&ps);
		HVData->SetBranchAddress("HVBottom",&ps2);
		HVData->SetBranchAddress("TimeStamp",&tStamp);

	Long64_t nentries = HVData->GetEntries();

	Long64_t nbytes = 0;
       for (Long64_t i=0; i<nentries;i++) {
	 std::cout << "=============Entry  : " << i << " =================" << std::endl;
         nbytes += HVData->GetEntry(i);
         std::cout << "..................Time : " << tStamp << ".............." << std::endl;
         std::cout << "................. HVTop .................." << std::endl;
         Print(ps);
         std::cout << "................. HVBottom .................." << std::endl;
         Print(ps2);
	}

        
/*	tStamp = time(0);
	HVData->Fill();
	HVData->Write();
	hvdata->Close();
*/
}

