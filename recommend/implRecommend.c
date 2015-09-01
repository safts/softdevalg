#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "./recommend.h"
#include "./input/dataCosine.h"
#include "./input/dataEuclidean.h"
#include "./dataStruct/data.h"
#include "./LSH/LSH.h"

#include "./cluster/initialization/kMedoidPlusPlus.h"
#include "./cluster/assignment/pam.h"
#include "./cluster/update/claransUpdate.h"
#include "./cluster/update/lloyds.h"

extern dataIF data;

#define MAXLOOPS 4

int Exists(int , int* , int );

void SpecifyDatasetRecommend(char* str,int kLSH){

	if (strcmp(str, "cosine") == 0) {
		data.tableSize=kLSH;
		data.distance = Cosine;
		data.inRange = InRangeCosine;
		data.print = PrintDataCosine;
		data.readData = ReadDataCosine;
		data.rateItems = RateItemsCosine;
		data.getScore = GetIthScoreCosine;
		data.getId = GetIthIdCosine;
		data.setScore = SetIthScoreCosine;
		data.deleteItem = DeleteItemCosine;
		data.getTotalItems = GetTotalItemsCosine;
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
		data.rateItems = RateItemsEuclidean;
		data.getScore = GetIthScoreEuclidean;
		data.getId = GetIthIdEuclidean;
		data.setScore = SetIthScoreEuclidean;
		data.deleteItem = DeleteItemEuclidean;
		data.getTotalItems = GetTotalItemsEuclidean;		
		data.G = FEuclidean;
		data.hash = HashEuclidean;
		data.initStruct = InitStructEuclidean;
		data.destroyStruct = DestroyStructEuclidean;
		data.destroyInput = DestroyInputEuclidean;
		data.destroyValue = DestroyValueEuclidean;
	}
	else {
		printf("Unknown metric \"%s\". Exiting.. " ,str);
		exit(-1);
	}
}

void destroyRec(){

}

void NNLSHRecommend(FILE* output, int LLSH, int kLSH, int P,List results, int validate){
	LSH* lsh;
	
	int j,i=0;
	BuildLSH(&lsh,LLSH,kLSH);
	value tempVal;
	printf("Percentage Complete: [");
	fflush(stdout);
	for(i=0;i<GetDataSize(); ++i){
		GetIthData(i,&tempVal);
		value** nnPtr = malloc(P*sizeof(value*));
		value* nn=malloc(P*sizeof(value));

		int res=PNNRange(lsh,tempVal,P,nn);
		for(j = 0; j < res ; j++)
			nnPtr[j] = &(nn[j]);




		data.rateItems(&tempVal,nnPtr,res,results);

		if(!validate){
	
			int size = SizeList(results);
			double* Rres = malloc(size*sizeof(double));
			int* resId = malloc(size*sizeof(int));
	
			Rating* lvalue;
			lvalue = GetFirst(results);
			for ( j = 0; j < size; ++j)
			{
						//while(lvalue != NULL){
				Rres[j] = lvalue->rate;
	
				resId[j] = lvalue->id;
				lvalue = GetNext(results);
				//}
			}
			QuickSortDesc(Rres,resId,size);
			fprintf(output, "User%d: ",i+1);
			for ( j = 0; j < 5; ++j)
			{
				fprintf(output,"%d ",resId[j]);
			}
			fprintf(output,"\n");
			free(Rres);
			free(resId);
			DeleteAllFromList(results);
		}
		free(nn);
		if((i%(GetDataSize()/5))==0){
			printf("=");
			fflush(stdout);

		}
		free(nnPtr);
		
	}
	printf("]\n");

	DestroyLSH(&lsh);
	

}
/*
 * PREPEI NA GINEI KAI TO MEDOID
 */
void loopQueries(FILE* output, int P,List Rresults,int validate){

	int clusters=GetNoOfCluster();

	int i;
	int j;
	int k;
	int l=1;

	for(i=0;i<clusters;i++){

		value** clustArr;
		value tempVal;
		int size=GetDataOfCluster(i,&clustArr);
		for(j=0;j<size;j++){	//for every item of the cluster

			tempVal=*clustArr[j];
			*(clustArr[j])=*(clustArr[size-1]);
			*(clustArr[size-1])=tempVal;

			
			data.rateItems(&tempVal,clustArr,size,Rresults);
			if(validate == 0){	
				//if  not validate
				int Ressize = SizeList(Rresults);
				double* Rres = malloc(Ressize*sizeof(double));
				int * resId = malloc(Ressize*sizeof(int));
				Rating* lvalue;
				lvalue = GetFirst(Rresults);
				for ( k = 0; k < Ressize; ++k)
				{
							//while(lvalue != NULL){
					Rres[k] = lvalue->rate;
					resId[k] = lvalue->id;
					lvalue = GetNext(Rresults);
					//}
				}
				QuickSortDesc(Rres,resId,Ressize);
				fprintf(output,"User%d: ",l++);
				for ( k = 0; k < 5; ++k)
				{
					fprintf(output,"%d ",resId[k]);
				}
				DeleteAllFromList(Rresults);
				fprintf(output,"\n");
			}
		}

		free(clustArr);
		//printf("---------------------_Cluster: %d_---------------------\n",i);
	}
	
}

int ClustRecommend(FILE* output,FILE* cluster,int* confNums,int P, int clust, List Rresults, int validate){

	void (*init)() = KMedoidPlusPlusInit;
	double (*assigment)() = PamAssign;
	int (*update)() = ClaransUpdate;
	
	double info[2];

	int k=0;
	int kCluster=clust;
	int N=GetDataSize();

	double silhouette=0;




	// N/K > P
	if(validate == 0){
		for(kCluster=N/100;P< (N/ (double)kCluster);kCluster+=N/200){

			double info[2];
			info[0] = 0; 

			printf("k: %d\n",kCluster);

			init(kCluster);

			FirstAssignment();

			assigment(confNums[1],confNums[2]);

			printf("Update loop #%d\n",k++);
			while(update(assigment,confNums)){
				printf("Update loop #%d\n",k);
				k++;
			}
			k=0;

			PrintClusters(cluster);

			silhouette=Silhouette(output);
			printf("Silhouette: %f\n",silhouette);

			DestroyClusters();

			if(info[0] < silhouette){
				info[0]=silhouette;
				info[1]=kCluster;
			}

		}
	}
	if(validate == 0){
		kCluster = info[1];
		printf("Optimal Number of clusters is: %d\n", kCluster);
		
	}
	init(kCluster);

	FirstAssignment();

	assigment(confNums[1],confNums[2]);

	printf("Update loop #%d\n",k++);
	while(update(assigment,confNums)){
		printf("Update loop #%d\n",k);
		k++;
	}
	k = 0;


	loopQueries(output,P, Rresults, validate);

}


void KFoldValidateLSH( FILE* output,int LLSH,int kLSH,int P, int validate){

	int i=0,j,J,size;
	
	double error,mae=0;
	List Rresults;
	InitList(&Rresults,sizeof(Rating),NULL,data.distance,destroyRec);
	double* Rres;
	int* resId;
	Rating* rating = malloc(GetDataSize()*sizeof(Rating));
	for(i=0;i<10;i++){
		printf("Validation loop #%d\n",i+1);
		error = 0;
		GetScoreMatrix(i,rating);
		DeleteItems(i);
		
		NNLSHRecommend(output, LLSH, kLSH,P, Rresults, validate);
		J=data.getTotalItems()/10;
		size = SizeList(Rresults);
		Rres = malloc(size*sizeof(double));
		resId = malloc(size*sizeof(int));
		
		Rating* lvalue;
		lvalue = GetFirst(Rresults);
		for ( j = 0; j < size; ++j)
		{
					//while(lvalue != NULL){
			Rres[j] = lvalue->rate;

			resId[j] = lvalue->id;
			lvalue = GetNext(Rresults);
			//}
		}
		int pos;
		for(j=0; j<GetDataSize(); ++j){
			pos = Exists(rating[j].id,resId,size);
			if(pos != -1){
				error += fabs(rating[j].rate-Rres[pos]);
			}
			else
				error += fabs(rating[j].rate);

		}
		mae+=error/J;

		SetScores(i,rating);
		DeleteAllFromList(Rresults);
	}
	free(rating);


	fprintf(output,"\nNN LSH Recommendation MAE: %f\n\n",mae/i );

	free(Rres);
	free(resId);
	DestroyList(&Rresults);

}

void KFoldValidateCluster(FILE* output,FILE* cluster,int* confNums, int P, int clust, List Rresults, int validate){

	int i=0,j,J,size;
	
	double error,mae=0;
//	List Rresults;
//	InitList(&Rresults,sizeof(Rating),NULL,data.distance,data.destroyValue);
	double* Rres;
	int* resId;
	Rating* rating = malloc(GetDataSize()*sizeof(Rating));
	for(i=0;i<10;i++){
		printf("Validation loop #%d\n",i+1);
		error = 0;
		GetScoreMatrix(i,rating);
		DeleteItems(i);
		ClustRecommend(output,cluster,confNums,P,clust,Rresults, validate);
		J=data.getTotalItems()/10;
		size = SizeList(Rresults);
		Rres = malloc(size*sizeof(double));
		resId = malloc(size*sizeof(int));

		Rating* lvalue;
		lvalue = GetFirst(Rresults);
		for ( j = 0; j < size; ++j)
		{
					//while(lvalue != NULL){
			Rres[j] = lvalue->rate;

			resId[j] = lvalue->id;
			lvalue = GetNext(Rresults);
			//}
		}
		int pos;
		for(j=0; j<GetDataSize(); ++j){
			pos = Exists(rating[j].id,resId,size);

			if(pos != -1){
				error += fabs(rating[j].rate-Rres[pos]);
			}
			else
				error += fabs(rating[j].rate);

		}
		if(J==0)
			printf("XAOS!");
		mae+=error/J;

		SetScores(i,rating);
		DeleteAllFromList(Rresults);
	}
	DestroyList(&Rresults);
	QuickSortDesc(Rres,resId,size);


	fprintf(output,"\nClustering Recommendation MAE: %f\n\n",mae/i );


	free(rating);	
	free(Rres);
	free(resId);
	

}


int Exists(int id, int* IDs, int size){
	int i;
	for(i=0; i<size; ++i){
		if(IDs[i] == id)
			return i;
	}
	return -1;

}
