/*
 * HVSupply.cpp
 *
 *  Created on: Feb 15, 2017
 *      Author: rsehgal
 */

#include "HVSupply.h"
#include <iostream>
#include <fstream>
#include "HVData.h"
//#include <TTree.h>
//#include <TH1F.h>
//#include <TFile.h>

namespace caen {

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
	Login();

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

PowerSupply HVSupply::ReadVoltageAndCurrentOfAllChannels(){


	//fOutputFileName = "simulatedData.root";
	/*std::ifstream f(fOutputFileName);
	if(f.good()){
		f.close();
		 Open the required ROOT file in append mode
		 TFile *hvdata = new TFile(fOutputFileName.c_str(),"UPDATE");
		 fHVDataTree = (TTree*)hvdata.Get("HVDataTree");

		 fHVDataTree->Fill();
		 fHVDataTree->Write();
		 hvdata.Close();

		//std::cout << "FILE ExIST>..........." << std::endl;
	}else{
		f.close();
		 Create a new ROOT file with desired name
		 TFile *hvdata = new TFile(fOutputFileName.c_str(),"CREATE");
		 PowerSupply ps;
		 fHVDataTree = new TTree("HVDataTree","HVDataTree");
		 fHVDataTree->Branch("HVTop","PowerSupply", &ps);
		//std::cout << "FILE does NOT ExiST......... " << std::endl;
	}*/


	std::ofstream outfile(fIPAddress+".txt",std::ios::app);
	unsigned long int timestamp = 0;
	outfile << timestamp << ",";
	if(IsLoginOk()){
		PowerSupply ps;
		for(unsigned short int slotIndex = 0 ; slotIndex < fSlotVector.size(); slotIndex++){
			Slot sl;
			for(unsigned short int channelIndex = 0 ; channelIndex < fVectOfChannelVector.size() ; channelIndex++){
				Channel ch(GetV0Set(slotIndex,channelIndex),GetVMon(slotIndex,channelIndex),
						   GetI0Set(slotIndex,channelIndex),GetIMon(slotIndex,channelIndex));

				//outfile << GetVoltage(slotIndex,channelIndex) << "," << GetCurrent(slotIndex,channelIndex) << ",";
				sl.push_back(ch);
			}
			ps.push_back(sl);
		}
		return ps;


			/*std::ifstream f(fOutputFileName);
			if(f.good()){
				f.close();
				 Open the required ROOT file in append mode
				 TFile *hvdata = new TFile(fOutputFileName.c_str(),"UPDATE");
				 fHVDataTree = (TTree*)hvdata->Get(fIPAddress.c_str());

				 fHVDataTree->Fill();
				 fHVDataTree->Write();
				 hvdata->Close();

				//std::cout << "FILE ExIST>..........." << std::endl;
			}else{
				f.close();
				 Create a new ROOT file with desired name
				 TFile *hvdata = new TFile(fOutputFileName.c_str(),"CREATE");
				 //PowerSupply ps;
				 fHVDataTree = new TTree(fIPAddress.c_str(),fIPAddress.c_str());
				 fHVDataTree->Branch("HVTop","PowerSupply", &ps);
				 fHVDataTree->Fill();
				 fHVDataTree->Write();
				 hvdata->Close();

				//std::cout << "FILE does NOT ExiST......... " << std::endl;
			}*/

	}
	else{
		outfile << "Login Problem for Power Suppy : " << fIPAddress;
	}
	outfile << std::endl;
	outfile.close();
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

float HVSupply::GetVMon(int slot, ushort channel){
    param[0]=0;
    //float par[1]={0};
	fRet = CAENHV_GetChParam(fSysHandle, slot, "VMon", 1, &channel, param);
	if (fRet==CAENHV_OK) {
	   fVMon = param[0];
	}

	return fVMon;
}

float HVSupply::GetIMon(int slot, ushort channel){
	param[0]=0;
	fRet = CAENHV_GetChParam(fSysHandle, slot, "IMon", 1, &channel, param);
	if (fRet==CAENHV_OK) {
	   fIMon = param[0];
	}

	return fIMon;
}

float HVSupply::GetV0Set(int slot, ushort channel){
    param[0]=0;
    //float par[1]={0};
	fRet = CAENHV_GetChParam(fSysHandle, slot, "V0Set", 1, &channel, param);
	if (fRet==CAENHV_OK) {
	   fVMon = param[0];
	}

	return fVMon;
}

float HVSupply::GetI0Set(int slot, ushort channel){
	param[0]=0;
	fRet = CAENHV_GetChParam(fSysHandle, slot, "I0Set", 1, &channel, param);
	if (fRet==CAENHV_OK) {
	   fIMon = param[0];
	}

	return fIMon;
}

float HVSupply::GetVoltage(int slot, ushort channel){
    param[0]=0;
    //float par[1]={0};
	fRet = CAENHV_GetChParam(fSysHandle, slot, "VMon", 1, &channel, param);
	if (fRet==CAENHV_OK) {
	   fVMon = param[0];
	}

	return fVMon;
}

float HVSupply::GetCurrent(int slot, ushort channel){
	param[0]=0;
	fRet = CAENHV_GetChParam(fSysHandle, slot, "IMon", 1, &channel, param);
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
