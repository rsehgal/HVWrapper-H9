/*
 * PowerSupplyConfReader.cpp
 *
 *  Created on: 27-Oct-2020
 *      Author: rsehgal
 */

#include "PowerSupplyConfReader.h"
#include <fstream>
#include <iostream>


PowerSupplyConfReader::PowerSupplyConfReader() {
	// TODO Auto-generated constructor stub

}

PowerSupplyConfReader::~PowerSupplyConfReader() {
	// TODO Auto-generated destructor stub
}

PowerSupplyConfReader::PowerSupplyConfReader(std::string filename){
	ReadFile(filename);
}

void PowerSupplyConfReader::Print(){
	std::cout << "IPAddress : " << fIpAddress << std::endl;
	for(unsigned short int i = 0 ; i < fNumOfSlots ; i++){
		std::cout << "Slot Num : " << fSlotVector[i] << " : Num of Channels in Slot : " << fVectOfChannelVector[i].size()
				  << " : Channels : " ;
		for(unsigned short int j = 0 ; j < fVectOfChannelVector[i].size() ; j++){
			std::cout << fVectOfChannelVector[i][j] << " , ";
		}
		std::cout << std::endl;
	}
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
