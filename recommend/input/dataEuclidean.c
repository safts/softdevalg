#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dataEuclidean.h"
#include "../dataStruct/data.h"
#include "../dataStruct/hashTable.h"
#include "../dataStruct/list.h"
#include "random.h"

extern dataIF data;

//typedef unsigned long long int bitStr;


typedef struct _item {
	int id;
	int score;
} item;

typedef struct _user {
	int id;
	int NoOfItems;
	item *myitems;
} user;

typedef struct euclidean_t {
	int k;
	int dim;
	int NoOfUser;
	int totItems;
	user* users;
} dataEuclidean;

dataEuclidean euc;

typedef struct confEuclidean_t {

	int **v;
	int w;
	int *t;
	unsigned int *gt;

} confEuclidean;

int BinarySearchEuclidean(item* , int , int );

void* InitStructEuclidean() {
	int i,j;
	unsigned long long int temp = 1;
	unsigned long long int M = (temp << 32) - 5;
	confEuclidean *ce = malloc(sizeof(confEuclidean));

	ce->v = malloc((euc.k) * sizeof(int*));
	ce->w = 4;
	ce->t = malloc((euc.k)*sizeof(int));
	for (i = 0; i < euc.k; i++) {

		ce->t[i] = (int)GetUniform(0, ce->w);
		ce->v[i] = malloc((euc.dim) * sizeof(int));
		for (j = 0; j < euc.dim; j++) {
			ce->v[i][j] = GetNormal(0, 1);
		}
	}

	ce->gt = malloc((euc.k) * sizeof(unsigned int));
	for (i = 0; i < euc.k; i++) {
		//ce->gt[i] = malloc((euc.dim) * sizeof(double));
		//for (j = 0; j < euc.dim; j++) {
		ce->gt[i] = (unsigned int)GetUniform(0, M);
		//printf("%u\n", ce->gt[i]);
		//printf("%f\n",ce->gt[i][j]);
		//}
	}


	return ce;
}

void DestroyStructEuclidean(void** euVoid) {
	int i;
	confEuclidean* eu = *euVoid;
	free(eu->t);
	for(i=0; i<euc.k; i++)
		free((eu)->v[i]);
	free((eu)->v);

	free((eu)->gt);
	free(eu);
}

double distanceEuc(int *x, int *y){
	return abs(*x-*y);
}

unsigned long long int IntHashFuncEuc(int *x, void* arg){
	return *x;
}

void RateItemsEuclidean(value* lvalue, value** nn, int size, List Rresult){
	int i,j,k,pos;
	double R;
	int *x = lvalue->content;
	hashTable rated;
	int hashSize = euc.users[*x].NoOfItems*size;
	Rating rating;

	InitHashTable(&rated,hashSize,sizeof(int),NULL,
			distanceEuc,IntHashFuncEuc,NULL);
	//InitList(Rresult,sizeof(int),NULL,distance,NULL);

	for(i=0; i<euc.users[*x].NoOfItems; ++i){
		InsertHashTable(rated, &euc.users[*x].myitems[i].id,NULL);
	}
	double z = 0.0;
	for ( i = 0; i < size; ++i)
	{
		double tmp = (-data.distance(lvalue,nn[i])-1);
		z+= ( tmp <0 ? -tmp : tmp);
	}
	z = 1/z;
	double Ru = 0.0;
	for (k = 0; k < euc.users[*x].NoOfItems; ++k)
	{
		Ru+=euc.users[*x].myitems[k].score;
	}
	Ru/=euc.users[*x].NoOfItems;
	for(i=0; i<size; ++i){
		int* curr = nn[i]->content;

		for(j=0; j<euc.users[*curr].NoOfItems; ++j ){
			if(SearchHashTable(rated, &euc.users[*curr].myitems[j].id)!=NULL){
				continue;
			}
			InsertHashTable(rated, &euc.users[*curr].myitems[j].id,NULL);
			double sum = 0.0;
			int avg = 0;
			for (k = 0; k < euc.users[*curr].NoOfItems; ++k)
			{
				avg+=euc.users[*curr].myitems[k].score;
			}
			avg/=euc.users[*curr].NoOfItems;
			for(k=0; k<size; ++k){
				int l;
				int *index = nn[k]->content;
				// for(l=0; l<euc.users[*index].NoOfItems; l++){
				// 	printf("%d ",euc.users[*index].myitems[l].id );
				// }
				// printf("\n");
				
				pos=BinarySearchEuclidean(euc.users[*index].myitems,euc.users[*index].NoOfItems,euc.users[*curr].myitems[j].id);
				//printf("%d %d\n", euc.users[*curr].myitems[j].id,pos);
				if(pos == -1)
					continue;
				sum+= (-data.distance(lvalue,nn[k])+1)*(euc.users[k].myitems[pos].score-avg);
				//printf("dist %f score %d \n", (-data.distance(lvalue,nn[k])+1),euc.users[k].myitems[pos].score);
			}
			R = Ru+z*sum;
			rating.rate = R;
			rating.id = euc.users[*curr].myitems[j].id;
			InsertValueList(Rresult, &rating);
			//printf("%f , %d\n",R, euc.users[*curr].myitems[j].id);
		}

	}

	// HashToArr(rated,(void**) array);
	// for ( i = 0; i < euc.users[*x].NoOfItems; ++i)
	// {
	// 	printf("%d\n",*array[i] );
	// }
}

int BinarySearchEuclidean(item* myitems, int size, int id){

	unsigned int left = 1, right = size;

	while (left < right){
	    /* invariant: a[left] <= *KeyPtr <= a[right] or *KeyPtr not in Array */
		unsigned int m = (left  +  right) / 2; /*NOTE: *intentionally* truncate for odd sum */

		if (myitems[m].id < id) 
		  left = m + 1;       /* a[m] < *KeyPtr <= a[right] or *KeyPtr not in Array */
		else 
		  /* assert(right != m) or infinite loop possible */
		  right = m;          /* a[left] <= *KeyPtr <= a[m] or *KeyPtr not in Array */
	}
	/* assert(left == right) */
	if(myitems[right-1].id == id )
		return right-1;
	if(right < size && myitems[right].id == id )
		return right;
	return -1;
}

double GetIthScoreEuclidean(int user, int item){
	return euc.users[user].myitems[item].score;
}

double GetIthIdEuclidean(int user, int item){
	return euc.users[user].myitems[item].id;
}

void SetIthScoreEuclidean(int user, int item,int score){
	euc.users[user].myitems[item].score = score;
}

void DeleteItemEuclidean(int user, int item){

	euc.users[user].myitems[item].score = 0;
}

int GetTotalItemsEuclidean(){
	return euc.totItems;
}

bitStr InnerProductFmod(unsigned int *x, unsigned int *y) {
	int i;
	bitStr xy = 0;
	bitStr mod = 1;
	for (i = 0; i < euc.k; ++i) {
		// printf("%d\n",i);
		xy += x[i] * y[i];
		xy = fmod(xy, (mod << 32) - 5);
	}
	//printf("%lld\n", xy);
	return xy;
}


double InnerProductEuclidean(int *x, int *y) {
	int i = 0;
	int xy = 0;
	for (i = 0; i < euc.users[*x].NoOfItems; ++i)
	{
		int pos = euc.users[*x].myitems[i].id;
		//printf("%d\n", pos);
		xy += euc.users[*x].myitems[i].score * y[pos-1];
	}
	return xy;
	//int xy = 0.0;
	//while( i < euc.users[x].NoOfItems && j < euc.users[y].NoOfItems ){
	//	if( euc.users[x].myitems[i].id == euc.users[y].myitems[j].id){
	//		xy += euc.users[x].myitems[i].score * euc.users[y].myitems[j].score;
	//		i++;
	//		j++;
	//	}
	//	else if( euc.users[x].myitems[i].id > euc.users[y].myitems[j].id)
	//		j++;
	//	else if( euc.users[x].myitems[i].id < euc.users[y].myitems[j].id)
	//		i++;
	//	// printf("%d\n",i);
	//	//xy += x[i] * y[i];
	//	//xy = fmod(xy, (mod << 31) - 5);
	//}
	//return xy;
}


double Euclidean(value *valueX, value *valueY) {
	int *x, *y;
	x = valueX->content;
	y = valueY->content;
	int xy = 0.0;
	int i = 0, j = 0;
	while( i < euc.users[*x].NoOfItems && j < euc.users[*y].NoOfItems ){
		if( euc.users[*x].myitems[i].id == euc.users[*y].myitems[j].id){
			int diff = euc.users[*x].myitems[i].score -euc.users[*y].myitems[j].score;
			xy += diff*diff;
			i++;
			j++;
		}
		else if( euc.users[*x].myitems[i].id > euc.users[*y].myitems[j].id){
			int diff = euc.users[*y].myitems[j].score;
			xy += diff*diff;
			j++;
		}
		else if( euc.users[*x].myitems[i].id < euc.users[*y].myitems[j].id){
			int diff = euc.users[*x].myitems[i].score;
			xy += diff*diff;
			i++;
		}
		// printf("%d\n",i);
		//xy += x[i] * y[i];
		//xy = fmod(xy, (mod << 31) - 5);
	}
	return xy;




	/*
	double *x, *y;
	x = valueX->content;
	y = valueY->content;
	printf("%d, %d\n", *x, *y);
	//if ( valueX->extra != NULL && valueY->extra != NULL)
	//if ( *(bitStr*)valueX->extra != *(bitStr*)valueY->extra) 
	//return 9999.0;
	int i;
	double sq_dist = 0.0;
	for (i = 0; i < euc.dim; i++) {
		//printf("(%f - %f)^2 + ", x[i], y[i]);
		sq_dist += (x[i] - y[i])*(x[i] - y[i]);
		//sq_dist += pow((x[i]- y[i]), 2);
	}
	//printf("== %f\n", sqrt(sq_dist));
	//printf("\n\n\n");
	return sq_dist;
	return sqrt(sq_dist);
	*/
}

int EqualEuclidean(value *valueX, value *valueY) {
	return (Euclidean(valueX, valueY) == 0);
}

int InRangeEuclidean(value *valueX, value *valueY, double R) {
	return (Euclidean(valueX, valueY) <= R);
}

void PrintEuclidean(double dist) {
	printf("%f\n", dist);
}

int HiEuclidean(value* valueP, confEuclidean *ce, int i) {
	int *p = valueP->content;
	double res = InnerProductEuclidean(p, ce->v[i]);
	return (int) floor((res + ce->t[i]) / ce->w);
}

void GEuclidean(value* valueP, confEuclidean *ce, unsigned int* res) {
	int i;
	for (i = 0; i < euc.k; ++i)
		res[i] = HiEuclidean(valueP, ce,i);
}

bitStr FEuclidean(value* valueP, void *voidCE) {
	confEuclidean *ce = voidCE;
	unsigned int *gMatrix;
	bitStr res = 0;
	gMatrix = malloc(sizeof(unsigned int) * euc.k);
	//here
	GEuclidean(valueP, ce, gMatrix);
	res = InnerProductFmod(ce->gt, gMatrix);

	//for (i = 0; i < euc.k; ++i) {
	//GEuclidean(valueP, ce, gMatrix);
	//res = fmod(res, (mod << 31) - 5);
	//}

	/*if ( valueP->extra == NULL){
	  valueP->extra = malloc(sizeof(bitStr));
	 *(bitStr*)valueP->extra = res;
	 }*/
	free(gMatrix);
	return (bitStr) floor(res);
}


unsigned long long int HashEuclidean(value* value){

	unsigned long long int square = 0;
	int i;
	int *x = value->content;
	for( i = 0;i < euc.users[*x].NoOfItems; i++) {
		int d = euc.users[*x].myitems[i].score;
		square += d*d;
	}
	return square;

}

void PrintDataEuclidean(value* v){
	int i;
	printf("%s  ",v->name);
	for(i=0; i<euc.dim; i++)
		printf("%f  ",((double*)v->content)[i]);
	printf("\n\n");
}

void PrintDataEuclidean2(value* v){

	int i=*(int*)(v->content);
	int j;
	printf("User: %d",euc.users[i].id);
//	for(j=0;j<euc.users[i].NoOfItems;j++){
//		printf("Item: id=%d score=%d\n",
//				euc.users[i].myitems[j].id,euc.users[i].myitems[j].score);
//	}
}

void DestroyInputEuclidean() {
	//DestroyList(&(euc.data));
	//DestroyList( &(euc.queries));
//	free(euc);
}

void DestroyValueEuclidean(void* vVoid) {
	value* v = vVoid;
	/*if ( v->extra != NULL)
	  {
	  free(v->extra);
	  v->extra = NULL;
	  }*/
	if (v->content != NULL) {
		free(v->content);
		v->content = NULL;
	}
	if (v->name != NULL) {
		free(v->name);
		v->name = NULL;
	}
}


void ReadDataEuclidean(value** val,FILE* input, int k) {

	List tempList;
	euc.k = k;
	euc.totItems = 0;

	size_t lineSize = 0;
	size_t lineLen = 0;

	char* lineBuff = NULL;
	char* token=NULL;

	char delims[4]="\t \n";

	int nn;
	InitList(&tempList, sizeof(List*), NULL, NULL, NULL);

	lineLen = getline(&lineBuff, &lineSize, input);
	if (sscanf(lineBuff, "%d",  &nn) < 2) {
		//perror("Malformed file - Exiting...\n");
		//exit(-1);
		nn = 20;
	}
	else{
		lineLen = getline(&lineBuff, &lineSize, input);
	}
	data.Pnn = nn;
	//secBuff=malloc((strlen(lineBuff)+1)* sizeof(char));
	//memcpy(secBuff,lineBuff,strlen(lineBuff));
	//secBuff[strlen(lineBuff)]='\0';

	int startUser;
	int prevUser = -1;
	int maxDim = -1;
	do{
		List tempList2;
		//temp = malloc(sizeof(value));
		token = strtok(lineBuff, delims);
		if (token == NULL)
			break;
		int currUser = atoi(token);

		if( prevUser == -1)
			startUser = currUser;

		if(currUser != prevUser ){
			prevUser = currUser;
			InitList(&tempList2, sizeof(item*), NULL, NULL, NULL);
			InsertValueList(tempList, &tempList2);
		}
		item *it = malloc(sizeof(item));
		token = strtok(NULL, delims);
		it->id = atoi(token);
		if(it->id > maxDim )
			maxDim = it->id;
		token = strtok(NULL, delims);
		it->score = atoi(token);
		InsertValueList(tempList2, it);
		euc.totItems++;
		free(it);
	} while ((lineLen = getline(&lineBuff, &lineSize, input)) != -1);
	euc.dim = maxDim;



	int NoOfUser = SizeList(tempList);
	euc.users = malloc(sizeof(user)* NoOfUser);
	euc.NoOfUser = NoOfUser;
	int i;
	List *tmptempList2 = GetFirst(tempList);
	List tempList2 = *tmptempList2;
	if( tempList2 == NULL )
		printf("NULLLLL\n");
	for (i = 0; i < NoOfUser; ++i)
	{
		int NoOfItems = SizeList(tempList2);
		euc.users[i].myitems = malloc(sizeof(item)* NoOfItems);
		euc.users[i].id = i + 1;
		euc.users[i].NoOfItems = NoOfItems;
		int j;
		item *it = GetFirst(tempList2);
		for (j = 0; j < NoOfItems; ++j)
		{
			euc.users[i].myitems[j] = *it;
			it = GetNext(tempList2);
		}
		DestroyList(&tempList2);
		tmptempList2 = GetNext(tempList);
		if(tmptempList2 == NULL)
			break;
		tempList2 = *tmptempList2;
	}
/**/
	SetDataSize(NoOfUser);
	*val = malloc(NoOfUser*sizeof(value));
	for(i = 0; i< GetDataSize(); i++){
		//(*val)[i] = malloc(sizeof(value));
		(*val)[i].name = NULL;
		int *content = malloc(sizeof(int));
		*content = i;
		(*val)[i].content = content;
	}
	/**/

	data.tableSize = SizeList(tempList)/8;
	DestroyList(&tempList);
	//data.tableSize = 10;
	free(lineBuff);
	/*
	for (i = 0; i < euc.NoOfUser; ++i)
	{
		int j;
		printf("User %d\n", i);
		for (j = 0; j < euc.users[i].NoOfItems; ++j){
			printf("Item: id=%d score=%d\n",euc.users[i].myitems[j].id, euc.users[i].myitems[j].score );
		}

		//free(euc.users[i].myitems);
	}
	*/
	//free(euc.users);
	//free(secBuff);
}


