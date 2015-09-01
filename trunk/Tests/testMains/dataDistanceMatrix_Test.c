
#include <stdio.h>
#include <stdlib.h>

#include "dataDistanceMatrix.h"
struct dataDistanceMatrix_t {
	int k;
	int index;
	double t1;
	double t2;
	double *T;
} ;

void SetVector(double *x, double seed, int k){
	int i;
	for (i = 0; i < k; ++i)
		x[i] = -(seed*4)/(seed-(i+4)*49.0);

}

int main(void)
{
	int k = 2;
	int vectorCount = 3;
	double **x;
	int i;
	int seed = 100;
	dataDistanceMatrix *ddm;
	ddm = InitStructDistanceMatrix(k);
	x = malloc(sizeof(double*)*vectorCount);
	for (i = 0; i < vectorCount; ++i)
	{
		x[i] = malloc(sizeof(double)*k);
		SetVector(x[i], (double)(2*seed+i*22%43), k);
		PrintDistanceMatrix(x[i], ddm);
	}
	printf("%f\n",HiDistanceMatrixActual(x[0], x[2], x[1]));


	return 0;
}
