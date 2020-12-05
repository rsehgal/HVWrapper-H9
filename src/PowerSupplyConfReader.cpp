/*
 * PowerSupplyConfReader.cpp
 *
 *  Created on: 27-Oct-2020
 *      Author: rsehgal
 */

#include "PowerSupplyConfReader.h"
#include <fstream>
#include <iostream>
#include <HVSupply.h>
#include <ctime>
using namespace caen;

PowerSupplyConfReader::PowerSupplyConfReader() {
	// TODO Auto-generated constructor stub

}

PowerSupplyConfReader::~PowerSupplyConfReader() {
	// TODO Auto-generated destructor stub
#ifdef VERBOSE
	std::cout << "Deleting power supply object from  PowerSupplyConfReader with IP : " << fIpAddress << std::endl;
#endif
}

PowerSupplyConfReader::PowerSupplyConfReader(std::string filename){
	ReadFile(filename);
}

void PowerSupplyConfReader::Print(){
#ifdef VERBOSE
	std::cout << "IPAddress : " << fIpAddress << std::endl;
	for(unsigned short int i = 0 ; i < fNumOfSlots ; i++){
		std::cout << "Slot Num : " << fSlotVector[i] << " : Num of Channels in Slot : " << fVectOfChannelVector[i].size()
				  << " : Channels : " ;
		for(unsigned short int j = 0 ; j < fVectOfChannelVector[i].size() ; j++){
			std::cout << fVectOfChannelVector[i][j] << " , ";
		}
		std::cout << std::endl;
	}
#endif
}

void PowerSupplyConfReader::ReadFile(std::string filename){
	std::ifstream infile(filename);
		infile >> fIpAddress >> fNumOfSlots;
		for(unsigned int i = 0 ; i < fNumOfSlots ; i++){
			unsigned short slotNo;
			unsigned short numOfChannels;
			std::vector<unsigned short> vecOfChannelNum;
			infile >> slotNo >> numOfChannels;
			fSlotVector.push_back(slotNo);
			for(unsigned short int j = 0 ; j < numOfChannels ; j++){
				unsigned short int channelNum = 2000;
				infile >> channelNum;
				if(channelNum != 2000)
					vecOfChannelNum.push_back(channelNum);
			}
			fVectOfChannelVector.push_back(vecOfChannelNum);
		}

		infile.close();
}

unsigned short int CalculateTotalNumOfChannels(std::vector<PowerSupplyConfReader*> psConfVect ){
	unsigned short int numOfChannels = 0;
	for(unsigned short i = 0 ; i < psConfVect.size() ; i++){
		unsigned short int numOfChannelsInAPs = 0 ;
		for(unsigned short j = 0 ; j < (psConfVect[i]->fSlotVector).size() ; j++){
			numOfChannelsInAPs+= (psConfVect[i]->fVectOfChannelVector)[j].size();
		}
		numOfChannels += numOfChannelsInAPs;
	}

	//std::cout << "TotalNum of Channels : " << numOfChannels << std::endl;
	return numOfChannels;

}

//============ Read Power Supplied class

ReadPowerSupplies::ReadPowerSupplies(std::vector<std::string> filenamesVector){
		for(unsigned short int i = 0 ; i < filenamesVector.size() ; i++){
			fPowerSupplyConfVector.push_back(new PowerSupplyConfReader(filenamesVector[i]));
			fPowerSupplyVector.push_back(new HVSupply(
													  //fPowerSupplyConfVector[i]->fOutputFileName,
													  fPowerSupplyConfVector[i]->fIpAddress,
													  fPowerSupplyConfVector[i]->fSlotVector,
													  fPowerSupplyConfVector[i]->fVectOfChannelVector));
		}
}

ReadPowerSupplies::~ReadPowerSupplies(){
		for(unsigned short int i = 0 ; i < fPowerSupplyVector.size() ; i++){
#ifdef VERBOSE
			std::cout << "Deleting power supply object from ReadPowerSupplies with IP : " << fPowerSupplyConfVector[i]->fIpAddress << std::endl;
#endif
			delete fPowerSupplyVector[i];
		}
		fPowerSupplyVector.clear();
}

void ReadPowerSupplies::ReadVoltageAndCurrentOfAllPowerSupplies(){
	//std::cout << "--------- Reading ReadVoltageAndCurrentOfAll Power Supplies ----------" << std::endl;
	fTStamp = std::time(0);
	fPsVector.clear();
	for(unsigned short int i = 0 ; i < fPowerSupplyVector.size() ; i++){
		fPsVector.push_back(fPowerSupplyVector[i]->ReadVoltageAndCurrentOfAllChannels());
	}
	//std::cout << "Size of fPsVector : " << fPsVector.size() << std::endl;
}
