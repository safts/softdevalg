#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "dataMolecule.h"
#include "../lapackeIF.h"


typedef struct Molecule_t {
	int noOfMols;
	int N;
	double** molecules;
} dataMolecule;

dataMolecule dm;

double CRMSDact(double* X, double *Y, int dimx, int dimy){
	double* diff = malloc(sizeof(double) * dimx *dimy);
	int i;
	for (i = 0; i < dimx *dimy; ++i)
		diff[i] = Y[i];
	AddMatr(X, diff, dimx*dimy, -1);
	
	double fn = getFrobNorm( diff, dimx);
	double d1 =  fn/sqrt(dimx);
	free(diff);
	//double d2 =  sqrt(fn)/sqrt(dimx);
	//printf("%f, %f\n", d1, d2);
	return d1;
}

double CRMSD(value* v1,value* v2){
	int mol1 = *(int*)v1->content;
	int mol2 = *(int*)v2->content;
	return CRMSDact(dm.molecules[mol1], dm.molecules[mol2], dm.N, 3);

}

void printMol(double *mol){
	int i,j;
	printf("[");
	for (i = 0; i < dm.N; ++i)
	{
		for (j = 0; j < 2; ++j)
			printf("%f, ", mol[i*3 + j]);
			printf("%f ", mol[i*3 + 2]);
		printf(";");
	}
	printf("]\n");

	printf("****************\n");
}

double optRMSD(value* v1, value* v2){
	int molIndex1 = *(int*)v1->content;
	int molIndex2 = *(int*)v2->content;
	double* mol1 = dm.molecules[molIndex1];
	double* mol2 = dm.molecules[molIndex2];
	int i;
	double Xc[] = {0, 0 ,0};
	for (i = 0; i < dm.N * 3; i+=3)
	{
		Xc[0] += mol1[i + 0];
		Xc[1] += mol1[i + 1];
		Xc[2] += mol1[i + 2];
	}
	for (i = 0; i < 3; ++i)
		Xc[i] /= dm.N;

	double Yc[] = {0, 0 ,0};

	for (i = 0; i < dm.N * 3; i+=3)
	{
		Yc[0] += mol2[i + 0];
		Yc[1] += mol2[i + 1];
		Yc[2] += mol2[i + 2];
	}
	for (i = 0; i < 3; ++i)
		Yc[i] /= dm.N;
//	for (i = 0; i < 3; ++i)
//	printf("%f\n", Xc[i]);
//	for (i = 0; i < 3; ++i)
//	printf("%f\n", Yc[i]);
	double* X = malloc(sizeof(double)* dm.N * 3);
	double* Y = malloc(sizeof(double)* dm.N * 3);
	for (i = 0; i < dm.N *3; ++i)
		X[i] = mol1[i] - Xc[i%3];
	for (i = 0; i < dm.N *3; ++i)
		Y[i] = mol2[i] - Yc[i%3];
	//printMol(X);
	//printMol(Y);
	double *RES = malloc(sizeof(double) * 3 *3);
	MultiMatr(X, Y, RES, dm.N, 3);
	//printMol(RES);
	//it's ok until now
	double *U, *S, *Vt;
	U = malloc(sizeof(double) * 3 *3 );
	Vt = malloc(sizeof(double) * 3 *3 );
	S = malloc(sizeof(double) * 3 );
	CalcSVD(RES, 3, 3, U, S, Vt);
//	printMol(U);
//	printMol(Vt);
//	for (i = 0; i < 3; ++i)
//		printf("%f\n", S[i]);
//	if( ! (S[2] > 0) )
//		printf("oooo s[2] big problem in DRMSD ...\n");
	double *Q = malloc(sizeof(double) * 3*3);
	MultiMatrNoTrans(U, Vt, Q, 3, 3);
	double det = DetMatr(Q, 3, 3);
	//printf("%f\n", det);
	if(det < 0){
		//printMol(U);
		for (i = 0; i < 3; ++i)
			U[3*i + 2] = -U[3*i + 2];
		//printMol(U);
		MultiMatrNoTrans(U, Vt, Q, 3, 3);
		//printMol(Q);
	}
	double *xq = malloc(sizeof(double) * dm.N*3);
	MultiMatrNoTrans(X, Q, xq, dm.N, 3);
	//printMol(xq);
	double dist = CRMSDact(xq, Y, dm.N, 3);





	free(xq);
	free(Q);
	free(U);
	free(Vt);
	free(S);
	free(RES);
	free(X);
	free(Y);
	return dist;
}


void DestroyStructMolecule(void** v){

}

double DRMSD(value* v1, value* v2){

}


int EqualMolecule(value* v1, value* v2){
	int mol1 = *(int*)v1->content;
	int mol2 = *(int*)v2->content;
	int flag = 0;
	int i;
	for (i = 0; i < dm.N *3; ++i)
		flag += dm.molecules[mol1][i] == dm.molecules[mol2][i];
	return !flag;
}

void PrintMolecule(double d){

}

void DestroyInputMolecule(){
	int i;
	for (i = 0; i < dm.noOfMols; ++i)
		free(dm.molecules[i]);
	free(dm.molecules);

}

void DestroyValueMolecule(void* v){

}

void ReadDataMolecule(value** val, FILE* input ){
	size_t lineSize = 0;
	size_t lineLen = 0;

	char* lineBuff = NULL;

	// for strtok
	//char* token=NULL;
	//char delims[4]="\t \n";

	lineLen = getline(&lineBuff, &lineSize, input);
	if (sscanf(lineBuff, "%d",  &dm.noOfMols) < 1) {
		printf("error???? \n");
	}
	lineLen = getline(&lineBuff, &lineSize, input);
	if (sscanf(lineBuff, "%d",  &dm.N) < 1) {
		printf("error???? \n");
	}
	int i, j;
	//printf("%d\n", dm.noOfMols);
	//printf("%d\n", dm.N);
	*val = malloc(sizeof(value) * dm.noOfMols);

	dm.molecules = malloc(sizeof(double*)* dm.noOfMols);
	for (i = 0; i < dm.noOfMols; ++i)
		dm.molecules[i] = malloc(sizeof(double) * dm.N *3);

	int flag = 0;
	for (i = 0; i < dm.noOfMols; ++i)
	{
		for (j = 0; j < dm.N*3; j+=3)
		{
			//dm.molecules[i][j] = malloc(sizeof(double)*3);
			lineLen = getline(&lineBuff, &lineSize, input);
			float x, y, z;
			if (sscanf(lineBuff, "%f %f %f",  &x, &y, &z) < 1) {
				printf("error???? \n");
			}
			/* row major */
			dm.molecules[i][j + 0] = x;
			dm.molecules[i][j + 1] = y;
			dm.molecules[i][j + 2] = z;
			/**/
			/* col major *
			dm.molecules[i][j + 0 + dm.N*0] = x;
			dm.molecules[i][j + 1 + dm.N*1] = y;
			dm.molecules[i][j + 2 + dm.N*2] = z;
			*/
			//if (flag == 0)
				//printf("%g\t%g\t%g\n", x, y, z);
		}
		//double r = getFrobNorm( dm.molecules[i], dm.N);
		//if(flag == 0)
			//printf("%f\n", r);
		int *content = malloc(sizeof(int));
		*content = i;
		(*val)[i].content = content;
		(*val)[i].name = NULL;

		flag = 1;
	}
	SetDataSize(dm.noOfMols);
}


