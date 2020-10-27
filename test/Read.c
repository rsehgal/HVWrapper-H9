/*
 * Read.c
 *
 *  Created on: 26-Oct-2020
 *      Author: rsehgal
 */
#include <iostream>
#include "CAENHVWrapper.h"

int main(){
	char          arg[30], userName[30], passwd[30];
	int           i, c, link;
	int           sysHndl=-1;
	int           sysType=-1;
	CAENHVRESULT  ret;
	ret = CAENHV_InitSystem((CAENHV_SYSTEM_TYPE_t)SY4527, 0, "192.168.0.1", admin, admin, &sysHndl);

	std::cout << "Login Output : " << ret <<std::endl;
	return 0;
}
