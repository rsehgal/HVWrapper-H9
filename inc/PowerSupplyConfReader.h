/*
 * PowerSupplyConfReader.h
 *
 *  Created on: 27-Oct-2020
 *      Author: rsehgal
 */

#ifndef CAEN_HVWRAPPER_H9_INC_POWERSUPPLYCONFREADER_H_
#define CAEN_HVWRAPPER_H9_INC_POWERSUPPLYCONFREADER_H_
#include <vector>
#include <string>
#include <iostream>
#include "HVSupply.h"
#include "HVDataClass.h"

using namespace caen;

class PowerSupplyConfReader {
public:
	//std::string fOutputFileName;
	std::string fIpAddress;
	unsigned short fNumOfSlots;
	std::vector<unsigned short> fSlotVector;
	std::vector<std::vector<unsigned short>> fVectOfChannelVector;

public:
	PowerSupplyConfReader(std::string filename);
	PowerSupplyConfReader();
	virtual ~PowerSupplyConfReader();
	void Print();
	void ReadFile(std::string filename);
};


class ReadPowerSupplies{
public:
	std::vector<PowerSupply> fPsVector;
	unsigned long int fTStamp;

public:
	std::vector<PowerSupplyConfReader*> fPowerSupplyConfVector;
	std::vector<HVSupply*> fPowerSupplyVector;

	ReadPowerSupplies(std::vector<std::string> filenamesVector);
	~ReadPowerSupplies();
	void ReadVoltageAndCurrentOfAllPowerSupplies();


};

#endif /* CAEN_HVWRAPPER_H9_INC_POWERSUPPLYCONFREADER_H_ */
