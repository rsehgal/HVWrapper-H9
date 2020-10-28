/*
 * ReadPSUsingConfFile.cpp
 *
 *  Created on: 28-Oct-2020
 *      Author: rsehgal
 */

#include "PowerSupplyConfReader.h"
#include <TH1F.h>
#include "Loader.h"
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
int main(){
	//gROOT->ProcessLine(".L /home/ismran/Work/geant4/ismran/Loader.C+");
	std::cout << "============================================" << std::endl;
	std::vector<std::string> psfileVector;
	psfileVector.push_back("PowerSupply1.txt");
	psfileVector.push_back("PowerSupply2.txt");

	ReadPowerSupplies psV(psfileVector);
	psV.ReadVoltageAndCurrentOfAllPowerSupplies();

	PowerSupply psTop = psV.fPsVector[0];
	PowerSupply psBottom = psV.fPsVector[1];


	std::string outputFileName = "HVData.root";

	std::ifstream f(outputFileName);
	if(f.good()){
		 f.close();
		 //Open the required ROOT file in append mode
		 TFile *hvdata = new TFile(outputFileName.c_str(),"UPDATE");
		 TTree *hvDataTree = (TTree*)hvdata->Get("HVData");

		 hvDataTree->Fill();
		 hvDataTree->Write();
		 hvdata->Close();
		//std::cout << "FILE ExIST>..........." << std::endl;
	}else{
		f.close();
		 //Create a new ROOT file with desired name
		 TFile *hvdata = new TFile(outputFileName.c_str(),"CREATE");
		 //PowerSupply ps;
		 TTree *hvDataTree = new TTree("HVData","HVData");
		 hvDataTree->Branch("HVTop","PowerSupply", &psTop);
		 hvDataTree->Branch("HVTop","PowerSupply", &psBottom);
		 hvDataTree->Fill();
		 hvDataTree->Write();
		 hvdata->Close();
	}

	return 0;

}

