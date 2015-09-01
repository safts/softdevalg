#ifndef DATAEUCLIDEAN_H_
#define DATAEUCLIDEAN_H_

#include "../dataStruct/data.h"
#include "dataHamming.h"


void* InitStructEuclidean();

void DestroyStructEuclidean(void** );

value* GetFirstEuclidean();

value* GetNextEuclidean(); 

value* GetFirstQueryEuclidean();

value* GetNextQueryEuclidean();

bitStr InnerProductFmod( unsigned  int *, unsigned int *);

double InnerProductEuclidean( int *, int *);

double Euclidean(value*, value*);

int EqualEuclidean(value *, value *);

int InRangeEuclidean(value*, value*, double);

void PrintEuclidean(double );

void ReadDataEuclidean(value**, FILE* , int );

void ReadQueriesEuclidean(FILE* );

void RateItemsEuclidean(value* , value** , int , List);

double GetIthScoreEuclidean(int, int );

double GetIthIdEuclidean(int , int );

void SetIthScoreEuclidean(int , int ,int );

void DeleteItemEuclidean(int , int);

int GetTotalItemsEuclidean();

bitStr FEuclidean(value* , void *);

unsigned long long int HashEuclidean(value* );

void PrintDataEuclidean(value* );

void PrintDataEuclidean2(value* );

double BruteForceANNEuclidean(value , List ); 

void DestroyInputEuclidean();

void DestroyValueEuclidean(void* ); 

void RateItemsEuclidean(value* , value** , int , List );


#endif /* DATAEUCLIDEAN_H_ */
