/*
 * PowerSupplyConfTester.cpp
 *
 *  Created on: 27-Oct-2020
 *      Author: rsehgal
 */

#include "PowerSupplyConfReader.h"
int main(){
	PowerSupplyConfReader ps("PowerSupply1.txt");
	ps.Print();
	ps.ReadFile("PowerSupply2.txt");
	ps.Print();

	return 0;

}
