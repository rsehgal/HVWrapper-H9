/*
 * HVDataClass.h
 *
 *  Created on: 29-Oct-2020
 *      Author: rsehgal
 */

#ifndef CAEN_HVWRAPPER_H9_INC_HVDATACLASS_H_
#define CAEN_HVWRAPPER_H9_INC_HVDATACLASS_H_

#include <vector>

class HVDataClass {
public:

        float sV0Set;
        float sVMon;
        float sI0Set;
        float sIMon;

public:
        HVDataClass();
        HVDataClass(float v0set, float vmon, float i0set, float imon);
        virtual ~HVDataClass();

};
using Channel = HVDataClass;
using Slot = std::vector<Channel>;
using PowerSupply = std::vector<Slot>;

#endif /* CAEN_HVWRAPPER_H9_INC_HVDATACLASS_H_ */
