#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "CUnit/Basic.h"

#include "../dataStruct/data.h"
#include "../LSH/LSH.h"

//#include "dataFunction.h"
#include "../input/dataHamming.h"
#include "../input/dataEuclidean.h"
#include "../input/dataDistanceMatrix.h"
#include "../input/random.h"

#include "assignment/pam.h"

#define E 0.0001


void TestInit();
void TestComputeDJ();
void TestAssignLSH();
void TestAssign();
void TestGetC();
void TestAddC();
void TestSwapC();
void TestMedoid();
void TestMinDist();
void TestMaxDist();
void TestPam();
int init_suite4(void);
int init_suite3(void);
int init_suite2(void);
int init_suite1(void);
int clean_suite1(void);


char* lineBuff=NULL;
FILE* inputFile;
FILE* config;
char** argvG;


dataIF data;
int main(int argc, char *argv[]){
	argvG = argv;



	CU_pSuite pSuite1 = NULL;
	CU_pSuite pSuite2 = NULL;
	CU_pSuite pSuite3 = NULL;
	CU_pSuite pSuite4 = NULL;

	/* initialize the CUnit Test registry */
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	/* add a suite to the registry */
	pSuite1= CU_add_suite("Suite_1", init_suite1, clean_suite1);
	pSuite2= CU_add_suite("Suite_2", init_suite2, clean_suite1);
	pSuite3= CU_add_suite("Suite_3", init_suite3, clean_suite1);
	pSuite4= CU_add_suite("Suite_4", init_suite4, clean_suite1);

	if (NULL == pSuite1) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	if (NULL == pSuite2) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	if (NULL == pSuite3) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	if (NULL == pSuite4) {
		CU_cleanup_registry();
		return CU_get_error();
	}

	/* add the Tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(pSuite1, "Test of AddCentroid(int)", TestAddC)) ||
			 (NULL == CU_add_test(pSuite1, "Test of GetIthCentroid(int)", TestGetC)) ||
			(NULL == CU_add_test(pSuite1, "Test of SwapCentroid(int, int)", TestSwapC))||
			(NULL == CU_add_test(pSuite1, "Test of AssignLSH(int, int)", TestAssignLSH))||
			(NULL == CU_add_test(pSuite1, "Test of Assign(int, int)", TestAssign))||
			(NULL == CU_add_test(pSuite1, "Test of ComputeDJ(int, int)", TestComputeDJ))||
			(NULL == CU_add_test(pSuite1, "Test of ComputeDJ(int, int)", TestInit))
	   )
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if ((NULL == CU_add_test(pSuite2, "Test of MinDistCendroids()", TestMinDist)) ||
			 (NULL == CU_add_test(pSuite2, "Test of MaxDistCendroids()", TestMaxDist))
	   )
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if (NULL == CU_add_test(pSuite3, "Test of GetMedoid(int)", TestMedoid))   
	{
		CU_cleanup_registry();
		return CU_get_error();
	}
	if (NULL == CU_add_test(pSuite4, "Test of PamAssign()", TestPam))   
	{
		CU_cleanup_registry();
		return CU_get_error();
	}


	/* Run all Tests using the CUnit Basic interface */
	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();
	CU_cleanup_registry();
	return CU_get_error();

}

typedef unsigned long marker;
marker one = 1;

void comb(int pool, int need, marker chosen, int at)
{
	if (pool < need + at) return; /* not enough bits left */

	if (!need) {
		/* got all we needed; print the thing.  if other actions are
		 *		 * desired, we could have passed in a callback function. */
		for (at = 0; at < pool; at++)
			if (chosen & (one << at)){
				//printf("%d ", at);
			}
		printf("ok...\n");
		return;
	}
	/* if we choose the current item, "or" (|) the bit to mark it so. */
	comb(pool, need - 1, chosen | (one << at), at + 1);
	comb(pool, need, chosen, at + 1);  /* or don't choose it, go to next */
}

void TestInit(){
	int k = 5;
	//comb(GetDataSize(), k, 0, 0);
	

}




void TestComputeDJ(){
	int clust = 1;
	int point = 8;
	double Jarx = PamAssign(5,4);
	//printf("\n");
	double DJ = ComputeDJ(clust ,point);
	SwapCentroid(point, clust);
	double Jtel = PamAssign(5,4);
	//int i=0;
//	for(i=0;i<GetDataSize() - GetNoOfCluster();i++){			//Loop on points, NOT clusters
//		value IthData, IthCentroid;
//		GetIthData(i, &IthData);
//		GetIthCentroid(GetClusterOf(i), &IthCentroid);
//		Jtel+=data.distance(&IthData, &IthCentroid);
//	}

	CU_ASSERT( (Jtel - Jarx)-DJ < E );

}

void TestAddC(){
	value v1;
	GetIthData(1, &v1);
	int index = AddCentroid(1);
	value v2;
	GetIthData(index, &v2);
	CU_ASSERT(0 == data.distance(&v1,&v2));
}

void TestGetC(){
	value v1;
   	GetIthData(1, &v1);
	AddCentroid(1);
	value v2;
	GetIthCentroid(0, &v2);
	CU_ASSERT(0 == data.distance(&v1, &v2));
}

void TestSwapC(){
	value v1;
   	GetIthCentroid(1, &v1);
	value v2; 
	GetIthData(5, &v2);
	SwapCentroid(5,1);
	value v3;
	GetIthCentroid(1, &v3);
	value v4;
    GetIthData(5, &v4);
	CU_ASSERT(0 == data.distance(&v1,&v4));
	CU_ASSERT(0 == data.distance(&v2,&v3));

}

void TestMedoid(){
	int i,medoid;
	// SetNoOfCluster(1);
	KMedoidPlusPlusInit(1);
	FirstAssignment();
	medoid=GetMedoid(0);
	value v;
	GetIthData(medoid,&v);
	CU_ASSERT(medoid == 200);

}

void TestAssignLSH(){
	AddCentroid(1);
	AddCentroid(1);
	AddCentroid(1);
	AddCentroid(1);
	int radius = 1;
	// InitAssigned();
	InitClustExtra(1);
	InitClustExtra(2);
	InitClustExtra(3);
	value v1;
	GetIthData(1,&v1);
	value v2;
	GetIthData(2,&v2);
	value v3;
	GetIthData(3,&v3);
	CU_ASSERT(0==AssignLSH(&v1, 1, 0,0));
	CU_ASSERT(0==AssignLSH(&v2, 3, 1, 0));
	CU_ASSERT(0==AssignLSH(&v3, 5, 2, 0));
	radius *=2;
	CU_ASSERT(1==AssignLSH(&v1, 4, 3, 0));
	CU_ASSERT(1==AssignLSH(&v2, 2, 3, 0));
	CU_ASSERT(1==AssignLSH(&v3, 4, 3, 0));


}


void TestAssign(){
	AddCentroid(5);
	AddCentroid(10);
	AddCentroid(150);
	FirstAssignment();
	int i;
	double dist, th=35.00;
	int loops = GetDataSize()-GetNoOfCluster();
	value v1,v2;
	GetIthCentroid(0,&v2);
	//for (i = 0; i < loops; ++i)
	i = 0;
	while( i < loops ){
		GetIthData(i,&v1);
		dist = data.distance(&v1,&v2);
		//printf("%f\n",dist);
		if(dist < th){
			if(!AddToClust(i, 0)){
				i++;
			}
		}
		else{
			if(!AddToClust(i,1)){
				i++;
			}
		}
	}
	int count = 0;
	for (i = 0; i < loops; ++i)
	{
		GetIthData(i,&v1);
		dist = data.distance(&v1,&v2);
		if(dist < th){
			if(!(GetClusterOf2(i)==0)){
				count++;
				//printf("item  No%d is at %d insted of 0 with d = %f\n",i, GetClusterOf(i), dist );
			}
		}
		else{
			if(!(GetClusterOf2(i)==1)){
				count++;
				//printf("item  No%d is at %d insted of 1 with d = %f\n",i, GetClusterOf(i), dist );
			}
		}
		
		
		//if( cluster[i] == GetClusterOf(i)){
				//count++;
		//}
		//else {
			
		//}
	}
	//printf("No Of C %d\n", count);
	//printf("errors :%d\n", GetDataSize()-GetNoOfCluster() - count);
	CU_ASSERT( 0 == count);
}

void TestMinDist(){
	double dist = MinDistCendroids();
	CU_ASSERT(4 == dist);
}

void TestMaxDist(){
	double dist = MaxDistCendroids();
	CU_ASSERT(400 == dist);
}

void TestPam(){
	int k= GetNoOfCluster();
	int q = 100;
	int flag = 0;
	int i, j;
	int l,oldCentr,newCentr;
	int x, uniSup = q-1;

	double Jarx = PamAssign(5,4);
		int ret = 0;
		for (j = 0; j < q; ++j)
		{
			x = (int)GetUniform(0,uniSup);
			oldCentr = x%k;
			newCentr = x/k;
			double dj = ComputeDJ(oldCentr, newCentr);
			SwapCentroid( newCentr,oldCentr);
			double Jtel = PamAssign(5,4);
			double tmp = Jtel - Jarx - dj;
			ret += (tmp == 0)? 0 : 1;
			Jarx = Jtel;
		}

		CU_ASSERT( ret  == 0);
}


int clean_suite1(void){
	//PrintData();
	//data.readData(inputFile,k);
	free(lineBuff);
	//free(data.Data);
	fclose(inputFile);
	fclose(config);
	//data.destroyValue(data.Data);
	return 0;
}

int init_suite1(void){
	int k=4;
//	char *output = "out.txt", *input = "in.txt", *query = "query.txt";
	//char output[512];
	char input[512];
	int confNums[5];
	//char query[512];


	//FILE* queryFile;
	//FILE* outputFile;

	//size_t lineSize=0;
	//size_t lineLen=0;

	//LSH* lsh;

	if((config=fopen("./inputFiles/cluster_5.conf","r"))==NULL){
		perror("File specified could not be opened(inp)\n");
		exit(-1);
	}

	char secBuff1[256];
	char secBuff2[256];
	//printf("input %s, output %s, query %s, k = %d, L = %d\n",input, output, query, k, L);

	memset(secBuff1,'\0',sizeof(secBuff1));
	memset(secBuff2,'\0',sizeof(secBuff2));

	if((inputFile=fopen("./inputFiles/Ex2_Datasets/DataHamming_5_1000x64.csv","r"))==NULL){
		perror("File specified could not be opened(inp)\n");
		exit(-1);
	}

	specifyDataset(inputFile);
	ParseConfig(config,confNums);
	parseData(inputFile, confNums[0]);
	
	return 0;
}

int init_suite2(void){
	int k=4;

	char secBuff1[256];
	char secBuff2[256];
	int confNums[5];
	//printf("input %s, output %s, query %s, k = %d, L = %d\n",input, output, query, k, L);

	memset(secBuff1,'\0',sizeof(secBuff1));
	memset(secBuff2,'\0',sizeof(secBuff2));

	if((inputFile=fopen("./inputFiles/testInput.csv","r"))==NULL){
		perror("File specified could not be opened(inp)\n");
		exit(-1);
	}
	if((config=fopen("./inputFiles/cluster_5.conf","r"))==NULL){
		perror("File specified could not be opened(inp)\n");
		exit(-1);
	}

	specifyDataset(inputFile);
	ParseConfig(config,confNums);
	parseData(inputFile, confNums[0]);
	AddCentroid(400);
	AddCentroid(399);
	AddCentroid(398);
	AddCentroid(397);
	AddCentroid(396);
	
	return 0;
}

int init_suite3(void){
	int k=4;

	char secBuff1[256];
	char secBuff2[256];
	int confNums[5];
	//printf("input %s, output %s, query %s, k = %d, L = %d\n",input, output, query, k, L);

	memset(secBuff1,'\0',sizeof(secBuff1));
	memset(secBuff2,'\0',sizeof(secBuff2));

	if((inputFile=fopen("./inputFiles/testMedoid.csv","r"))==NULL){
		perror("File specified could not be opened(inp)\n");
		exit(-1);
	}
	if((config=fopen("./inputFiles/cluster_5.conf","r"))==NULL){
		perror("File specified could not be opened(inp)\n");
		exit(-1);
	}

	specifyDataset(inputFile);
	ParseConfig(config,confNums);
	parseData(inputFile, confNums[0]);
	//
	
	return 0;
}

int init_suite4(void){
	int k=4;
//	char *output = "out.txt", *input = "in.txt", *query = "query.txt";
	//char output[512];
	char input[512];
	int confNums[5];
	srand(time(NULL));
	//char query[512];


	//FILE* queryFile;
	//FILE* outputFile;

	//size_t lineSize=0;
	//size_t lineLen=0;

	//LSH* lsh;

	if((config=fopen("./inputFiles/cluster_5.conf","r"))==NULL){
		perror("File specified could not be opened(inp)\n");
		exit(-1);
	}

	char secBuff1[256];
	char secBuff2[256];
	//printf("input %s, output %s, query %s, k = %d, L = %d\n",input, output, query, k, L);

	memset(secBuff1,'\0',sizeof(secBuff1));
	memset(secBuff2,'\0',sizeof(secBuff2));

	if((inputFile=fopen("./inputFiles/Ex2_Datasets/DataHamming_5_1000x64.csv","r"))==NULL){
		perror("File specified could not be opened(inp)\n");
		exit(-1);
	}

	specifyDataset(inputFile);
	ParseConfig(config,confNums);
	parseData(inputFile, confNums[0]);
	KMedoidPlusPlusInit(confNums[0]);
	FirstAssignment();

	
	return 0;
}


