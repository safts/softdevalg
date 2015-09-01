#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataHamming.h"
#include "data.h"
#include "random.h"
#include "list.h"

extern dataIF data;

typedef struct hamming_t {
	List data;
	List queries;
	int k;

} dataHamming;

dataHamming* ham;  // prepei na to kanw malloc kai na to ftiaksw opws sto distm

typedef struct confHamming_t { int* randomMatrix; } confHamming;

void* InitStructHamming() {
	confHamming* ch = malloc(sizeof(confHamming));
	ch->randomMatrix = malloc((ham->k) * sizeof(int));
	GetKUniform(ham->k, 0, 64, ch->randomMatrix);
	return ch;
}

void DestroyStructHamming(void** chVoid) {
	confHamming* ch = *chVoid;
	free((ch)->randomMatrix);
	free(ch);
}

value* GetFirstHamming() { 
	return GetFirst(ham->data); 
}

value* GetNextHamming() { 
	return GetNext(ham->data); 
}

value* GetFirstQueryHamming() {
	return GetFirst(ham->queries);
}

value* GetNextQueryHamming() {
	return GetNext(ham->queries);
}

double Hamming(value* aptr, value* bptr) {

	bitStr a = *(bitStr*)(aptr->content);
	bitStr b = *(bitStr*)(bptr->content);
	int i = 0;
	double count = 0;
	for (i = 0; i < 64; i++) {
		if ((a & 1) != (b & 1)) {
			count++;
		}
		a >>= 1;
		b >>= 1;
	}
	return count;
}

double EqualHamming(value* a, value* b) { return Hamming(a,b)==0; }

int InRangeHamming(value* a, value* b, double R) {
	return Hamming(a, b) <= R;
}

void PrintHamming(bitStr* a) {
	int i;
	bitStr shift;
	bitStr initial = 1;
	for (i = 0; i < 64; ++i) {
		shift = initial << i;
		printf("%d", !!(*a & shift));
	}
	printf("\n");
}

void PrintDataHamming(value* v) {
	printf("ID: %8s, bitString: ", v->name);
	PrintHamming((v->content));
}

int HiHamming(bitStr* aptr, int pos) {
	bitStr a = *aptr;
	bitStr initial = 1;
	return (!!((a) & (initial << (pos))));
}

bitStr GHamming(value* value, void* cfhVoid) {
	confHamming* cfh = cfhVoid;
	bitStr res = 0;
	bitStr initial = 1;
	int i;
	for (i = 0; i < ham->k; ++i) {
		initial = HiHamming(value->content, cfh->randomMatrix[i]);
		res |= initial << i;
	}
	return res;
}

bitStr CharTobitStr(char* string) {
	bitStr total = 0;
	int i;
	for (i = 63; i > -1; i--) {
		total <<= 1;
		if (string[i] == '1') total ^= 1;
	}
	return total;
}

void ReadQueriesHamming(FILE* inputFile) {
	size_t lineSize = 0;
	size_t lineLen = 0;

	value* temp = NULL;

	char* lineBuff = NULL;
	char secBuff1[256];
	char secBuff2[256];

	int i = 0;
	double radius = 0.0;

	if ((lineLen = getline(&lineBuff, &lineSize, inputFile)) == -1) {
		// something went wrong
		free(lineBuff);
		lineBuff = NULL;
		exit(-1);
	}

	if (sscanf(lineBuff, "%s %lf", secBuff1, &radius) < 2) {
		perror("Malformed file - Exiting...\n");
		exit(-1);
	}
	data.radius = radius;

	free(lineBuff);
	lineBuff = NULL;
	lineSize = 0;
	lineLen = 0;

	memset(secBuff1, '\0', sizeof(secBuff1));
	memset(secBuff2, '\0', sizeof(secBuff2));

	InitList(&(ham->queries), sizeof(value), data.print, data.distance,
			data.destroyValue);

	while ((lineLen = getline(&lineBuff, &lineSize, inputFile)) != -1) {
		if (sscanf(lineBuff, "%s %s", secBuff1, secBuff2) < 2) {
			printf("Error! Input file: line %d\n", i + 1);
			perror("Malformed file - Exiting...\n");
			exit(-1);
		}

		temp = malloc(sizeof(value));
		temp->name = malloc((strlen(secBuff1) + 1) * sizeof(char));
		strcpy(temp->name, secBuff1);
		temp->name[strlen(secBuff1)] = '\0';
		temp->content = malloc(sizeof(bitStr));
		bitStr bit = CharTobitStr(secBuff2);
		memcpy(temp->content, &bit, sizeof(bitStr));
		InsertValueList(ham->queries, temp);
		i++;
		free(temp);
	}
	free(lineBuff);
	// printf("Inserted %d queries.. \n",i);
}

void ReadDataHamming(FILE* inputFile, int k) {
	size_t lineSize = 0;
	size_t lineLen = 0;

	value* temp = NULL;

	char* lineBuff = NULL;
	char secBuff1[256];
	char secBuff2[256];

	int i = 0;

	ham = malloc(sizeof(dataHamming));
	ham->k = k;

	memset(secBuff1, '\0', sizeof(secBuff1));
	memset(secBuff2, '\0', sizeof(secBuff2));

	InitList(&(ham->data), sizeof(value), data.print, data.distance,
			data.destroyValue);

	while ((lineLen = getline(&lineBuff, &lineSize, inputFile)) != -1) {
		memset(secBuff1, '\0', sizeof(secBuff1));
		memset(secBuff2, '\0', sizeof(secBuff2));

		if (sscanf(lineBuff, "%s %s", secBuff1, secBuff2) < 2) {
			printf("Error! Input file: line %d\n", i + 1);
			perror("Malformed file - Exiting...\n");
			exit(-1);
		}

		temp = malloc(sizeof(value));
		temp->name = malloc((strlen(secBuff1) + 1) * sizeof(char));
		strcpy(temp->name, secBuff1);
		temp->name[strlen(secBuff1)] = '\0';
		temp->content = malloc(sizeof(bitStr));
		bitStr bit = CharTobitStr(secBuff2);
		memcpy(temp->content, &bit, sizeof(bitStr));
		InsertValueList((ham->data), temp);
		i++;
		free( temp);
	}
	// printf("Inserted %d values.. \n",i);
	// PrintList(ham->data);
	data.tableSize = 1 << k;
	free(lineBuff);
}

double BruteForceANNHamming(value query, List nn) {
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

void BruteForceInRangeHamming(value query, double radius, List nn) {
	value* temp = data.getFirst(ham->data);

	while (temp != NULL) {
		if (data.inRange(temp, query, radius))
			InsertValueList(nn, temp);

		temp = data.getNext(ham->data);
	}
}

void FillRandomMatrix(int k, int* randomMatrix) {
	GetKUniform(k, 0, 64, randomMatrix);
}

void DestroyInputHamming() {
	DestroyList(&(ham->data));
	DestroyList( &(ham->queries));
	free(ham);
}

void DestroyValueHamming(void* vVoid) {
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


