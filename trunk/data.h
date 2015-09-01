#ifndef DATA_H
#define DATA_H

#include "list.h"

typedef struct value_t{
	char* name;
	void* content;
	void* extra;
}value;


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


#endif
