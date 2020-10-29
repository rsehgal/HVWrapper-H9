/*
 * TestWritingVecOfStruct.cpp
 *
 *  Created on: 28-Oct-2020
 *      Author: rsehgal
 */

#include "HVDataClass.h"
//#include "Loader.h"
#include <fstream>
#include <TFile.h>
#include <TTree.h>
#include <TROOT.h>
#include <TRint.h>

int main(){

	//gROOT->ProcessLine(".L /home/rsehgal/TrackingMod.restart.08.June/CAEN/HVWrapper-H9/inc/Loader.h");

	Channel ch1(100.,99.,10,9.08);
	Channel ch2(102.,98.,10,9.01);
	Channel ch3(103.,97.,10,9.07);
	Channel ch4(104.,96.,9.9,9.03);

	Slot s;
	s.push_back(ch1);
	s.push_back(ch2);
	s.push_back(ch3);
	s.push_back(ch4);

	PowerSupply ps;
	ps.push_back(s);
	ps.push_back(s);
	ps.push_back(s);

	std::string outputFileName = "HVData.root";
	TFile *hvdata = new TFile(outputFileName.c_str(),"RECREATE");
	TTree *hvDataTree = new TTree("HVData","HVData");
	hvDataTree->Branch("HVTop","PowerSupply", &ps);
	hvDataTree->Fill();
	hvDataTree->Write();
	hvdata->Close();

}

