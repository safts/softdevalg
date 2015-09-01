#ifndef DATAFUNCTION_H_RA5JZYOT
#define DATAFUNCTION_H_RA5JZYOT

#include "data.h"


#define BUFFSIZE 256
#define LABELSIZE 512


value* GetFirstFunction();

value* GetNextFunction();

value* GetFirstQueryFunction();

value* GetNextQueryFunction();

double DistanceFunction(value*, value*);

int EqualFunction(value*, value*);

int InRangeFunction(value*, value*, double);

void PrintFunction(value *);

void *InitStructFunction( );

void DestroyStructFunction(void ** );

void DestroyInputFunction();

void DestroyValueFunction(void* );

void ReadDataFunction(FILE*, int );

void ReadQueriesFunction(FILE*);

unsigned long long int GFunction(value* , void* );

double BruteForceANNFunction(value , List );

void DestroyInputFunction();

void DestroyValueFunction();


#endif
