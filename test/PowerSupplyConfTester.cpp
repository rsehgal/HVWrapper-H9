/*
 * PowerSupplyConfTester.cpp
 *
 *  Created on: 27-Oct-2020
 *      Author: rsehgal
 */

#include "PowerSupplyConfReader.h"
#include <vector>

int main(){
	PowerSupplyConfReader ps("PowerSupply1.txt");
	ps.Print();
	ps.ReadFile("PowerSupply2.txt");
	ps.Print();

	/*std::cout << "============================================" << std::endl;
	std::vector<std::string> psfileVector;
	psfileVector.push_back("PowerSupply1.txt");
	psfileVector.push_back("PowerSupply2.txt");

	ReadPowerSupplies psV(psfileVector);
*/
	return 0;

}
