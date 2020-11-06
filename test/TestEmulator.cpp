#include "HVSupply.h"
#include <iostream>
//using namespace caen;
int main(){
	float param[1]={0};
	ushort channel=2;
	std::cout << "Voltage Mon : " << CAENHV_GetChParam(1,2 ,
 	"VMon", 2, &channel, param) << std::endl;

 	return 0;
}