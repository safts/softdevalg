#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dataCosine.h"
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

typedef struct Cosine_t {
	int k;
	int dim;
	int NoOfUser;
	user* users;
	int totItems;
} dataCosine;

dataCosine co;



typedef struct confCosine_t {

	double **r;

} confCosine;

int BinarySearchCosine(item* , int , int );

void* InitStructCosine() {
	int i;
	confCosine *ce = malloc(sizeof(confCosine));

	ce->r = malloc(( co.k) * sizeof(double*));
	int j;
	for (j = 0; j < co.k; ++j) {
		ce->r[j] = malloc((co.dim) *sizeof(double));
	}
	for (j = 0; j < co.k; ++j) {
		for (i = 0; i <  co.dim; i++) {
			ce->r[j][i] = GetNormal(0, 1);
		}
	}
	return ce;
}

void DestroyStructCosine(void** confCoVoid) {
	confCosine* confCo = *confCoVoid;
	int j;
	for (j = 0; j < co.k; ++j) {
		free(confCo->r[j]);
	}
	free(confCo->r);
	free(confCo);
}

double distanceCos(int *x, int *y){
	return abs(*x-*y);
}

unsigned long long int IntHashFuncCos(int *x, void* arg){
	return *x;
}
void destroy(){

}

void RateItemsCosine(value* lvalue, value** nn, int size, List Rresult){
	int i,j,k,pos;
	double R;
	int *x = lvalue->content;
	hashTable rated;
	int hashSize = co.users[*x].NoOfItems*size;
	Rating rating;

	InitHashTable(&rated,hashSize,sizeof(int),NULL,
			distanceCos,IntHashFuncCos,destroy);
	//InitList(Rresult,sizeof(int),NULL,distance,NULL);

	for(i=0; i<co.users[*x].NoOfItems; ++i){
		if(co.users[*x].myitems[i].score != 0)
			InsertHashTable(rated, &co.users[*x].myitems[i].id,NULL);
	}
	double z = 0.0;
	for ( i = 0; i < size; ++i)
	{
		//z+=abs((-data.distance(lvalue,nn[i])-1));
		double tmp = (-data.distance(lvalue,nn[i])-1);
		z+= ( tmp <0 ? -tmp : tmp);
	}
	z = 1/z;

	double Ru = 0.0;
	for (k = 0; k < co.users[*x].NoOfItems; ++k)
	{
		Ru+=co.users[*x].myitems[k].score;
	}
	//printf("dddd%d\n",co.users[*x].NoOfItems );
	Ru/=co.users[*x].NoOfItems;
	//printf("ddd1d%d\n",co.users[*x].NoOfItems );
	for(i=0; i<size; ++i){
		int* curr = nn[i]->content;

		for(j=0; j<co.users[*curr].NoOfItems; ++j ){
			if(SearchHashTable(rated, &co.users[*curr].myitems[j].id)!=NULL){
				continue;
			}
			InsertHashTable(rated, &co.users[*curr].myitems[j].id,NULL);
			double sum = 0.0;
			int avg = 0;
			for (k = 0; k < co.users[*curr].NoOfItems; ++k)
			{
				avg+=co.users[*curr].myitems[k].score;
			}
			//printf("eeee%d\n",co.users[*x].NoOfItems );
			avg/=co.users[*curr].NoOfItems;
			//printf("eeee%d\n",co.users[*x].NoOfItems );
			for(k=0; k<size; ++k){
				int l;
				int *index = nn[k]->content;
				// for(l=0; l<co.users[*index].NoOfItems; l++){
				// 	printf("%d ",co.users[*index].myitems[l].id );
				// }
				// printf("\n");
				
				pos=BinarySearchCosine(co.users[*index].myitems,co.users[*index].NoOfItems,co.users[*curr].myitems[j].id);
				//printf("%d %d\n", co.users[*curr].myitems[j].id,pos);
				if(pos == -1)
					continue;
				sum+= (-data.distance(lvalue,nn[k])+1)*(co.users[k].myitems[pos].score-avg);
				//printf("dist %f score %d \n", (-data.distance(lvalue,nn[k])+1),co.users[k].myitems[pos].score);
			}
			R = Ru+z*sum;
			rating.rate = R;
			rating.id = co.users[*curr].myitems[j].id;
			InsertValueList(Rresult, &rating);
			//printf("%f , %d\n",R, co.users[*curr].myitems[j].id);
		}

	}
	DestroyHashTable(&rated);
}

int BinarySearchCosine(item* myitems, int size, int id){

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

double GetIthScoreCosine(int user, int item){
	return co.users[user].myitems[item].score;
}

double GetIthIdCosine(int user, int item){
	return co.users[user].myitems[item].id;
}

void SetIthScoreCosine(int user, int item,int score){
	co.users[user].myitems[item].score = score;
}

void DeleteItemCosine(int user, int item){

	co.users[user].myitems[item].score = 0;
}

int GetTotalItemsCosine(){
	return co.totItems;
}


double InnerProductCosine(int *x, int *y) {
	double xy = 0.0;
	int i = 0, j = 0;
	while( i < co.users[*x].NoOfItems && j < co.users[*y].NoOfItems ){
		if( co.users[*x].myitems[i].id == co.users[*y].myitems[j].id){
			int diff = co.users[*x].myitems[i].score *co.users[*y].myitems[j].score;
			xy += diff;
			i++;
			j++;
		}
		else if( co.users[*x].myitems[i].id > co.users[*y].myitems[j].id){
			j++;
		}
		else if( co.users[*x].myitems[i].id < co.users[*y].myitems[j].id){
			i++;
		}
	}
	return xy;

}

double InnerProductNumber(int *x, double *r){
	int i = 0;
	double xy = 0.0;
	for (i = 0; i < co.users[*x].NoOfItems; ++i)
	{
		int pos = co.users[*x].myitems[i].id;
		//printf("%d\n", pos);
		xy += co.users[*x].myitems[i].score * r[pos-1];
	}
	return xy;

}

unsigned long long int HashCosine(value* value, void* arg){

	unsigned long long int square = 0;
	int i;
	int *x = value->content;
	for( i = 0;i < co.users[*x].NoOfItems; i++) {
		int d = co.users[*x].myitems[i].score;
		square += d*d;
	}
	return square;

}

double Norm(int *x){
	int square = 0;
	int i;
	for( i = 0;i < co.users[*x].NoOfItems; i++) {
		int d = co.users[*x].myitems[i].score;
		square += d*d;
	}
	return sqrt(square);
}

double Cosine(value* x, value* y){
	double dist;
	dist = InnerProductCosine(x->content,y->content)/(Norm(x->content)*Norm(y->content));
	return - (dist -1 );
}

int EqualCosine(value *valueX, value *valueY) {
	return (Cosine(valueX, valueY) == 0);
}

int InRangeCosine(value *valueX, value *valueY, double R) {
	return (Cosine(valueX, valueY) <= R);
}

void PrintCosine(double dist) {
	printf("%f\n", dist);
}
void PrintDataCosine(value *v) {
	int i=*(int*)(v->content);
	int j;
	printf("User: %d",co.users[i].id);
	/*for(j=0;j<co.users[i].NoOfItems;j++){
		printf("Item: id=%d score=%d\n",
				co.users[i].myitems[j].id,co.users[i].myitems[j].score);
	}*/
}

void PrintDataCosine2(int i) {
	//int i=*(int*)(v->content);
	int j;
	printf("User: %d\n",co.users[i].id);
	/*for(j=0;j<co.users[i].NoOfItems;j++){
		printf("Item: id=%d score=%d\n",
				co.users[i].myitems[j].id,co.users[i].myitems[j].score);
	}*/
}

int HiCosine(int* x, confCosine *cc, int i) {
	double d = InnerProductNumber(x,cc->r[i]);
	//printf("hhhhhh %f\n",d );
	if(d < 0.0 )
		return 0;
	else
		return 1;
}

bitStr GCosine(value* value, void* cfcVoid) {
	confCosine* cfc = cfcVoid;
	bitStr res = 0;
	bitStr initial = 1;
	int i;

	for (i =0; i<co.k; ++i) {
		initial = HiCosine(value->content, cfc, i);
		//printf("gggggg %llu\n",initial );
		res |= initial << i;
	}
	// //printf("%llu\n\n",res );
	// static char b[9];
 //    b[0] = '\0';

 //    int z;
 //    for (z = 64; z > 0; z >>= 1)
 //    {
 //        strcat(b, ((res & z) == z) ? "1" : "0");
 //    }
	// printf("res %s\n\n",b );
	return res;
}

void DestroyInputCosine() {
	int i,j;
	for(i=0; i<co.NoOfUser; ++i){
		free(co.users[i].myitems);
	}
	free(co.users);

}

void DestroyValueCosine(void* vVoid) {
	value* v = vVoid;

	if (v->content != NULL) {
		free(v->content);
		v->content = NULL;
	}
	if (v->name != NULL) {
		free(v->name);
		v->name = NULL;
	}
}

void ReadDataCosine(value** val,FILE* input, int k) {

	List tempList;
	co.k = k;
	co.totItems = 0;

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
		co.totItems++;
		free(it);
	} while ((lineLen = getline(&lineBuff, &lineSize, input)) != -1);
	co.dim = maxDim;



	int NoOfUser = SizeList(tempList);
	co.users = malloc(sizeof(user)* NoOfUser);
	co.NoOfUser = NoOfUser;
	int i;
	List *tmptempList2 = GetFirst(tempList);
	List tempList2 = *tmptempList2;
	if( tempList2 == NULL )
		printf("NULLLLL\n");
	for (i = 0; i < NoOfUser; ++i)
	{
		int NoOfItems = SizeList(tempList2);
		co.users[i].myitems = malloc(sizeof(item)* NoOfItems);
		co.users[i].id = i + 1;
		co.users[i].NoOfItems = NoOfItems;
		int j;
		item *it = GetFirst(tempList2);
		for (j = 0; j < NoOfItems; ++j)
		{
			co.users[i].myitems[j] = *it;
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
		InitClustExtra(i);
	}
	/**/

	data.tableSize = (1 << co.k)/2;
	DestroyList(&tempList);
	//data.tableSize = 10;
	free(lineBuff);
	/*
	for (i = 0; i < co.NoOfUser; ++i)
	{
		int j;
		printf("User %d\n", i);
		for (j = 0; j < co.users[i].NoOfItems; ++j){
			printf("Item: id=%d score=%d\n",co.users[i].myitems[j].id, co.users[i].myitems[j].score );
		}

		//free(co.users[i].myitems);
	}
	*/
	//free(co.users);
	//free(secBuff);
}
