/*
**	Filename : TestMapping.cpp
**	2021-10-05
**	username : rsehgal
*/
#include "Mapping.h"
#include <iostream>
int main(int argc, char *argv[]) {

  FillHVMapping();

  for (unsigned int i = 0; i < Bar.size(); i++) {
    Bar[i]->Print();
  }

  std::cout << "--------------------------------------" << std::endl;
  std::cout << Bar[0]->GetPsId() << " : " << Bar[0]->GetSlotId() << std::endl;
  std::cout << "--------------------------------------" << std::endl;
  PsMap *mp = GetMapOf(9);
  mp->Print();
  std::cout << "--------------------------------------" << std::endl;
  PsMap *mps = GetMapOf("PS85_SF881");
  mps->Print();
	
return 0;
}
