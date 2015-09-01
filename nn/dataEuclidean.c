#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dataEuclidean.h"
#include "data.h"
#include "random.h"

extern dataIF data;

//typedef unsigned long long int bitStr;

typedef struct euclidean_t {

	List data;
	List queries;
	int k;
	int dim;

} dataEuclidean;

dataEuclidean* euc;

typedef struct confEuclidean_t {

	double *v;
	int w;
	int t;
	unsigned int *gt;

} confEuclidean;

void* InitStructEuclidean() {
	int i;
	unsigned long long int temp = 1;
	unsigned long long int M = (temp << 32) - 5;
	confEuclidean *ce = malloc(sizeof(confEuclidean));

	ce->v = malloc((euc->dim) * sizeof(double));
	for (i = 0; i < euc->dim; i++) {
		ce->v[i] = GetNormal(0, 1);
	}

	ce->gt = malloc((euc->k) * sizeof(unsigned int));
	for (i = 0; i < euc->k; i++) {
		//ce->gt[i] = malloc((euc->dim) * sizeof(double));
		//for (j = 0; j < euc->dim; j++) {
		ce->gt[i] = (unsigned int)GetUniform(0, M);
		//printf("%u\n", ce->gt[i]);
		//printf("%f\n",ce->gt[i][j]);
		//}
	}
	ce->w = 4;
	ce->t = GetUniform(0, ce->w);
	return ce;
}

void DestroyStructEuclidean(void** euVoid) {
    //int i;
	confEuclidean* eu = *euVoid;
	// printf("%p\n\n", ch);
	free((eu)->v);
    //for(i=0; i<euc->k; i++)
        //free((eu)->gt[i]);
    free((eu)->gt);
	free(eu);
}

value* GetFirstEuclidean() { 
	return GetFirst(euc->data); 
}

value* GetNextEuclidean() { 
	return GetNext(euc->data); 
}


value* GetFirstQueryEuclidean() { 
	return GetFirst(euc->queries); 
}

value* GetNextQueryEuclidean() { 
	return GetNext(euc->queries); 
}

bitStr InnerProductFmod(unsigned int *x, unsigned int *y) {
	int i;
	bitStr xy = 0;
	bitStr mod = 1;
	for (i = 0; i < euc->k; ++i) {
       // printf("%d\n",i);
		xy += x[i] * y[i];
		xy = fmod(xy, (mod << 32) - 5);
	}
	//printf("%lld\n", xy);
	return xy;
}


double InnerProduct(double *x, double *y) {
	int i;
	double xy = 0.0;
	for (i = 0; i < euc->dim; ++i) {
       // printf("%d\n",i);
		xy += x[i] * y[i];
		//xy = fmod(xy, (mod << 31) - 5);
	}
	return xy;
}

double Euclidean(value *valueX, value *valueY) {
	double *x, *y;

	x = valueX->content;
	y = valueY->content;
	//if ( valueX->extra != NULL && valueY->extra != NULL)
		//if ( *(bitStr*)valueX->extra != *(bitStr*)valueY->extra) 
			//return 9999.0;
	int i;
	double sq_dist = 0.0;
	for (i = 0; i < euc->dim; i++) {
		//printf("(%f - %f)^2 + ", x[i], y[i]);
		//sq_dist += (x[i] - y[i])*(x[i] - y[i]);
		sq_dist += pow((x[i] - y[i]), 2);
	}
	//printf("== %f\n", sqrt(sq_dist));
	//printf("\n\n\n");
	return sqrt(sq_dist);
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

int HiEuclidean(value* valueP, confEuclidean *ce) {
	double *p = valueP->content;
	double res = InnerProduct(p, ce->v);
	return (int) floor((res + ce->t) / ce->w);
}

void GEuclidean(value* valueP, confEuclidean *ce, unsigned int* res) {
	int i;

	for (i = 0; i < euc->dim; ++i)
		res[i] = HiEuclidean(valueP, ce);
}

bitStr FEuclidean(value* valueP, void *voidCE) {
	confEuclidean *ce = voidCE;
	unsigned int *gMatrix;
	bitStr res = 0;
	gMatrix = malloc(sizeof(unsigned int) * euc->dim);
	GEuclidean(valueP, ce, gMatrix);
	res = InnerProductFmod(ce->gt, gMatrix);

	//for (i = 0; i < euc->k; ++i) {
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

void ReadDataEuclidean(FILE* inputFile, int k) {
    

	size_t lineSize = 0;
	size_t lineLen = 0;
    
	value* temp = NULL;

	char* lineBuff = NULL;
	char* token=NULL;
	char* secBuff= NULL;

	char delims[3]="\t ";

	int i = 0;
	int j=0;

	euc = malloc(sizeof(dataEuclidean));
	euc->k = k;
	euc->dim=0;


	lineLen = getline(&lineBuff, &lineSize, inputFile);
	lineLen = getline(&lineBuff, &lineSize, inputFile);

	secBuff=malloc((strlen(lineBuff)+1)* sizeof(char));
	memcpy(secBuff,lineBuff,strlen(lineBuff));
	secBuff[strlen(lineBuff)]='\0';

	InitList(&(euc->data), sizeof(value), data.print, data.distance,
			data.destroyValue);

	token = strtok(secBuff, delims);

	while (token != NULL) {
		token = strtok(NULL, delims);
		if (token == NULL)
			break;
		euc->dim++;
	}
	do{

		temp = malloc(sizeof(value));
		token = strtok(lineBuff, delims);
		temp->name = malloc((strlen(token) + 1) * sizeof(char));
		strcpy(temp->name, token);
		temp->content = malloc((euc->dim)*sizeof(double));
		temp->extra = NULL;
		//printf("ID: %s Value:", temp->name);
		if (token == NULL)
			break;
		for (j = 0; j < euc->dim; j++) {

			token = strtok(NULL, delims);
			double* content=temp->content;
			content[j] = atof(token);
			//printf("%f ", content[i]);
		}

		InsertValueList((euc->data), temp);
        free(temp);
		i++;
		//printf("\n");
	} while ((lineLen = getline(&lineBuff, &lineSize, inputFile)) != -1);
	
	data.tableSize = SizeList(euc->data);
	free(lineBuff);
    free(secBuff);
}

void PrintDataEuclidean(value* v){
    int i;
    printf("%s  ",v->name);    
    for(i=0; i<euc->dim; i++)
        printf("%f  ",((double*)v->content)[i]);
    printf("\n\n");
}

void printEverythingEuclidean(){

	int i;
	value* data=GetFirst(euc->data);
	value* query=GetFirst(euc->queries);

	printf("Data\n");
	while(data!=NULL){
		printf("ID: %s , value: ",data->name);
		for(i=0;i<euc->dim;i++){
			if(i == euc->dim /2){
				printf(" ... ");
				continue;
			}
			if(i > 3 && i < euc->dim -3)
				continue;
			if(i == euc->dim /2)
				printf(" ... ");
			double* content=data->content;
			printf("%f ",content[i]);
		}
		printf("\n");
		data=GetNext(euc->data);
	}
	printf("\n\nQueries\n\n");
	while(query!=NULL){
		printf("ID: %s , value: ",query->name);
		for(i=0;i<euc->dim;i++){
			if(i == euc->dim /2){
				printf(" ... ");
				continue;
			}
			if(i > 3 && i < euc->dim -3)
				continue;

			double* content=query->content;
			printf("%f ",content[i]);
		}
		printf("\n");
		query=GetNext(euc->queries);
	}

}

void ReadQueriesEuclidean(FILE* inputFile) {

	size_t lineSize = 0;
	size_t lineLen = 0;

	value* temp = NULL;

	char* lineBuff = NULL;
	char* token=NULL;
	char secBuff[512];
	double radius=0.0;

	char delims[3]="\t ";

	int i = 0;
	int j;

	memset(secBuff,'\0',sizeof(secBuff));

	lineLen = getline(&lineBuff, &lineSize, inputFile);
	if (sscanf(lineBuff, "%s %lf", secBuff, &radius) < 2) {
		perror("Malformed file - Exiting...\n");
		exit(-1);
	}
	data.radius = radius;

	InitList(&(euc->queries), sizeof(value), data.print, data.distance,
			data.destroyValue);

	while ((lineLen = getline(&lineBuff, &lineSize, inputFile)) != -1){

		temp = malloc(sizeof(value));
		token = strtok(lineBuff, delims);
		temp->name = malloc((strlen(token) + 1) * sizeof(char));
		strcpy(temp->name, token);
		temp->content = malloc((euc->dim)*sizeof(double));
		temp->extra = NULL;
		//printf("ID: %s Value:", temp->name);
		if (token == NULL)
			break;
		for (j = 0; j < euc->dim; j++) {

			token = strtok(NULL, delims);
			double* content=temp->content;
			content[j] = atof(token);
			//printf("%f ", content[i]);
		}

		InsertValueList((euc->queries), temp);
        free(temp);
		i++;
		//printf("\n");
	}

	free(lineBuff);
	//printEverythingEuclidean();
}

double BruteForceANNEuclidean(value query, List nn) {
	value* temp =GetFirst(euc->data);

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

		temp = GetNext(euc->data);
	}
	return min_distance;
}

void DestroyInputEuclidean() {
	DestroyList(&(euc->data));
	DestroyList( &(euc->queries));
	free(euc);
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


