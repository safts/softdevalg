#ifndef DATACOSINE_H_
#define DATACOSINE_H_

#include "../dataStruct/data.h"
#include "dataHamming.h"


void* InitStructCosine();

void DestroyStructCosine(void** );

void RateItemsCosine(value* , value** , int , List);

double GetIthScoreCosine(int, int );

double GetIthIdCosine(int , int );

void SetIthScoreCosine(int , int ,int );

void DeleteItemCosine(int , int);

int GetTotalItemsCosine();

double InnerProductCosine( int *, int *);

double InnerProductNumber(int *, double *);

double Norm(int *);

double Cosine(value*, value*);

int EqualCosine(value *, value *);

int InRangeCosine(value*, value*, double);

void PrintCosine(double );

bitStr GCosine(value* , void *);

unsigned long long int HashCosine(value*, void*);

void PrintDataCosine(value* );

void PrintDataCosine2(int);

void DestroyInputCosine();

void DestroyValueCosine(void* ); 

void ReadDataCosine(value**, FILE* ,int );


#endif /* DATACosine_H_ */
