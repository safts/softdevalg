#ifndef DATACOSINE_H_
#define DATACOSINE_H_

#include "../dataStruct/data.h"

#define bitStr unsigned long long int

void* InitStructCosine();

void DestroyStructCosine(void** );

void RateItemsCosine(value* , value** , int , List);

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
