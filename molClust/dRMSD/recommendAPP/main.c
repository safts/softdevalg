#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataStruct/data.h"
#include "dataStruct/list.h"
#include "recommend.h"
#include "dataStruct/data.h"


dataIF data;

int main(int argc, char *argv[])
{

	FILE* input=NULL;
	FILE* output=NULL;
	FILE* config=NULL;
	input=fopen("./inputFiles/yahoo_music_small.dat","r");
	output=fopen("./inputFiles/bla.dat","w");
	config=fopen("./inputFiles/cluster_5.conf","r");

	int LLSH=5;
	int kLSH=4;

	SpecifyDatasetRecommend("cosine",kLSH);

	//NN-recommend

//	parseData(input, kLSH);

//	NNLSHRecommend(output,LLSH,kLSH);


	//Cluster-recommend

	ClustRecommend(input,config,output,10);
//
//	PrintClusters(output);
////
//	Silhouette(output);

	fclose(input);
	fclose(output);
	return 0;

}
