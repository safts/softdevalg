#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "CUnit/Basic.h"

#include "./dataStruct/data.h"
//#include "./LSH.h"


#include "./cluster/assignment/pam.h"
#include "./input/random.h"
#include "lapackeIF.h"

#define E 0.0001


void TestAdd();
void TestMult();

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


	if (NULL == pSuite1) {
		CU_cleanup_registry();
		return CU_get_error();
	}
	

	/* add the Tests to the suite */
	/* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
	if ((NULL == CU_add_test(pSuite1, "Test of AddMatr()", TestAdd)) ||
		(NULL == CU_add_test(pSuite1, "Test of TestMult()", TestMult)))
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

void TestAdd(){
	int i,j;
	double x[5*3] = { 1, 2, 3,
				      4, 5, 6,
				      7, 8, 9,
				      10,11,12,
				      13,14,15};
	double y[5*3] = { 1, 2, 3,
				      4, 5, 6,
				      7, 8, 9,
				      10,11,12,
				      13,14,15};
	double c[3*3];
	AddMatr(x, y , 5*3 ,-1 );
	
	for ( i = 0; i < 5; ++i)
	{
		for ( j = 0; j < 3; ++j)
		{
			CU_ASSERT((int)y[i*5+j] == 0);
		}
	}

}

void TestMult(){
	int i,j;
	double x[3*5] = { 1, 1, 1, 1, 1,
				      1, 1, 1, 1, 1,
				     1, 1, 1, 1, 1
				 	};

	double y[5*3] = { 1, 1, 1,
				      1, 1, 1,
				      1, 1, 1,
				      1, 1, 1,
				      1, 1, 1};
	double c[3*3];
	MultiMatr(x, y , c,5,3);
	
	for ( i = 0; i < 3; ++i)
	{
		for ( j = 0; j < 3; ++j)
		{
			CU_ASSERT((int)c[i*3+j] == 5);
		}
	}

}

int clean_suite1(void){
	//PrintData();

	return 0;
}

int init_suite1(void){


	
	return 0;
}