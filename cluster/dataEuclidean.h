#ifndef DATAEUCLIDEAN_H_
#define DATAEUCLIDEAN_H_

#include "data.h"
#include "dataHamming.h"


void* InitStructEuclidean();

void DestroyStructEuclidean(void** );

value* GetFirstEuclidean();

value* GetNextEuclidean(); 

value* GetFirstQueryEuclidean();

value* GetNextQueryEuclidean();

bitStr InnerProductFmod( unsigned  int *, unsigned int *);

double InnerProduct( double *, double *);

double Euclidean(value*, value*);

int EqualEuclidean(value *, value *);

int InRangeEuclidean(value*, value*, double);

void PrintEuclidean(double );

void ReadDataEuclidean(value**, FILE* , int );

void ReadQueriesEuclidean(FILE* );

bitStr FEuclidean(value* , void *);

void PrintDataEuclidean(value* );

double BruteForceANNEuclidean(value , List ); 

void DestroyInputEuclidean();

void DestroyValueEuclidean(void* ); 


#endif /* DATAEUCLIDEAN_H_ */
