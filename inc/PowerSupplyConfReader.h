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

class PowerSupplyConfReader {
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

#endif /* CAEN_HVWRAPPER_H9_INC_POWERSUPPLYCONFREADER_H_ */
