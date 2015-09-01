#ifndef DATADISTANCEMATRIX_H_RA5JZYOT
#define DATADISTANCEMATRIX_H_RA5JZYOT

#include "../dataStruct/data.h"
//typedef char * item;

//testing

#define BUFFSIZE 256
#define LABELSIZE 512

typedef int item;


//typedef struct confDistanceMatrix_t  confDistanceMatrix;

value* GetFirstDistanceMatrix();

value* GetNextDistanceMatrix();

value* GetFirstQueryDistanceMatrix();

value* GetNextQueryDistanceMatrix();

double DistanceMatrixDistance(value*, value*);

int EqualDistanceMatrix(value*, value*);

int InRangeDistanceMatrix(value*, value*, double);

void PrintDistanceMatrix(value*);

void printEverything(value*);

void *InitStructDistanceMatrix( );

void DestroyStructDistanceMatrix(void ** );

void DestroyInputDistanceMatrix();

void DestroyValueDistanceMatrix(void* );

void ReadDataDistanceMatrix(value**, FILE*, int );

void ReadQueriesDistanceMatrix(FILE*);

unsigned long long int GDistanceMatrix(value* , void* );

double BruteForceANNDistanceMatrix(value , List );

void DestroyInputDistanceMatrix();

void DestroyValueDistanceMatrix();

#endif /* end of include guard: DATADISTANCEMATRIX_H_RA5JZYOT */
