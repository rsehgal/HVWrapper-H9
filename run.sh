#!/bin/sh
export CAENHVROOT=`pwd`/CAENHVLIB
currDate=`date +"%d-%m-%Y"`
filename="HVData-$currDate.root"
#echo $currDate
#echo $filename
./build/ReadPSUsingConfFile $filename
