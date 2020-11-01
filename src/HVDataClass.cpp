/*
 * HVDataClass.cpp
 *
 *  Created on: 29-Oct-2020
 *      Author: rsehgal
 */

#include "HVDataClass.h"
#include <iostream>
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

void HVDataClass::Print(){
  std::cout << "V0Set : " << sV0Set << " : VMon : " << sVMon <<" : I0Set : " << sI0Set << " : IMon : " << sIMon << std::endl; 
}

void Print(PowerSupply *ps){
PowerSupply Ps = *ps;

 for(unsigned int i = 0 ; i < Ps.size() ; i++){
	 for(unsigned int j = 0 ; j < Ps[i].size() ; j++){
		 //std::cout << ps[i][j] << " , "
		 Ps[i][j].Print();
	 }
	 //std::cout << std::endl;
 }

}

ClassImp(HVDataClass)
