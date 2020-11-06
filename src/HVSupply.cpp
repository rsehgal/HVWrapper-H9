/*
 * HVSupply.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: rsehgal
 */

#include "HVSupply.h"
#include <iostream>
#include <fstream>
#include "HVDataClass.h"
#include <string>
//#include <TTree.h>
//#include <TH1F.h>
//#include <TFile.h>

/*CAENHVRESULT  CAENHV_GetChParam(int handle, ushort slot, 
 	const char *ParName, ushort ChNum, const ushort *ChList, float *ParValList){
 		float val;
		if(std::string(ParName).compare(0,1,"V")==0){
			val = (rand() % (12300 - 11000 + 1)) + 11000; 
		}
		if(std::string(ParName).compare(0,1,"I")==0){
			val = (rand() % (200 - 100 + 1)) + 100;	
		}

		ParValList[0]=val;
		return CAENHV_OK;
 	}
*/
namespace caen {

namespace emulator{
	CAENHVRESULT  CAENHV_GetChParam(int handle, ushort slot, 
 	const char *ParName, ushort ChNum, const ushort *ChList, float *ParValList){
 		float val;
		if(std::string(ParName).compare(0,1,"V")==0){
			val = (rand() % (12300 - 11000 + 1)) + 11000; 
		}
		if(std::string(ParName).compare(0,1,"I")==0){
			val = (rand() % (200 - 100 + 1)) + 100;	
		}

		ParValList[0]=val;
		return CAENHV_OK;
 	}
}


HVSupply::HVSupply(){

}

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

	Login();

}

HVSupply::HVSupply(std::string ipaddress, std::vector<unsigned short> slotVector, std::vector<std::vector<unsigned short>> vectOfChannelVector,
			 int sysType, int link, std::string username, std::string passwd ){

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

	fSlotVector = slotVector;
	fVectOfChannelVector = vectOfChannelVector;
#ifdef VERBOSE
	Debug();
#endif
	Login();

}

void HVSupply::Debug(){
	std::cout << "================= Power Supply : " << fIPAddress << "================" << std::endl;
	for(unsigned short int i = 0 ; i < fSlotVector.size() ; i++){
		std::cout << "Slot No. :  " << fSlotVector[i] << " : Channel Nos. : ";
		for(unsigned short int j=0 ; j < fVectOfChannelVector[i].size() ; j++){
			std::cout <<  fVectOfChannelVector[i][j] << " , ";
		}
		std::cout << std::endl;
	}
}

HVSupply::HVSupply(std::string outputfileName,std::string ipaddress, std::vector<unsigned short> slotVector, std::vector<std::vector<unsigned short>> vectOfChannelVector,
			 int sysType, int link, std::string username, std::string passwd ){

	fOutputFileName = outputfileName;
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

	fSlotVector = slotVector;
	fVectOfChannelVector = vectOfChannelVector;
	Login();

}


#if(0)
void HVSupply::ReadVoltageAndCurrentOfAllChannels(){

	std::ofstream outfile(fIPAddress+".txt",std::ios::app);
	unsigned long int timestamp = 0;
	outfile << timestamp << ",";
	if(IsLoginOk()){
		for(unsigned short int slotIndex = 0 ; slotIndex < fSlotVector.size(); slotIndex++){
			for(unsigned short int channelIndex = 0 ; channelIndex < fVectOfChannelVector.size() ; channelIndex++){
				outfile << GetVoltage(slotIndex,channelIndex) << "," << GetCurrent(slotIndex,channelIndex) << ",";
			}
		}
	}
	else{
		outfile << "Login Problem for Power Suppy : " << fIPAddress;
	}
	outfile << std::endl;
	outfile.close();
}
#endif

PowerSupply* HVSupply::ReadVoltageAndCurrentOfAllChannels(){

#ifdef VERBOSE
	std::ofstream outfile(fIPAddress+".txt",std::ios::app);
#endif
	unsigned long int timestamp = 0;
	//outfile << timestamp << ",";
	if(IsLoginOk()){
		PowerSupply *ps = new PowerSupply;
		for(unsigned short int slotIndex = 0 ; slotIndex < fSlotVector.size(); slotIndex++){
			Slot sl;
			for(unsigned short int channelIndex = 0 ; channelIndex < fVectOfChannelVector[slotIndex].size() ; channelIndex++){
				Channel ch(fSlotVector[slotIndex],fVectOfChannelVector[slotIndex][channelIndex],
						   GetV0Set(fSlotVector[slotIndex],fVectOfChannelVector[slotIndex][channelIndex]),
						   GetVMon(fSlotVector[slotIndex],fVectOfChannelVector[slotIndex][channelIndex]),
						   GetI0Set(fSlotVector[slotIndex],fVectOfChannelVector[slotIndex][channelIndex]),
						   GetIMon(fSlotVector[slotIndex],fVectOfChannelVector[slotIndex][channelIndex]));
#ifdef VERBOSE
				outfile << GetVoltage(fSlotVector[slotIndex],fVectOfChannelVector[slotIndex][channelIndex]) << ","
						<< GetCurrent(fSlotVector[slotIndex],fVectOfChannelVector[slotIndex][channelIndex]) << ",";
#endif
				sl.push_back(ch);
			}
			ps->push_back(sl);
		}
		return ps;

	}
	else{
#ifdef VERBOSE
		outfile << "Login Problem for Power Suppy : " << fIPAddress;
#endif
	}

#ifdef VERBOSE
	outfile << std::endl;
	outfile.close();
#endif
}

HVSupply::~HVSupply() {
	// TODO Auto-generated destructor stub
#ifdef EMULATOR_MODE
	fRet_init = CAENHV_OK;
#else
	fRet_init = CAENHV_DeinitSystem(fSysHandle);
#endif

	if(fRet_init != CAENHV_OK){
#ifdef VERBOSE
		std::cout << "Problem in Logging out of : " << fIPAddress << std::endl;
#endif
	}else{
#ifdef VERBOSE
		std::cout << "Successfully Logged out of : " << fIPAddress << std::endl;
#endif
	}
}

void HVSupply::Login(){

#ifdef EMULATOR_MODE
	fRet_init = CAENHV_OK;
#else

	//TODO : if already logged in then don't login again,
	//Take care of semaphores

	fRet_init =  CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)fSysType, fLink, const_cast<char*>(fIPAddress.c_str()),
								    const_cast<char*>(fUsername.c_str()), const_cast<char*>(fPasswd.c_str()), &fSysHandle);
	if(fRet_init == CAENHV_OK){
#ifdef VERBOSE
		std::cout<<"Login Successfull..." << std::endl;
#endif
	}
	else{
#ifdef VERBOSE
		std::cerr << "Login Unsuccessfull. Please check login parameter...." << std::endl;
#endif
	}

#endif
}

float HVSupply::GetVMon(int slot, ushort channel){
    param[0]=0;
    //float par[1]={0};
#ifdef EMULATOR_MODE
    fRet = emulator::CAENHV_GetChParam(fSysHandle, slot, "VMon", 1, &channel, param);
#else
	fRet = CAENHV_GetChParam(fSysHandle, slot, "VMon", 1, &channel, param);
#endif
	if (fRet==CAENHV_OK) {
	   fVMon = param[0];
	}

	return fVMon;
}

float HVSupply::GetIMon(int slot, ushort channel){
	param[0]=0;
#ifdef EMULATOR_MODE
	fRet = emulator::CAENHV_GetChParam(fSysHandle, slot, "IMon", 1, &channel, param);
#else
	fRet = CAENHV_GetChParam(fSysHandle, slot, "IMon", 1, &channel, param);
#endif
	if (fRet==CAENHV_OK) {
	   fIMon = param[0];
	}
	return fIMon;
}

float HVSupply::GetV0Set(int slot, ushort channel){
    param[0]=0;
#ifdef EMULATOR_MODE
    fRet = emulator::CAENHV_GetChParam(fSysHandle, slot, "V0Set", 1, &channel, param);
#else
    //float par[1]={0};
	fRet = CAENHV_GetChParam(fSysHandle, slot, "V0Set", 1, &channel, param);
#endif
	if (fRet==CAENHV_OK) {
	   fVMon = param[0];
	}
	return fVMon;
}

float HVSupply::GetI0Set(int slot, ushort channel){
	param[0]=0;
#ifdef EMULATOR_MODE
	fRet = emulator::CAENHV_GetChParam(fSysHandle, slot, "I0Set", 1, &channel, param);
#else
	fRet = CAENHV_GetChParam(fSysHandle, slot, "I0Set", 1, &channel, param);
#endif

	if (fRet==CAENHV_OK) {
	   fIMon = param[0];
	}
	return fIMon;
}

float HVSupply::GetVoltage(int slot, ushort channel){
    param[0]=0;
#ifdef EMULATOR_MODE
    fRet = emulator::CAENHV_GetChParam(fSysHandle, slot, "VMon", 1, &channel, param);
#else
    //float par[1]={0};
	fRet = CAENHV_GetChParam(fSysHandle, slot, "VMon", 1, &channel, param);
#endif

	if (fRet==CAENHV_OK) {
	   fVMon = param[0];
	}
	return fVMon;
}

float HVSupply::GetCurrent(int slot, ushort channel){
	param[0]=0;
#ifdef EMULATOR_MODE
	fRet = emulator::CAENHV_GetChParam(fSysHandle, slot, "IMon", 1, &channel, param);
#else
	fRet = CAENHV_GetChParam(fSysHandle, slot, "IMon", 1, &channel, param);
#endif

	if (fRet==CAENHV_OK) {
	   fIMon = param[0];
	}
	return fIMon;
}

int HVSupply::GetPowerStatus(int slot, ushort channel){
	param2[0]=0;
#ifdef EMULATOR_MODE
	fRet = emulator::CAENHV_GetChParam(fSysHandle, slot, "Pw", 1, &channel, param2);
#else
	fRet = CAENHV_GetChParam(fSysHandle, slot, "Pw", 1, &channel, param2);
#endif

	if (fRet==CAENHV_OK) {
	   fPower = param2[0];
	}
	return fPower;
}

int HVSupply::GetSystemStatus(int slot, ushort channel){
	param2[0]=0;
#ifdef EMULATOR_MODE
	fRet = emulator::CAENHV_GetChParam(fSysHandle, slot, "Status", 1, &channel, param2);
#else
	fRet = CAENHV_GetChParam(fSysHandle, slot, "Status", 1, &channel, param2);
#endif

	if (fRet==CAENHV_OK) {
	   fSystemStatus = param2[0];
	}
	return fSystemStatus;
}


} /* namespace caen */
