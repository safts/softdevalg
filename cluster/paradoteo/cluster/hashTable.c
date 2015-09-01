#include <stdio.h>
#include <stdlib.h>
#include "hashTable.h"
#include "list.h"


struct hashTable_t {
	int M;
	List* table;
	unsigned long long int (*hashFun)();
};

typedef struct hashTable_t hashTablePrivate;


void InitHashTable(hashTablePrivate **htptr, int M,
		int valueSize, void (*print)(), double (*equal)(), 
		unsigned long long int (*hashFun)(), void (*destroyValue)()){

	*htptr = malloc(sizeof(hashTablePrivate));
	hashTablePrivate *ht = *htptr;
	ht->table = malloc(sizeof(List)*M);
	int i;
	for (i = 0; i < M; ++i){
        
		InitList(&ht->table[i], valueSize, print, equal, destroyValue);
	}
	ht->hashFun = hashFun;
	ht->M = M;
}

void InsertHashTable(hashTablePrivate *ht, void *value, void *arg){

	int pos = ht->hashFun(value, arg);
    if(SearchList(ht->table[abs(pos%ht->M)],value))
        return;
	InsertValueList(ht->table[abs(pos%ht->M)], value);
}

void* SearchHashTable(hashTablePrivate *ht, void *value){

	int bucket = (ht->hashFun(value))%ht->M;
	return SearchList(ht->table[bucket], value);

}

List GetListHashTable(hashTablePrivate* hashTable,void* value, void *arg){

	int bucket = (hashTable->hashFun(value, arg))%hashTable->M;
	return hashTable->table[abs(bucket)];

}

void DestroyHashTable(hashTablePrivate **htptr){
	int i;
	hashTablePrivate *ht = *htptr;
	for (i = 0; i < ht->M; ++i)
		DestroyList(&(ht->table[i]));
	free(ht->table);
	free(*htptr);
}

void InitBarrierHash(hashTablePrivate* ht){

	int i=0;
	for(i=0;i<ht->M;i++){
		InitBarrier(ht->table[i]);
	}
}

