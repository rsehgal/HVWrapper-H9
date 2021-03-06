/*
 * HVDataClass.h
 *
 *  Created on: 29-Oct-2020
 *      Author: rsehgal
 */


#ifndef CAEN_HVWRAPPER_H9_INC_HVDATACLASS_H_
#define CAEN_HVWRAPPER_H9_INC_HVDATACLASS_H_

#pragma once

#include <TROOT.h>
#include <vector>

class HVDataClass {
public:

		unsigned short sSlotNo;
		unsigned short sChannelNo;
        float sV0Set;
        float sVMon;
        float sI0Set;
        float sIMon;

public:
        HVDataClass();
        HVDataClass(unsigned short slotNo, unsigned short channelNo, float v0set, float vmon, float i0set, float imon);
        virtual ~HVDataClass();
	void Print();
        ClassDef(HVDataClass,1)


};
using Channel = HVDataClass;
using Slot = std::vector<Channel>;
using PowerSupply = std::vector<Slot>;

extern void Print(PowerSupply *ps);
#endif /* CAEN_HVWRAPPER_H9_INC_HVDATACLASS_H_ */
