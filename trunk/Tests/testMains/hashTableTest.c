#include <stdio.h>
#include <assert.h>
#include "hashTable.h"

int hashFun(int *a){
	return *a;
}

void print(int *a){
	if (a == NULL) 
		printf("NULL\n");
	else
		printf("%d\n",*a);
}

int equal(int *a, int *b){
    if(*a==*b)
        return 1;
    else
        return 0;
}


int main(void)
{
	hashTable ht;
	InitHashTable(&ht, 100, print, equal, hashFun);
	int a1 =2;
	int a2 =4;

	InsertHashTable(ht, &a1);

	//print(SearchHashTable(ht, &a1));
	int *r =(int*)SearchHashTable(ht, &a1);
	assert( *r == 2 );

	r = SearchHashTable(ht, &a2);
	assert( r == NULL );

	InsertHashTable(ht, &a2);
	r = SearchHashTable(ht, &a2);
	assert( *r == 4);

	DestroyHashTable(&ht);
	printf("hashTable OK\n");
	
	
	return 0;
}


