#include <stdio.h>
#include <stdlib.h>

#include "LSH.h"
#include "../dataStruct/hashTable.h"
#include "../input/random.h"
#include "../dataStruct/list.h"

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

void QuickSortValue(double *,value** , int );
void QuickSort (double *,int* , int );
void QuickSortDesc (double *,int* , int );


extern dataIF data;

void BuildLSH(LSH** lshptr, int L, int k) {
	int i,j;

	*lshptr = malloc(sizeof(LSH));
	LSH* lsh = *lshptr;
	lsh->L = L;
	lsh->k = k;
	lsh->tables = malloc(L * sizeof(hashMap));
	value value;
	for (i = 0; i < L; i++) {
        
        //initialize struct for each G function depending on the metric
		lsh->tables[i].GConfiguration = data.initStruct();
		
		//initialize hash
		InitHashTable(&(lsh->tables[i].tables), data.tableSize, 
				sizeof(value), data.print, data.distance, data.G,
				NULL);
	
		//insert each value in hash table
		for(j=0; j<GetDataSize(); j++){
			GetIthData(j,&value);
			 
			InsertHashTable(lsh->tables[i].tables, &value,
					lsh->tables[i].GConfiguration);
			//value = GetNextData();
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

int MarkPointsOf(LSH* lsh,int cluster, double range) {
	
	int i;
	int pointsN=0;
	value query;
	GetIthCentroid(cluster,&query);
	for (i = 0; i < lsh->L; i++) {
	
	//gets bucket where the value is
		List bucketList = GetListHashTable(lsh->tables[i].tables, 
				&query, lsh->tables[i].GConfiguration);
		value* lValue = GetFirstBarrier(bucketList);
		//get all neighbours in range an place them in a list
		while (lValue != NULL) {
			double dist = data.distance(lValue, &query);

				// printf("%f\n", dist);
//			printf("%p\n", lValue);
			if ( dist < range) {
				int assigned=AssignLSH(lValue, dist, cluster, range);
//				InsertValueList(neighbours, lValue);
				if( assigned){
					//AddToBarrier(bucketList);
					pointsN++;
				}
			}
			lValue = GetNextBarrier(bucketList);
		}
	}

	return pointsN;
}

unsigned long long int hashFunc(value* value,void* nothing){


	return data.hash(value,nothing);
}



int PNNRange(LSH* lsh, value query, int P,value* array){

	int i=0;
	int hashSize=0;
	hashTable neighbors;
	double* distArr=NULL;

	for (i = 0; i < lsh->L; i++) {

	//gets bucket where the value is
		List bucketList = GetListHashTable(lsh->tables[i].tables,
				&query, lsh->tables[i].GConfiguration);
		hashSize+=SizeList(bucketList);
	}
	InitHashTable(&neighbors,hashSize,sizeof(value),NULL,
			data.distance,hashFunc,NULL);

	for (i = 0; i < lsh->L; i++) {


	//gets bucket where the value is
		List bucketList = GetListHashTable(lsh->tables[i].tables,
				&query, lsh->tables[i].GConfiguration);
		value* lValue = GetFirst(bucketList);
		//get all neighbours in range an place them in a list
		while (lValue != NULL) {
			InsertHashTable(neighbors, lValue,NULL);
			lValue = GetNext(bucketList);
		}
	}
	value** hashArr=malloc( GetHashSize(neighbors) * sizeof(value*));
	// WARNING //
	HashToArr(neighbors,(void**)hashArr);
	distArr=malloc(GetHashSize(neighbors)*sizeof(double));


	for(i=0;i<GetHashSize(neighbors);i++){
		distArr[i]=data.distance(hashArr[i],&query);
	}

	QuickSortValue(distArr, hashArr,  GetHashSize(neighbors));

	for(i=0;(i<P && i<GetHashSize(neighbors)-1 );i++){
		array[i]=*hashArr[i+1];
	}
	DestroyHashTable(&neighbors);
	free(hashArr);
	free(distArr);
	return i;
}

void BruteForce(value query, int* neighbours){

	int i;
	double *distArr = malloc(GetDataSize()*sizeof(double));
	//int *tempArr = malloc(GetDataSize()*sizeof(int));
	
	for(i = 0; i < GetDataSize(); ++i){
		neighbours[i] = i;
		
	}

	value tempVal;
	for(i = 0; i < GetDataSize(); ++i){
		GetIthData(i,&tempVal);
		distArr[i] = data.distance(&query,&tempVal);
	}
	QuickSort(distArr,neighbours,GetDataSize());

	 // for ( i = 0; i < GetDataSize(); ++i)
	 // {
	 // 	printf("%d   %f\n",neighbours[i]+1,distArr[i] );
	 // }
	free(distArr);
	//free(tempArr);
}



void InitBarrierLSH(LSH* lsh){
	int i=0;
	for(i=0;i<lsh->L;i++){
		InitBarrierHash(lsh->tables[i].tables);
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

void QuickSortValue(double *a,value** b, int n) {
    if (n < 2)
        return;
    double p = a[n / 2];
    double *l = a;
    value **ll =b;
    double *r = a + n - 1;
    value **rr = b +n -1;
    while (l <= r) {
        if (*l < p) {
            l++;
            ll++;
        }
        else if (*r > p) {
            r--;
            rr--;
        }
        else {
            double t = *l;
            value* tt= *ll;
            *l = *r;
            *ll = *rr;
            *r = t;
            *rr = tt;
            l++;
            ll++;
            r--;
            rr--;
        }
    }
    QuickSortValue(a,b, r - a + 1);
    QuickSortValue(l,ll, a + n - l);
}

void QuickSort (double *a,int* b, int n) {
    if (n < 2)
        return;
    double p = a[n / 2];
    double *l = a;
    int *ll =b;
    double *r = a + n - 1;
    int *rr = b +n -1;
    while (l <= r) {
        if (*l < p) {
            l++;
            ll++;
        }
        else if (*r > p) {
            r--;
            rr--;
        }
        else {
            double t = *l;
            double tt= *ll;
            *l = *r;
            *ll = *rr;
            *r = t;
            *rr = tt;
            l++;
            ll++;
            r--;
            rr--;
        }
    }
    QuickSort(a,b, r - a + 1);
    QuickSort(l,ll, a + n - l);
}

void QuickSortDesc (double *a,int* b, int n) {
    if (n < 2)
        return;
    double p = a[n / 2];
    double *l = a;
    int *ll =b;
    double *r = a + n - 1;
    int *rr = b +n -1;
    while (l <= r) {
        if (*l > p) {
            l++;
            ll++;
        }
        else if (*r < p) {
            r--;
            rr--;
        }
        else {
            double t = *l;
            double tt= *ll;
            *l = *r;
            *ll = *rr;
            *r = t;
            *rr = tt;
            l++;
            ll++;
            r--;
            rr--;
        }
    }
    QuickSortDesc(a,b, r - a + 1);
    QuickSortDesc(l,ll, a + n - l);
}
