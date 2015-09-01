#include <stdio.h>
#include <stdlib.h>

#include "LSH.h"
#include "hashTable.h"
#include "random.h"
#include "list.h"

/* struct for each G function */
typedef struct hashMap_t {
	hashTable tables;
	void* GConfiguration;
} hashMap;

/* struct for LSH */ 
struct LSH_t {
	hashMap* tables;
	int L;
	int k;
};

extern dataIF data;

void BuildLSH(LSH** lshptr, int L, int k) {
	int i;

	*lshptr = malloc(sizeof(LSH));
	LSH* lsh = *lshptr;
	lsh->L = L;
	lsh->k = k;
	lsh->tables = malloc(L * sizeof(hashMap));
	for (i = 0; i < L; i++) {
        
        //initialize struct for each G function depending on the metric
		lsh->tables[i].GConfiguration = data.initStruct();
		
		//initialize hash
		InitHashTable(&(lsh->tables[i].tables), data.tableSize, 
				sizeof(value), data.print, data.distance, data.G,
				NULL);
	
	// insert each value in hash table
		value* value = data.getFirst();
		while (value != NULL) {

			InsertHashTable(lsh->tables[i].tables, value,
					lsh->tables[i].GConfiguration);
			value = data.getNext();
		}
	}
	
}

double SearchANN(LSH* lsh, value query, value** result) {
	int i;
	int retrieved = 0;
	double cur_distance = 0.0;
	double distance = 10000.0;
	value* lValueN = NULL;
	
	for (i = 0; i < lsh->L; i++) {
		//gets bucket where the value is
		List bucketList = GetListHashTable(lsh->tables[i].tables, &query,
				lsh->tables[i].GConfiguration);
		value* lValue = GetFirst(bucketList);
		
		// algorithm from theory
		while (lValue != NULL) {
			retrieved++;
			if (retrieved > 5 * lsh->L) {
                *result = lValueN;
				return distance;
			}
			cur_distance = data.distance(lValue, &query);

			if (cur_distance < distance) {
				lValueN = lValue;
				distance = cur_distance;
			}
			lValue = GetNext(bucketList);
		}
	}
    *result = lValueN;
	return distance;
}

void SearchRange(LSH* lsh, value query, double range, List neighbours) {
	
	int i;
	for (i = 0; i < lsh->L; i++) {
	
	//gets bucket where the value is
		List bucketList = GetListHashTable(lsh->tables[i].tables, 
				&query, lsh->tables[i].GConfiguration);
		value* lValue = GetFirst(bucketList);
		//get all neighbours in range an place them in a list
		while (lValue != NULL) {
			if (data.inRange(lValue, &query, range)) {
				InsertValueList(neighbours, lValue);
			}
			lValue = GetNext(bucketList);
		}
	}
}

void DestroyLSH(LSH** lshptr) {
    int i;
    LSH *lsh = *lshptr;
    for(i=0; i<lsh->L; i++){
        data.destroyStruct(&(lsh->tables[i].GConfiguration));
		DestroyHashTable(&(lsh->tables[i].tables));
    }
    free(lsh->tables);
    free(*lshptr);
}
