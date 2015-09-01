#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>


#include "data.h"
#include "dataFunction.h"
#include "list.h"
#include "random.h"


#define SWAP(a, b) { tmp = v[a]; v[a] = v[b]; v[b] = tmp; }

extern dataIF data;

double qselectFunction(double *v, int len, int k){

	int i, st, tmp;

	for (st = i = 0; i < len - 1; i++) {
		if (v[i] > v[len-1]) continue;
		SWAP(i, st);
		st++;
	}

	SWAP(len-1, st);

	return k == st	?v[st]
			:st > k	? qselect(v, st, k)
				: qselect(v + st, len - st, k - st);
}

typedef struct dataFunction_t{
	void* initializer;
    value* data;
    value* queries;
    int k;
    int queryNum;
    int dataNum;
    int iterator;
    int qIterator;
    double (*distance)( char *, char *);
    
}dataFunction;
	    
	    
dataFunction* fun;


typedef struct confFunction_t {
	int index;
	double t1;
	double t2;
	double* T;
} confFunction;

void ReportFunction(){

	printf("DataNum: %d\n",fun->dataNum);
	printf("DataIterator: %d\n",fun->iterator);
	printf("QueryNum: %d\n",fun->queryNum);
	printf("QueryIterator: %d\n",fun->qIterator);

}


void* InitStructFunction() {
	int i;
	//static int currIndex = 0;
	confFunction* cF = malloc(sizeof(confFunction));
	cF->T = malloc(sizeof(double) * fun->k);
	for (i = 0; i < fun->k; ++i)
		cF->T[i] = 0.0 / 0.0;
	printf("%d\n", fun->dataNum);
	cF->index = GetUniform(0, fun->dataNum);
	return cF;
}

void DestroyStructFunction(void** cFVoid) {
	confFunction* cF = *cFVoid;
	// printf("%p\n\n", cF);
	free((cF)->T);
	free(cF);
}

value* GetFirstFunction(){

	fun->iterator=0;
	return &(fun->data[fun->iterator]);
}

value* GetNextFunction(){

	if(fun->iterator >= fun->dataNum-1)
		return NULL;
	return &(fun->data[++fun->iterator]);
}

value* GetFirstQueryFunction(){

	fun->qIterator=0;
	return &(fun->queries[fun->qIterator]);
}


value* GetNextQueryFunction(){

	ReportFunction();
	if(fun->qIterator >= fun->queryNum-1){
		printf("Epistrefw NULL guys!\n");
		return NULL;
	}
	return &(fun->queries[++fun->qIterator]);
}

double DistanceFunction(value* x, value* y){

	return fun->distance(x->name, y->name);
	
}

int EqualFunction(value* x1, value* x2) {
	return DistanceFunction(x1, x2) == 0;
}

int InRangeFunction(value* x1, value* x2, double r) {
	return DistanceFunction(x1, x2) <= r;
}

void PrintFunction(value *x) {
	printf("%s\n", x->name);

}


double HiFunctionActual(value* x, value* x1, value* x2) {
	double dX1X2 = DistanceFunction(x1, x2);
	double res = square(DistanceFunction(x, x1))
			+ square(DistanceFunction(x, x2)) - square(dX1X2);
	res /= 2 * dX1X2;
	return res;
}

int HiFunction(value* x, value* x1, value* x2, double t1) {
	double res = HiFunctionActual(x, x1, x2);
	return t1 <= res;
}

/**
 * get the item with name itemi
 * ex. for i = 1 get item1
 **/

void GetIthItemFunction(int i, value* v) {

	v->name = fun->data[i].name;
}


double squareFunction(double value) {
	return value * value;
}

/**
 * get the median of HiFunctionActual(x, x1, x2)
 * for every x in our dataset
 *
 **/
double GetMedianFunction(value* x1, value* x2) {
	int i;
	value* x = malloc(sizeof(value));
	double* values = malloc(sizeof(double) * fun->dataNum);
	for (i = 0; i < fun->dataNum; ++i) {
		GetIthItemFunction(i, x);
		values[i] = HiFunctionActual(x, x1, x2);
	}

	double res = qselectFunction(values,fun->dataNum,0);

	free(values);
	free(x);
	return res;
}

unsigned long long int GFunction(value* x, void* cFVoid) {
	int i;
	unsigned long long int res = 0;
	unsigned long long int initial = 1;
	confFunction* cF = cFVoid;
	value* x1, *x2;
	x1 = malloc(sizeof(value));
	x2 = malloc(sizeof(value));
	GetIthItemFunction(cF->index, x2);
	for (i = 0; i < fun->k; ++i) {
		GetIthItemFunction(i, x1);
		if (cF->T[i] != cF->T[i])
			cF->T[i] = GetMedianFunction(x1, x2);
		initial = HiFunction(x, x1, x2, cF->T[i]);
		res |= initial << i;
	}
	free(x1);
	free(x2);
	return res;
}

double BruteForceANNFunction(value query, List nn){
	value* temp = data.getFirst();


	double min_distance = data.distance(temp, &query);
	double dist;

	InsertValueList(nn, temp);
	while (temp != NULL) {
		dist = data.distance(temp, &query);

		if (dist < min_distance) {
			DeleteAllFromList(nn);
			InsertValueList(nn, temp);
			min_distance = dist;

		} else if (dist == min_distance) {
			InsertValueList(nn, temp);
		}
		temp = data.getNext();
	}
	return min_distance;
}


void DestroyInputFunction() {
	int i;

	for (i = 0; i < fun->queryNum; ++i)
	{
		free(fun->queries[i].name);

	}
	for (i = 0; i < fun->dataNum; ++i)
	{
		free(fun->data[i].name);

	}

	free(fun->data);
	free(fun->queries);
	dlclose(fun->initializer);
	free(fun);
}

void DestroyValueFunction(void* vVoid) {
	value* v = vVoid;
	if (v->name != NULL) {
		free(v->name);
		v->name = NULL;
	}
}

void ReadDataFunction(FILE* inputFile, int k) {
	List tempList;

	size_t lineSize = 0;

	char* lineBuff = NULL;
	char secBuff1[256];
	char library[256];
	char function[256];
	char* temp;
	char delims[4] = "\t ,";
	char* token = NULL;
	int num = 0;
	int i;
	int line = 0;

	fun = malloc(sizeof(dataFunction));
	
	fun->iterator=0;
	fun->qIterator=0;
	fun->k = k;
	getline(&lineBuff, &lineSize, inputFile);
	/*skip the first value*/
	token = strtok(lineBuff, delims);

	InitList(&tempList, BUFFSIZE * sizeof(char), NULL, NULL,NULL);

	while (token != NULL) {
		token = strtok(NULL, delims);
		if (token == NULL)
			break;
		temp = malloc((LABELSIZE + 1) * sizeof(char));
		memcpy(temp, token, strlen(token));
		temp[strlen(token)] = '\0';
		InsertValueList(tempList, temp);
		num++;
		free(temp);
	}

	fun->dataNum = num;

	fun->data= malloc(num* sizeof(value));

	char* tempVal = GetFirst(tempList);
	for (i = 0; i < num; i++) {
		fun->data[i].name = malloc(LABELSIZE * sizeof(char));
		memset(fun->data[i].name,'\0',LABELSIZE);
		if(tempVal ==NULL)
			continue;
		strcpy(fun->data[i].name, tempVal);
		tempVal=GetNext(tempList);
	}


	getline(&lineBuff, &lineSize, inputFile);

	memset(&secBuff1,'\0',sizeof(secBuff1));
	memset(&library,'\0',sizeof(library));
	sscanf(lineBuff,"%s %s",secBuff1,library+2);
	library[0] = '.';
	library[1] = '/';

	printf("%s\n",library);
	
	getline(&lineBuff, &lineSize, inputFile);

	memset(&secBuff1,'\0',sizeof(secBuff1));
	memset(&function,'\0',sizeof(function));
	sscanf(lineBuff,"%s %s",secBuff1,function);
	
	

	fun->initializer = dlopen(library,RTLD_LAZY);
	
	if (fun->initializer == NULL)
		printf("NULL\n");;
	*(void **)(&(fun->distance)) = dlsym(fun->initializer,function);



	data.tableSize = fun->dataNum;
	DestroyList(&tempList);
	free(lineBuff);


}
int GetQueryNumFunction(FILE* fp){

	int ch, number_of_lines = 0;

	do
	{
	    ch = fgetc(fp);
	    if(ch == '\n')
	    	number_of_lines++;
	} while (ch != EOF);

	rewind(fp);
	return number_of_lines;
}


void ReadQueriesFunction(FILE* inputFile) {


	size_t lineSize = 0;

	char* lineBuff = NULL;

	int i=0;
	char delims[6]="\t \n";
	char* token=NULL;

	fun->queryNum=GetQueryNumFunction(inputFile);
	fun->queryNum--;
	printf("Tha diavasw %d queries\n",fun->queryNum);
	getline(&lineBuff, &lineSize, inputFile);
	//scanf gia to radius
	free(lineBuff);
	lineSize=0;


	fun->queries= malloc(fun->queryNum* sizeof(value));
	printf("%d\n",fun->queryNum);


	while (getline(&lineBuff, &lineSize, inputFile) != -1) {

		token = strtok(lineBuff, delims);

		fun->queries[i].name=malloc(LABELSIZE*sizeof(char));

		strcpy(fun->queries[i].name,token);

		i++;
	}
	free(lineBuff);

	//printEverything();
}










