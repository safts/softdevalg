#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./implMolConf.h"
#include "./input/dataMolecule.h"
#include "./input/dataCosine.h"

extern dataIF data;

void SpecifyDatasetMolConf(char* str){

	if (strcmp(str, "cosine") == 0) {
		data.distance = Cosine;
		data.inRange = InRangeCosine;
		data.print = PrintDataCosine;
		data.readData = ReadDataCosine;
		data.rateItems= RateItemsCosine;
		data.G = GCosine;
		data.hash = HashCosine;
		data.initStruct = InitStructCosine;
		data.destroyStruct = DestroyStructCosine;
		data.destroyInput = DestroyInputCosine;
		data.destroyValue = DestroyValueCosine;
	}
	else if(strcmp(str,"molecule") == 0){

		data.distance = DRMSD;
//		data.print = PrintDataEuclidean2;
		data.readData = ReadDataMolecule;
		data.destroyInput = DestroyInputMolecule;
		data.destroyValue = DestroyValueMolecule;
	}
	else {
		printf("Unknown metric \"%s\". Exiting.. " ,str);
		exit(-1);
	}

}
