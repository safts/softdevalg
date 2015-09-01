#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../dataStruct/data.h"
#include "../input/random.h"

#include "../dataStruct/list.h"

#include "dataDistanceMatrix.h"



/*
 * to data einai to global, to conf einai hidden
 */

extern dataIF data;


double qselect(double *v, int len, int k){

	int i, st, tmp;

	for (st = i = 0; i < len - 1; i++) {
		if (v[i] > v[len-1]) 
			continue;
		//SWAP(i, st);
		tmp = v[i];
		v[i] = v[st];
		v[st] = tmp;
		st++;
	}

	//SWAP(len-1, st);
	tmp = v[len-1];
	v[len-1] = v[st];
	v[st] = tmp;

	return k == st	?v[st]
			:st > k	? qselect(v, st, k)
				: qselect(v + st, len - st, k - st);
}


typedef struct distanceMatrix_t {

	value* data;
	double** dataDist;
	int dataNum;
	int iterator;

	value* queries;
	double** queryDist;
	int queryNum;
	int qIterator;

	int k;

} datadistanceMatrix;

datadistanceMatrix* dm;

typedef struct confDistanceMatrix_t {
	int index;
	double t1;
	double t2;
	double* T;
} confDistanceMatrix;

void Report(){

	printf("DataNum: %d\n",dm->dataNum);
	printf("DataIterator: %d\n",dm->iterator);
	printf("QueryNum: %d\n",dm->queryNum);
	printf("QueryIterator: %d\n",dm->qIterator);

}

void* InitStructDistanceMatrix() {
	int i;
	//static int currIndex = 0;
	confDistanceMatrix* ddm = malloc(sizeof(confDistanceMatrix));
	ddm->T = malloc(sizeof(double) * dm->k);
	for (i = 0; i < dm->k; ++i)
		ddm->T[i] = 0.0 / 0.0;
	ddm->index = GetUniform(0, dm->dataNum);
	return ddm;
}

void DestroyStructDistanceMatrix(void** ddmVoid) {
	confDistanceMatrix* ddm = *ddmVoid;
	// printf("%p\n\n", ddm);
	free((ddm)->T);
	free(ddm);
}

value* GetFirstDistanceMatrix(){

	dm->iterator=0;
	return &(dm->data[dm->iterator]);
}

value* GetNextDistanceMatrix(){

	if(dm->iterator >= dm->dataNum-1)
		return NULL;
	return &(dm->data[++dm->iterator]);
}

value* GetFirstQueryDistanceMatrix(){

	dm->qIterator=0;
	return &(dm->queries[dm->qIterator]);
}


value* GetNextQueryDistanceMatrix(){

//	Report();
	if(dm->qIterator >= dm->queryNum-1){
		printf("Epistrefw NULL guys!\n");
		return NULL;
	}
	return &(dm->queries[++dm->qIterator]);
}

double DistanceMatrixDistance(value* x1, value* x2) {
	if ( *(int*)x1->content >= dm->dataNum)
	{
		int temp =*(int*)x1->content; 
		temp -= dm->dataNum;
		double res =  dm->queryDist[ temp ][*(int*)x2->content];
		return res;
	}
	else if ( *(int*)x2->content >= dm->dataNum) {
		int temp = *(int*)x2->content;
		temp -= dm->dataNum;
		return dm->queryDist[temp ][*(int*)x1->content];
	}
	return dm->dataDist[*(int*) (x1->content)][*(int*) (x2->content)];
}

int EqualDistanceMatrix(value* x1, value* x2) {
	return DistanceMatrixDistance(x1, x2) == 0;
}

int InRangeDistanceMatrix(value* x1, value* x2, double r) {
	return DistanceMatrixDistance(x1, x2) <= r;
}

void PrintDistanceMatrix(value *x) {
	//printf("%s\n", x->name);
	int j;
	int index_t=*(int*)x->content;
	if(index_t < dm->dataNum){
		printf("ID: %s, Values: ",dm->data[index_t].name);
		for(j=0;j<dm->dataNum;j++){
			if(j == dm->dataNum /2)
				printf(" ... ");
			if(j> 3 && j< dm->dataNum -3)
				continue;
			int * index=dm->data[index_t].content;
			printf("%d ", (int) dm->dataDist[ *index][j]);
		}
		printf("\n");
	}
	else{
		index_t-= dm->dataNum;
		printf("ID: %s, Values: ",dm->queries[index_t].name);
		for(j=0;j<dm->dataNum;j++){
			if(j == dm->dataNum /2)
				printf(" ... ");
			if(j> 3 && j< dm->dataNum -3)
				continue;

			int * index=dm->queries[index_t].content;
			printf("%d ", (int) dm->queryDist[ *index - dm->dataNum][j]);
		}
		printf("\n");
	}
}

double square(double value) {
	return value * value;
}

// afth kanei init to configuration struct


double HiDistanceMatrixActual(value* x, value* x1, value* x2) {
	double dX1X2 = DistanceMatrixDistance(x1, x2);
	double res = square(DistanceMatrixDistance(x, x1))
			+ square(DistanceMatrixDistance(x, x2)) - square(dX1X2);
	res /= 2 * dX1X2;
	return res;
}

int HiDistanceMatrix(value* x, value* x1, value* x2, double t1) {
	double res = HiDistanceMatrixActual(x, x1, x2);
	return t1 <= res;
}

/**
 * get the item with name itemi
 * ex. for i = 1 get item1
 **/

void GetIthItem(int i, value* v) {
	v->content = dm->dataDist[i];
	v->name = dm->data[i].name;
}

/**
 * get the median of HiDistanceMatrixActual(x, x1, x2)
 * for every x in our dataset
 *
 **/
double GetMedian(value* x1, value* x2) {
	int i;
	value* x = malloc(sizeof(value));
	double* values = malloc(sizeof(double) * dm->dataNum);
	for (i = 0; i < dm->dataNum; ++i) {
		GetIthData(i, x);
		values[i] = HiDistanceMatrixActual(x, x1, x2);
	}

//	for (i = 0; i < dm->dataNum; ++i) {
//	}

//	return values[2];
//	return 0;
//
	double res = qselect(values,dm->dataNum,0);

	free(values);
	free(x);
	return res;
}

unsigned long long int GDistanceMatrix(value* x, void* ddmVoid) {
	int i;
	unsigned long long int res = 0;
	unsigned long long int initial = 1;
	confDistanceMatrix* ddm = ddmVoid;
	value* x1, *x2;
	x1 = malloc(sizeof(value));
	x2 = malloc(sizeof(value));
	GetIthData(ddm->index, x2);
	for (i = 0; i < dm->k; ++i) {
		GetIthData(i, x1);
		if (ddm->T[i] != ddm->T[i])
			ddm->T[i] = GetMedian(x1, x2);
		initial = HiDistanceMatrix(x, x1, x2, ddm->T[i]);
		res |= initial << i;
	}
	free(x1);
	free(x2);
	return res;
}




double BruteForceANNDistanceMatrix(value query, List nn){
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

/*
 *
 * mia malakia gia mena mono
 */
//void Print(char* temp) {
//	printf("%s  ", temp);
//}

void DestroyInputDistanceMatrix() {
	int i;
	for (i = 0; i < dm->dataNum; ++i)
		free(dm->dataDist[i]);
	//for (i = 0; i < dm->queryNum; ++i)
		//free(dm->queryDist[i]);
	//for (i = 0; i < dm->queryNum; ++i)
	//{
		//free(dm->queries[i].name);
		//free(dm->queries[i].content);
	//}
	//for (i = 0; i < dm->dataNum; ++i)
	//{
		//free(dm->data[i].name);
		//free(dm->data[i].content);
	//}
	//free(dm->queryDist);
	free(dm->dataDist);
	//free(dm->data);
	//free(dm->queries);
	free(dm);
}

void DestroyValueDistanceMatrix(void* vVoid) {
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




/*{{{  data*/
void ReadDataDistanceMatrix(value** points, FILE* inputFile, int k) {
	List tempList;

	size_t lineSize = 0;

	char* lineBuff = NULL;
	char* temp;
	char delims[5] = "\t ,\n";
	char* token = NULL;
	int num = 0;
	int i;
	int line = 0;

	dm = malloc(sizeof(datadistanceMatrix));
	dm->data=NULL;
	dm->dataDist=NULL;
	dm->dataNum=0;
	dm->queries=NULL;
	dm->iterator=0;
	dm->queryDist=NULL;
	dm->queryNum=0;
	dm->qIterator=0;
	dm->k = k;
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

	SetDataSize(num);
	*points = malloc(num*sizeof(value));
	value* newData = *points;

	char* tempVal = GetFirst(tempList);
	for (i = 0; i < num; i++) {
		newData[i].name = malloc(LABELSIZE * sizeof(char));
		memset(newData[i].name,'\0',LABELSIZE);
		if(tempVal ==NULL)
			continue;
		strcpy(newData[i].name, tempVal);
		tempVal=GetNext(tempList);
	}

	dm->dataDist = malloc(num * sizeof(double*));
	while (getline(&lineBuff, &lineSize, inputFile) != -1) {

		dm->dataDist[line] = malloc(num * sizeof(double));
		newData[line].content=malloc(sizeof(int));
		memcpy(newData[line].content,&line,sizeof(int));


		token = strtok(lineBuff, delims);
		if (token == NULL)
			break;
		for (i = 0; i < num; i++) {
			dm->dataDist[line][i] = atoi(token);
			token = strtok(NULL, delims);
		}
		line++;
	}
	dm->dataNum=line;
	data.tableSize = dm->dataNum;
	DestroyList(&tempList);

	free(lineBuff);


}

int GetQueryNum(FILE* fp){

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


void printEverything(value* data){


	int i;
	int j;

	 //printf("Printing INPUT DATA:\n");

    //for(i=0;i<GetDataSize();i++){
        //if(!(i%100==0))
        //    continue;
        printf("ID: %s, Values: ",data->name);
        for(j=0;j<GetDataSize();j++){
            int * index=data->content;
            printf("%d ", (int) dm->dataDist[ *index][j]);
        }
        printf("\n\n");
    //}
	

	//print Queries
	/*printf("\n\nPrinting Queries:\n");

	for(i=0;i<dm->queryNum;i++){
		printf("ID: %s, Values: ",dm->queries[i].name);
		for(j=0;j<dm->dataNum;j++){
			int * index=dm->queries[i].content;
			printf("%d ", (int)dm->queryDist[(*index) - dm->dataNum][j]);
		}
	}*/

}


void ReadQueriesDistanceMatrix(FILE* inputFile) {


	size_t lineSize = 0;

	char* lineBuff = NULL;

	int i=0;
	char delims[6]="\t \n";
	char* token=NULL;
	double radius;
	char secBuff[256];

	dm->queryNum=GetQueryNum(inputFile);
	//dm->queryNum--;
//	printf("Tha diavasw %d queries\n",dm->queryNum);
	getline(&lineBuff, &lineSize, inputFile);
	//scanf gia to radius
	if (sscanf(lineBuff, "%s %lf", secBuff, &radius) < 2) {
		perror("Malformed file - Exiting...\n");
		exit(-1);
	}
	data.radius = radius;
	free(lineBuff);
	lineSize=0;
	//printf("To vlepw?\n");

	dm->queries= malloc(dm->queryNum* sizeof(value));
//	printf("%d\n",dm->queryNum);
//	getchar();
	dm->queryDist = malloc(dm->queryNum * sizeof(double*));

	while (getline(&lineBuff, &lineSize, inputFile) != -1) {

		token = strtok(lineBuff, delims);

		//printf("%s\t", token);

//		dm->data=malloc(sizeof(value));
		dm->queries[i].name=malloc(LABELSIZE*sizeof(char));

		strcpy(dm->queries[i].name,token);

		dm->queries[i].content=malloc(sizeof(int));
		int temp=dm->dataNum+i;
		*(int*)(dm->queries[i].content)=temp;
		dm->queryDist[i]=malloc(dm->dataNum * sizeof(double));
		int j=0;
		double* content=dm->queryDist[i];
		for(j=0;j<dm->dataNum;j++){
			token = strtok(NULL, delims);
			content[j] = atof(token);
			//printf("%d ", * (int*)dm->queries[i].content);
		}
		i++;
	}
	free(lineBuff);

	//printEverything();
}


/*}}}*/



