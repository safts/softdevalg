#!/bin/sh
j=128
for i in {1..9}; 
do 
	time ./molConfAPP $j
	cp ./inputFiles/conform.dat ./inputFiles/conform$i.data
	j=`echo "$j*2" | bc `
done
