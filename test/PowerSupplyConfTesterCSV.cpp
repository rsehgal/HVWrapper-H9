/*
 * PowerSupplyConfTester.cpp
 *
 *  Created on: 27-Oct-2020
 *      Author: rsehgal
 */

#include "PowerSupplyConfReader.h"
#include <vector>

int main(){
        /*
	PowerSupplyConfReader ps("PowerSupply1.txt");
	ps.Print();
	ps.ReadFile("PowerSupply2.txt");
	ps.Print();*/

	std::vector<std::string> psfileVector;
	psfileVector.push_back("PowerSupply1.txt");
	psfileVector.push_back("PowerSupply2.txt");

	ReadPowerSupplies psV(psfileVector);
	unsigned int psNum = 0;
	for(unsigned short int i = 0 ; i < psV.fPowerSupplyConfVector.size() ; i++){
		std::cout << "============================================" << std::endl;
		psNum++;
		PowerSupplyConfReader *psConf = psV.fPowerSupplyConfVector[i];
		for(unsigned int slotIndex = 0 ; slotIndex < psConf->fNumOfSlots ; slotIndex++){
			std::cout << "PowerSuppy : " << psNum << " : SlotNum : " << psConf->fSlotVector[slotIndex] << std::endl;
			std::cout << " -------------- :  Num Of Channels : " << psConf->fVectOfChannelVector[slotIndex].size() << std::endl;
			for(unsigned short int j = 0 ; j < psConf->fVectOfChannelVector[slotIndex].size() ; j++){
				std::cout << psConf->fVectOfChannelVector[slotIndex][j] << " , ";
			}
			std::cout << std::endl;
		}
		psV.fPowerSupplyConfVector[i]->Print();
	}

	std::cout << "=====================================" << std::endl;
	std::cout << "Total Number of Channel : " << CalculateTotalNumOfChannels(psV.fPowerSupplyConfVector) << std::endl;
	std::cout << "=====================================" << std::endl;

	/*std::cout << "============================================" << std::endl;
	std::vector<std::string> psfileVector;
	psfileVector.push_back("PowerSupply1.txt");
	psfileVector.push_back("PowerSupply2.txt");

	ReadPowerSupplies psV(psfileVector);
*/
	return 0;

}
