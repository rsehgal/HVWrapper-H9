/*
 * ReadHVSupply.cpp
 *
 *  Created on: 27-Oct-2020
 *      Author: rsehgal
 */

#include "HVSupply.h"

using namespace caen;

int main(){
	HVSupply hv("169.254.1.100");
	hv.Login();
	if(hv.IsLoginOk()){
		hv.GetVoltage(1,0);
		hv.GetCurrent(1,0);
	}

	return 0;
}
