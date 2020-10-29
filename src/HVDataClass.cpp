/*
 * HVDataClass.cpp
 *
 *  Created on: 29-Oct-2020
 *      Author: rsehgal
 */

#include "HVDataClass.h"

HVDataClass::HVDataClass() {
	// TODO Auto-generated constructor stub

}

HVDataClass::~HVDataClass() {
	// TODO Auto-generated destructor stub
}

HVDataClass::HVDataClass(float v0set, float vmon, float i0set, float imon){
                sV0Set = v0set;
                sVMon = vmon;
                sI0Set = i0set;
                sIMon = imon;
}
