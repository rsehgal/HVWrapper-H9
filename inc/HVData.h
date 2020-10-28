/*
 * HVData.h
 *
 *  Created on: 28-Oct-2020
 *      Author: rsehgal
 */

#ifndef CAEN_HVWRAPPER_H9_INC_HVDATA_H_
#define CAEN_HVWRAPPER_H9_INC_HVDATA_H_

#include<vector>

struct HVData{
	float sV0Set;
	float sVMon;
	float sI0Set;
	float sIMon;

	HVData(){

	}

	HVData(float v0set, float vmon, float i0set, float imon){
			sV0Set = v0set;
			sVMon = vmon;
			sI0Set = i0set;
			sIMon = imon;
	}

	~HVData(){

	}
};

using Channel = HVData;
using Slot = std::vector<Channel>;
using PowerSupply = std::vector<Slot>;


#endif /* CAEN_HVWRAPPER_H9_INC_HVDATA_H_ */
