#ifndef DATAHAMMING_H_ORPDYCXU
#define DATAHAMMING_H_ORPDYCXU

#include "../dataStruct/data.h"
#include "../dataStruct/list.h"


typedef unsigned long long int bitStr;

/**
 * initialize the hashfunction  specific 
 * struct ( basically select a G )
 **/
void* InitStructHamming();

/**
 * handle the free of the struct
 **/
void DestroyStructHamming( void** );

/**
 * returns the first value from our
 * dataset
 **/
value* GetFirstHamming();

/**
 * itirate through the dataset
 **/
value* GetNextHamming();

/**
 * returns the first value from our
 * queries
 **/
value* GetFirstQueryHamming();

/**
 * itirate through the queries
 **/
value* GetNextQueryHamming();

/**
 * check if the two values are equal
 **/
double EqualHamming(value*, value*);

/**
 * returns the hamming distance
 **/
double Hamming(value *, value* );

/**
 * checks if the hamming distance is less
 * than the given value ( arg3)
 **/
int InRangeHamming(value*, value*, double);

/**
 * print for debugging
 **/
void PrintDataHamming(value* );

/**
 * the G function from lsh theory
 **/
bitStr GHamming(value* , void* );

/**
 * returns a random matrix 
 * drawn iid uniform
 **/
void FillRandomMatrix(int ,int*);

/**
 * parse the dataset
 **/
void ReadDataHamming(value**, FILE* ,int );

/**
 * parse the queries
 **/
void ReadQueriesHamming(FILE* );

/**
 * brute force evaluation for 
 * nearest neighbor
 **/
double BruteForceANNHamming( value , List );

/**
 * brute force evaluation for 
 * points in given range 
 **/
void BruteForceInRangeHamming( value , double , List );

/**
 * cleanup the internal
 * structs
 **/
void DestroyInputHamming();

/**
 * cleanup hamming spesific value
 **/
void DestroyValueHamming(void *);

#endif /* end of include guard: DATAHAMMING_H_ORPDYCXU */


