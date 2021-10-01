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
#include <TString.h>
#include <ctime>
#include <csignal>
#include <unistd.h>

TFile *hvdata;
TTree *HVData;
TTree *hvDataTree;

void signalHandler( int signum ) {
  std::cout << "Interrupt signal (" << signum << ") received.\n";
  std::cout << "Saving data to file and closing...." << std::endl;

  // cleanup and close up stuff here
  hvDataTree->Write();
  hvdata->Close();
  // terminate program
  exit(signum);
}

int main(int argc, char *argv[]){

  // register signal SIGINT and signal handler
  signal(SIGINT, signalHandler);


  std::string outputFileName = argv[1];//"HVData.root";
#ifdef VERBOSE
  std::cout << "OutputFileName :  " << outputFileName << std::endl;
  std::cout << "============================================" << std::endl;
#endif
  const char *path = getenv("HVWRAPPER");
  
  std::vector<std::string> psfileVector;
  psfileVector.push_back(Form("%s/conf_files/PowerSupply1.txt",path));
  psfileVector.push_back(Form("%s/conf_files/PowerSupply2.txt",path));

  ReadPowerSupplies psV(psfileVector);
  psV.ReadVoltageAndCurrentOfAllPowerSupplies();

  PowerSupply *psTop = psV.fPsVector[0];
  PowerSupply *psBottom = psV.fPsVector[1];
  //ULong64_t tStamp = time(0);
  UInt_t tStamp = time(0);

  //Create a new ROOT file with desired name
  std::cout << "Starting recording values from Power Supplies..." << std::endl;
  std::cout << "Press Ctrl+c to stop the execution and writing data to file" << std::endl;
#ifdef VERBOSE
  std::cout << "File does not exist hence creating a new file........." << std::endl;
#endif
  //TFile *
  hvdata = new TFile(outputFileName.c_str(),"RECREATE");
  //PowerSupply ps;
  //TTree *
  hvDataTree = new TTree("HVData","HVData");
  hvDataTree->Branch("TimeStamp", &tStamp, "tStamp/i");
  hvDataTree->Branch("HVTop","PowerSupply", &psTop);
  hvDataTree->Branch("HVBottom","PowerSupply", &psBottom);
  while(1){
    tStamp = time(0);
    psV.ReadVoltageAndCurrentOfAllPowerSupplies();
    /*
    // COUTS for debugging
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << "---------Top---------" << std::endl;
    Print(psV.fPsVector[0]);
    std::cout << "---------Bottom---------" << std::endl;
    Print(psV.fPsVector[1]);
    */
    psTop = psV.fPsVector[0];
    psBottom = psV.fPsVector[1];
#ifdef VERBOSE
    std::cout << "Data Read successfully........ Press Ctrl+c to stop the execution" << std::endl;
#endif
    hvDataTree->Fill();
    sleep(20);
  }

  hvDataTree->Fill();
  hvDataTree->Write();
  hvdata->Close();


  return 0;

}

