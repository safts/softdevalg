#!/bin/sh
for i in {10..50}; 
do 
	nice -19 ./molConfAPP $i 
	cp ./inputFiles/conform.dat ./inputFiles/conform$i.data
done
