#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./recommend.h"
#include "../input/dataCosine.h"
#include "../input/dataEuclidean.h"
#include "../input/dataMolecule.h"
#include "./dataStruct/list.h"
#include "./dataStruct/data.h"
#include "./LSH/LSH.h"

#include "./cluster/initialization/kMedoidPlusPlus.h"
#include "./cluster/assignment/pam.h"
#include "./cluster/update/claransUpdate.h"
#include "./cluster/update/lloyds.h"

extern dataIF data;

#define MAXLOOPS 4

void SpecifyDatasetRecommend(char* str,int kLSH){

	if (strcmp(str, "cosine") == 0) {
		data.tableSize=kLSH;
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
	else if (strcmp(str, "euclidean") == 0) {

		data.distance = Euclidean;
		data.inRange = InRangeEuclidean;
		data.print = PrintDataEuclidean2;
		data.readData = ReadDataEuclidean;
		data.G = FEuclidean;
		data.hash = HashEuclidean;
		data.initStruct = InitStructEuclidean;
		data.destroyStruct = DestroyStructEuclidean;
		data.destroyInput = DestroyInputEuclidean;
		data.destroyValue = DestroyValueEuclidean;
	}
	else if(strcmp(str,"molecule") == 0){

	}
	else {
		printf("Unknown metric \"%s\". Exiting.. " ,str);
		exit(-1);
	}

}

void NNLSHRecommend(FILE* fp,int LLSH,int kLSH){
	LSH* lsh;
	List Rresults;
	InitList(&Rresults,sizeof(Rating),NULL,NULL,NULL);
	int i;
	BuildLSH(&lsh,LLSH,kLSH);
	printf("Meta to build!\n");
	value tempVal;
	GetIthData(0,&tempVal);
	value** nn=malloc(10*sizeof(value*));
	int* resId;
	double* Rres;
	int* neighbours = malloc(GetDataSize()*sizeof(int));
	int res=PNNRange(lsh,tempVal,10,nn);
	BruteForce(tempVal, neighbours);
	
	value temp;
	// for(i=0;i<res;i++){
	
	// 	data.print(nn[i]);
	// 	printf("\t");
	// 	GetIthData(neighbours[i],&temp);
	// 	data.print(&temp);
	// 	double dist = data.distance(&tempVal, nn[i]);
	// 	printf("\tDist:%f", dist);
	// 	double dist2 = data.distance(&tempVal, &temp);
	// 	printf("\tReal Dist:%f\n", dist2);
	// 	//printf("\n");
	// }
	data.rateItems(&tempVal,nn,res,Rresults);
	int size = SizeList(Rresults);
	Rres = malloc(size*sizeof(double));
	resId = malloc(size*sizeof(int));
	Rating* lvalue;
	lvalue = GetFirst(Rresults);
	for ( i = 0; i < size; ++i)
	{
				//while(lvalue != NULL){
		Rres[i] = lvalue->rate;
		resId[i] = lvalue->id;
		lvalue = GetNext(Rresults);
		//}
	}
	QuickSortDesc(Rres,resId,size);
	for ( i = 0; i < 5; ++i)
	{
		printf("%d %f \n",resId[i],Rres[i]);
	}
}

void loopQueries(int P){

	int clusters=GetNoOfCluster();

	int i;
	int j;
	int k;
	for(i=0;i<clusters;i++){
		printf("---------------------_Cluster: %d_---------------------\n",i);
		value** clustArr;
		int size=GetDataOfCluster(i,&clustArr);
		for(j=0;j<size;j++){	//for every item of the cluster
			printf("--------USER--------\n");
			List Rresult;
			InitList(&Rresult,sizeof(Rating),NULL,data.distance,NULL);

			value tempVal=*clustArr[j];
			*(clustArr[j])=*(clustArr[size-1]);
			*(clustArr[size-1])=tempVal;

			data.rateItems(&tempVal,clustArr,size-1,Rresult);

			int Ressize = SizeList(Rresult);
			double* Rres = malloc(Ressize*sizeof(double));
			int * resId = malloc(Ressize*sizeof(int));
			Rating* lvalue;
			lvalue = GetFirst(Rresult);
			for ( k = 0; k < Ressize; ++k)
			{
						//while(lvalue != NULL){
				Rres[k] = lvalue->rate;
				resId[k] = lvalue->id;
				lvalue = GetNext(Rresult);
				//}
			}
			QuickSortDesc(Rres,resId,Ressize);
			for ( k = 0; k < 5; ++k)
			{
				printf("Item: %d, Rating: %f \n",resId[k],Rres[k]);
			}
			DeleteAllFromList(Rresult);
			printf("--------USER--------\n");
		}
		printf("---------------------_Cluster: %d_---------------------\n",i);
	}
}

void ClustRecommend(FILE* input, FILE* config,FILE* output,int P){

	void (*init)() = KMedoidPlusPlusInit;
	double (*assigment)() = PamAssign;
	int (*update)() = LloydsUpdate;

	int confNums[5];

	ParseConfig(config,confNums);
	parseData(input,confNums[1]);

	if(confNums[3] == 0){
		if(0.012*(confNums[0]*(GetDataSize()-confNums[0])) > 250)
			confNums[3] = 0.012*(confNums[0]*(GetDataSize()-confNums[0]));
		else
			confNums[3] = 250;
	}

	int kCluster=0;
	int prevkCluster=0;
	int N=GetDataSize();
	int loops=0;

	double rate=0;
	double prevRate=0;
	double silhouette=0;
	double prevSilhouette=0;

	kCluster=N/(4*P);
	for(loops=0;loops<MAXLOOPS;loops++){
		int j=0;

		printf("k: %d\n",kCluster);
		fprintf(output,"k: %d\n",kCluster);

//		if(loops>0)
//			parseData(input,confNums[1]);

		init(kCluster);

		FirstAssignment();

		assigment(confNums[1],confNums[2]);

		while(update(assigment,confNums)){
			fprintf(output,"Upd	ate loop #%d\n",j);
			printf("Update loop #%d\n",j);
			j++;
		}
		PrintClusters(output);
		silhouette=Silhouette(output);
		printf("Silhouette: %f\n",silhouette);
//		DestroyData();
		DestroyClusters();
		rewind(input);
		if(loops > 0){
			rate=10000*(silhouette - prevSilhouette)/ (double)(kCluster-prevkCluster);
			printf("Silhouette increase rate: %f\n",rate);
			if(rate < prevRate ){
				if(silhouette > 0.0){
					printf("Rate has decreased. Stopping at optimal #k=%d\n",kCluster);
					//break;
				}
				else{
					printf("Rate has decreased, trying some more "
							"loops to achieve a better validation\n");
				}
			}
		}
		prevRate=rate;
		prevkCluster=kCluster;
		prevSilhouette=silhouette;
		kCluster+=N/(4*P);
	}
	if(silhouette < 0.0){
		printf("Clustering probably not good enough\n");
	}

//	loopQueries(P);

}


void KFoldValidate(){

	int sampleSize=(GetDataSize() - GetNoOfCluster())/10;
	int i=0;

	for(i=0;i<10;i++){
//		int foldMin=

	}

}
