/*
 * HVSupply.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: rsehgal
 */

#include "HVSupply.h"
#include <iostream>

namespace caen {

HVSupply::HVSupply(std::string ipaddress, int sysType, int link, std::string username, std::string passwd ) {
	// TODO Auto-generated constructor stub
	//fName = name;
	fSysType = sysType;
	fSysHandle  = -1;
	fLink = link;// LINKTYPE_TCPIP;
	fIPAddress = ipaddress;
	fUsername = username;
	fPasswd = passwd;

	param = new float; //malloc(sizeof(float));
	param2 = new float; //malloc(sizeof(uint));
	id_channel = new ushort; //malloc(sizeof(ushort));

	fVMon = 0.;
	fIMon = 0.;
	fPower = 0;


}

HVSupply::~HVSupply() {
	// TODO Auto-generated destructor stub
	fRet_init = CAENHV_DeinitSystem(fSysHandle);
	if(fRet_init != CAENHV_OK){
		std::cout << "Problem in Logging out of : " << fIPAddress << std::endl;
	}else{
		std::cout << "Successfully Logged out of : " << fIPAddress << std::endl;
	}
}

void HVSupply::Login(){

	//TODO : if already logged in then don't login again,
	//Take care of semaphores

	fRet_init =  CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)fSysType, fLink, const_cast<char*>(fIPAddress.c_str()),
								    const_cast<char*>(fUsername.c_str()), const_cast<char*>(fPasswd.c_str()), &fSysHandle);
	if(fRet_init == CAENHV_OK)
		std::cout<<"Login Successfull..." << std::endl;
	else
		std::cerr << "Login Unsuccessfull. Please check login parameter...." << std::endl;
}

float HVSupply::GetVoltage(int slot, ushort channel){
    param[0]=0;
    //float par[1]={0};
	fRet = CAENHV_GetChParam(fSysHandle, slot, "Vmon", 1, &channel, param);
	if (fRet==CAENHV_OK) {
	   fVMon = param[0];
	}

	return fVMon;
}

float HVSupply::GetCurrent(int slot, ushort channel){
	param[0]=0;
	fRet = CAENHV_GetChParam(fSysHandle, slot, "Imon", 1, &channel, param);
	if (fRet==CAENHV_OK) {
	   fIMon = param[0];
	}

	return fIMon;
}

int HVSupply::GetPowerStatus(int slot, ushort channel){
	param2[0]=0;
	fRet = CAENHV_GetChParam(fSysHandle, slot, "Pw", 1, &channel, param2);
	if (fRet==CAENHV_OK) {
	   fPower = param2[0];
	}

	return fPower;
}

int HVSupply::GetSystemStatus(int slot, ushort channel){
	param2[0]=0;
	fRet = CAENHV_GetChParam(fSysHandle, slot, "Status", 1, &channel, param2);
	if (fRet==CAENHV_OK) {
	   fSystemStatus = param2[0];
	}

	return fSystemStatus;
}


} /* namespace caen */
