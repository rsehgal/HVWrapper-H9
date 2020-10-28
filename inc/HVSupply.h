/*
 * HVSupply.h
 *
 *  Created on: Feb 15, 2017
 *      Author: rsehgal
 */

#ifndef CAEN_INC_HVSUPPLY_H_
#define CAEN_INC_HVSUPPLY_H_
#include <string>

#include "CAENHVWrapper.h"
#include <vector>
namespace caen {

class HVSupply {
	std::string fName;
	int fSysType;
	int fSysHandle;

	int fNumOfSlots;
	int fNumOfChannelPerSlot;
	CAENHVRESULT  fRet_init;
	CAENHVRESULT fRet;
	int fLink;
	std::string fIPAddress;
	std::string fUsername;
	std::string fPasswd;
	float fVMon;
	float fIMon;

	//TODO: DATA TYPES of These parameter must be carefully checked

	//Parameter for reading
	float *param;// = NULL; // used for reading VMON and IMON
	float *param2;// = NULL; //used for reading power status
	ushort *id_channel;// = NULL;
	std::string fParName;
	int fPower;
	int fSystemStatus;

	std::vector<unsigned short> fSlotVector;
	std::vector<std::vector<unsigned short>> fVectOfChannelVector;

public:
	//HVSupply(std::string name, std::string ipaddress, int link, std::string username="admin", std::string passwd="admin");
	HVSupply(std::string ipaddress, int sysType=2, int link=0, std::string username="admin", std::string passwd="admin" );
	HVSupply(std::string ipaddress, std::vector<unsigned short> slotVector, std::vector<std::vector<unsigned short>> vectOfChannelVector,
			 int sysType=2, int link=0, std::string username="admin", std::string passwd="admin" );
	virtual ~HVSupply();
	void Login();
	bool IsLoginOk(){return (fRet_init == CAENHV_OK);}

	//Getters functions
	int GetSysType()const {return fSysType;}
	std::string GetIPAddress() {return fIPAddress;}
	std::string GetName() {return fName;}
	int GetNumOfSlots() {return fNumOfSlots;}
	int GetNumOfChannelPerSlot() {return fNumOfChannelPerSlot;}
	float GetVoltage(int slot, ushort channel);//{return fVMon;}
    float GetCurrent(int slot, ushort channel);//{return fIMon;}
	int GetPowerStatus(int slot, ushort channel);//{return fPower;}
	int GetSystemStatus(int slot, ushort channel);//{return fSystemStatus;}

	//Setters functions
	void SetV0(){}
	void SetPowerOn(){}
	void SetPowerOff(){}
	void SetI0(){}
	void SetTrip(){}
	void HVPrint(){}
	void HVScan(){}
	void SetV0Single(){}
	void HVPrintSingle(){}
	void SetTripSingle(){}
	void SetI0Single(){}
	void HVMon_Full(){}
	void HVScanGap(){}
	void SetI0Gap(){}
	void SetTripGap(){}
	void SetV0Gap(){}
	void HVMonFullGap(){}
	void ChannelTest(){}
	void HVMonFullStabGap(){}

	void ReadVoltageAndCurrentOfAllChannels();


};

} /* namespace caen */

#endif /* CAEN_INC_HVSUPPLY_H_ */
