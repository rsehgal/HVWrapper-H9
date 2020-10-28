/*
 * ReadPSUsingConfFile.cpp
 *
 *  Created on: 28-Oct-2020
 *      Author: rsehgal
 */

#include "PowerSupplyConfReader.h"
int main(){
	std::cout << "============================================" << std::endl;
	std::vector<std::string> psfileVector;
	psfileVector.push_back("PowerSupply1.txt");
	psfileVector.push_back("PowerSupply2.txt");

	ReadPowerSupplies psV(psfileVector);
	psV.ReadVoltageAndCurrentOfAllPowerSupplies();

	return 0;

}

