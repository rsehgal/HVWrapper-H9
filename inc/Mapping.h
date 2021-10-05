/*
**	Filename : Mapping.h
**	2021-10-01
**	username : rsehgal
*/
#ifndef Mapping_h
#define Mapping_h
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
extern std::vector<std::string> vecOfPsBars;
extern std::vector<unsigned short> vecOfSlot;
extern std::vector<std::vector<unsigned short>> vecOfChannelPairs;
extern std::vector<unsigned short> vecOfPowerSupply;
struct PsMap{
        unsigned short sPsId;
        unsigned short sPsSlot;
        std::vector<unsigned short> sChannelPair;

        PsMap(){}
        PsMap(unsigned short psId, unsigned short psSlot, std::vector<unsigned short> channelPair){
                sPsId=psId;
                sPsSlot=psSlot;
                sChannelPair=channelPair;
        }

        unsigned short GetPsId() const{return sPsId;}
        unsigned short GetSlotId() const {return sPsSlot;}
        std::vector<unsigned short> GetChannelPair() const{return sChannelPair;}
        void Print(){
                std::cout << "PowerSupply ID : " << sPsId << " : Slot No. : " << sPsSlot << " : Channel Pair : (" << sChannelPair[0] <<" , "<<sChannelPair[1] << ")" << std::endl;
        }

};

/* Function to return the index of an element from std::vector */
template <typename T>
unsigned int GetIndex(std::vector<T> v, T K)
{
  auto it = find(v.begin(), v.end(), K);
  // If element was found
  if (it != v.end()) {
    // calculating the index of K
    int index = it - v.begin();
    // cout << index << endl;
    return index;
  } else {
    // If the element is not present in the vector
    // cout << "-1" << endl;
    return -1;
  }
}

extern PsMap* GetMapOf(unsigned int barId);
extern PsMap* GetMapOf(std::string barName);

extern std::vector<PsMap*> Bar;
/*This function will fill the PsMap vector*/
extern void FillHVMapping();
#endif
