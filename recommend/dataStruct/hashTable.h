#ifndef HASHTABLE_H
#define HASHTABLE_H value

#include "list.h"

typedef struct hashTable_t* hashTable;

/*
 * Initializes hash table, given its size and the size of members
 * It passes fuctions that are members of hash table
 */ 
void InitHashTable(hashTable *, int ,int , void (*)(),
	   	double (*)(), unsigned long long int (*)(), 
		void (*destroyValue)());

/*
 * Searches the hash table for the value
 * Returns the value if it will be found, NULL otherwise
 */
void* SearchHashTable(hashTable , void *value);

/*
 * Inserts a value in hash table given an argument 
 * for the hash function
 */
void InsertHashTable(hashTable , void *, void *);

/*
 * Returns the list where the value is
 */
List GetListHashTable(hashTable ,void* ,void *);

void HashToArr(hashTable,void** );
void InitBarrierHash(hashTable);
int GetHashSize(hashTable);

/*
 * Destroys hash table
 */
void DestroyHashTable(hashTable *);

#endif
