// See: https://root.cern.ch/selecting-dictionary-entries-linkdefh
#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

#pragma link C++ class HVDataClass+;
#pragma link C++ class std::vector<HVDataClass>+;
#pragma link C++ class std::vector<std::vector<HVDataClass>>+;
#pragma link C++ class Channel+;
#pragma link C++ class Slot+;
#pragma link C++ class PowerSupply+;
#endif
