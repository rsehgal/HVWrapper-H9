/*
 * ReadPSUsingConfFile.cpp
 *
 *  Created on: 28-Oct-2020
 *      Author: rsehgal
 */

#include "PowerSupplyConfReader.h"
#include <TH1F.h>
//#include "Loader.h"
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TSystem.h>
#include <ctime>
int main(int argc, char *argv[]){

    std::string outputFileName = argv[1];//"HVData.root";
#ifdef VERBOSE
    std::cout << "OutputFileName :  " << outputFileName << std::endl;
    std::cout << "============================================" << std::endl;
#endif
	std::vector<std::string> psfileVector;
	psfileVector.push_back("PowerSupply1.txt");
	psfileVector.push_back("PowerSupply2.txt");

	ReadPowerSupplies psV(psfileVector);
	psV.ReadVoltageAndCurrentOfAllPowerSupplies();

	PowerSupply *psTop = psV.fPsVector[0];
	PowerSupply *psBottom = psV.fPsVector[1];
	//ULong64_t tStamp = time(0);
	UInt_t tStamp = time(0);

	std::ifstream f(outputFileName);
	if(f.good()){
		 f.close();
#ifdef VERBOSE
		 std::cout << "File exist, hence opening it in Update mode ...." << std::endl;
#endif
		 //Open the required ROOT file in append mode
		 TFile *hvdata = new TFile(outputFileName.c_str(),"UPDATE");
		 TTree *HVData = (TTree*)hvdata->Get("HVData");
		 //Declaration of leaves types
		 //vector<vector<HVDataClass> > HVTop;
		 //vector<vector<HVDataClass> > HVBottom;
		 //UInt_t          tStamp;

		 // Set branch addresses.
		 HVData->SetBranchAddress("HVTop",&psTop);
		 HVData->SetBranchAddress("HVBottom",&psBottom);
		 HVData->SetBranchAddress("TimeStamp",&tStamp);

		 HVData->Fill();
		 HVData->Write();
		 hvdata->Close();
		//std::cout << "FILE ExIST>..........." << std::endl;
	}else{
		f.close();
		 //Create a new ROOT file with desired name
#ifdef VERBOSE
		std::cout << "File does not exist hence creating a new file........." << std::endl;
#endif
		 TFile *hvdata = new TFile(outputFileName.c_str(),"CREATE");
		 //PowerSupply ps;
		 TTree *hvDataTree = new TTree("HVData","HVData");
		 hvDataTree->Branch("TimeStamp", &tStamp, "tStamp/i");
		 hvDataTree->Branch("HVTop","PowerSupply", &psTop);
		 hvDataTree->Branch("HVBottom","PowerSupply", &psBottom);

		 hvDataTree->Fill();
		 hvDataTree->Write();
		 hvdata->Close();
	}

	return 0;

}

