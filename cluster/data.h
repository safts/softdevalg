#ifndef DATA_H
#define DATA_H

#include "list.h"
typedef struct dataAct_t Data;


typedef struct _value {
	char* name;
	void* content;
	void* clustExtra;

} value;




typedef struct data_t {

	/**
	 * size of the tablesize 
	 * depends on the lsh theory
	 **/
	unsigned long long int tableSize;

	/**
	 * the radius for the in range problem
	 **/
	double radius;


	/*
	 *
	 * a few fucntion pointers specifying 
	 * the data
	 *
	 **/

	/**
	 * returns the distance between two values
	 **/
	double (*distance)();

	/**
	 * if arg1 - arg2 < r 1
	 * else 0
	 **/
	int (*inRange)();

	/**
	 * initialize the hashfunction  specific 
	 * struct ( basically select a G )
	 **/
	void* (*initStruct)();
	/**
	 * handle the free of the struct
	 **/
	void (*destroyStruct)();
	/**
	 * printing ( for debug)
	 **/
	void (*print)();

	/**
	 * G from theory
	 **/
	unsigned long long int (*G)();

	/**
	 * parse the dataset
	 **/
	void (*readQueries)();
	/**
	 * parse the queries
	 **/
	void (*readData)();
	/**
	 * returns the first value from our
	 * dataset
	 **/
	value* (*getFirst)();
	/**
	 * itirate through the dataset
	 **/
	value* (*getNext)();
	/**
	 * returns the first value from our
	 * queries
	 **/
	value* (*getFirstQuery)();
	/**
	 * itirate through the queries
	 **/
	value* (*getNextQuery)();
	/**
	 * the brute force solution to the
	 * nn problem
	 **/
    double (*bruteForce)();
	/**
	 * the brute force solution to the
	 * in range problem
	 **/
	void (*bruteForceRange)();

	/**
	 * cleanup the internal
	 * structs
	 **/
	void (*destroyInput)();
	/**
	 * cleanup hamming spesific value
	 **/
	void (*destroyValue)();



} dataIF;


void Swap(int ind1,int ind2);

int GetMedoid(int );

int AddCentroid(int );

void GetIthCentroid(int ,value *);

void GetIthData(int, value*);

int AddToClust(int ,int );

int GetClusterOf(int );

void SwapCentroid(int , int );

double ComputeDJ(int, int);

double ComputeAproximateDJ(int, int, int*, int);

void InitAssigned();

double MinDistCendroids();

double MaxDistCendroids();

int AssignLSH(value*, int , int ,int);

void SetDataSize(int size);

int GetDataSize();

void PrintData();

void parseData(FILE* file, int kLSH);

void specifyDataset(FILE* file);

int GetMedoid(int );

void AssignClustExtra(int , int ,double , int ,double );

void InitClustExtra(int);

int GetExtrasCluster(int);

void FirstAssignment();

int GetNoOfCluster();

void SetNoOfCluster(int k);

double Silhouette(FILE*);

double GetDistFirstCentr(int);

int AssignLSH(value*, int, int, int );

int IsAssigned(int);

int GetClustSize(int);

void DestroyData();

void specifyDataset(FILE*);

void PrintClusters(FILE*);

void PrintClustersComplete(FILE*);

void ParseConfig(FILE*, int*);

#endif
