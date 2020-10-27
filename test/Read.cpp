/*
 * Read.c
 *
 *  Created on: 26-Oct-2020
 *      Author: rsehgal
 */
#include <iostream>
#include "CAENHVWrapper.h"
/*
#include <signal.h>
#ifdef UNIX
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "MainWrapp.h"
#include "console.h"
*/
#include <vector>
int main(){
	std::vector<int> sysHandleVec;
	char          arg[30]="192.168.0.1", userName[30]="admin", passwd[30]="admin";
	std::vector<char*> sysIpVec;


	int           i, c, link=0;
	int           sysHndl=-1;
	int           sysType=2;
	CAENHVRESULT  ret;
	//CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)SY4527,LINKTYPE_TCPIP , 192.168.0.1, "admin", "admin", &sysHndl);


	sysIpVec.push_back((char*)"169.254.2.1");
	sysIpVec.push_back((char*)"169.254.2.2");
	sysHandleVec.push_back(-1);
	sysHandleVec.push_back(-1);

	bool allLogin = true;


	for(unsigned int i = 0 ; i < sysIpVec.size() ; i++){
		ret = CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)sysType, link, sysIpVec[i], userName, passwd, &sysHandleVec[i]);
		std::cout << "Ret value of login : " << ret << std::endl;
		allLogin = allLogin && (ret == CAENHV_OK);
		if(ret == CAENHV_OK){
			std::cout << "Successful login to : " << sysIpVec[i] << std::endl;
			sysHandleVec.push_back(sysHndl);
		}else{
			std::cout << "Login to : "<< sysIpVec[i] << " failed.... " <<  std::endl;
		}
	}

	if(!allLogin){
		std::cout << "Login to some of the power supplies fails, hence exiting........." << std::endl;
		return 1;
	}


	const char *parName1 = "Vmon";
	const char *parName2 = "Imon";

	int numOfSlots = 8;
	for(unsigned int sysHandlIndex = 0 ; sysHandlIndex < sysHandleVec.size() ; sysHandlIndex++){
		for(unsigned short int slotIndex = 0 ; slotIndex < numOfSlots ; slotIndex++){
			float *parValListVmon = NULL;
			float *parValListImon = NULL;
			unsigned short numOfChannels = 12;
			const unsigned short chList[] = {0,1,2,3,4,5,6,7,8,9,10,11};
			CAENHVRESULT retValVMon =  CAENHV_GetChParam(sysHandleVec[sysHandlIndex], slotIndex ,parName1, numOfChannels, chList, parValListVmon);
			CAENHVRESULT retValIMon =  CAENHV_GetChParam(sysHandleVec[sysHandlIndex], slotIndex ,parName1, numOfChannels, chList, parValListImon);
			for(unsigned int i = 0 ; i < numOfChannels ; i++){
				std::cout << "VMon : " << parValListVmon[i] << " : IMon : " << parValListImon[i] << std::endl;
			}
		}
	}

	/*
	 * Logging out of all the Power supplied
	 */
	for(unsigned int i = 0 ; i < sysHandleVec.size() ; i++){
		std::cout << "Logging out of Power supply with handle  : " << sysHandleVec[i] << std::endl;
		CAENHVRESULT disconnect = CAENHV_DeinitSystem(sysHandleVec[i]);
	}

	//std::cout << "Login Output : " << ret <<std::endl;
	return 0;
}
