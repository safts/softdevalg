#include <stdio.h>
#include <stdlib.h>
#include "../../dataStruct/data.h"
#include "../../input/random.h"

/**
 *  
 *
 *
 *
 **/

extern dataIF data;

void GetRandomWithDist(int ,double*, double*);

void KMedoidPlusPlusInit(int k){

	int i = 0;
	int size;
	double *D = malloc(sizeof(double)*GetDataSize());
	double *prob = NULL;
	int c = (int)GetUniform(0,GetDataSize());
	c = AddCentroid(c);

	int j;
	for (j = 0; j < GetDataSize(); ++j){
		value vj, vc;
		GetIthData(j, &vj);
		GetIthData(c, &vc);
		D[j] = data.distance(&vj,&vc);
	}
	double max = D[0];
	for(j=1; j< GetDataSize(); j++)
		if (D[j] > max) 
			max = D[j];
	for(j=0; j< GetDataSize(); j++)
		D[j]/=max;

	i++;
	
	while(i < k){
		size = GetDataSize() - i;
		prob = malloc(sizeof(double)* size);
		GetRandomWithDist(size, D, prob);
		double r = GetUniform(0,prob[size-1]);
		//printf("------_+++===%f , r =%f\n", prob[size-1], r);
		for(j = 0;j < size; j++){
			if(j == size-1){
				c = j;
				continue;
			}
			if(prob[j]<r && r <= prob[j+1])
				c = j;
		}
		
		c = AddCentroid(c);
		double tempD=0.0;
//		#pragma omp parallel for
		for (j = 0; j < GetDataSize() - i; ++j){
			value vj, vc;
			GetIthData(j, &vj);
			GetIthData(c, &vc);
			double dist = data.distance(&vj,&vc);
			if (D[j] > dist)
				D[j] = dist;
		}

		i++;
		free(prob);
	}
	free(D);
}


void GetRandomWithDist(int size, double* D, double *prob){
	
	int j;
	prob[0] = D[0]*D[0];
	for(j=1; j< size; j++){
		prob[j] = prob[j-1] + D[j]*D[j];
	}
}




