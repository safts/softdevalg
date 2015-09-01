#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>

#include "data.h"
#include "LSH.h"

//#include "dataFunction.h"
#include "dataHamming.h"
#include "dataEuclidean.h"
#include "dataDistanceMatrix.h"


void test(value *v , FILE* file){
	if(v==NULL || v->name == NULL)
		fprintf(file,"NULL");
	fprintf(file, "%s\n",v->name);
}

void Print(value* v){

	if(v==NULL || v->name == NULL)
		printf("NULL");
	printf("%s",v->name);
}

int GetClockTimeInNanoSec()
{
	struct timeval t2; gettimeofday(&t2,NULL);
	return t2.tv_sec+t2.tv_usec;
}

void PrintTime(int milli_sec,FILE* file)
{
	int v=milli_sec;
	int hours=v/(1000*60*60); v%=(1000*60*60);
	int minutes=v/(1000*60); v%=(1000*60);
	int seconds=v/1000000; v%=1000000;
	int milli_seconds=v/1000; v%=1000;;
	int nano_seconds = v;
	int first=1;
	fprintf(file,"%d[", milli_sec);
	if(hours) {if(!first) fprintf(file,":"); fprintf(file,"%dh", hours); first=0;}
	if(minutes) {if(!first) fprintf(file,":"); fprintf(file,"%dm", minutes); first=0;}
	if(seconds) {if(!first) fprintf(file,":"); fprintf(file,"%ds", seconds); first=0;}
	if(milli_seconds) {if(!first) fprintf(file,":"); fprintf(file,"%dms", milli_seconds); first=0;}
	if(nano_seconds) {if(!first) fprintf(file,":"); fprintf(file,"%dns", nano_seconds); first=0;}
	fprintf(file,"]");
}


dataIF data;


int main(int argc, char *argv[]){
	//double radius;
	int k = 4, L = 5;
//	char *output = "out.txt", *input = "in.txt", *query = "query.txt";
	char output[512];
	char input[512];
	char query[512];
	int outputFlag = 0;
	int inputFlag = 0;
	int queryFlag = 0;

	FILE* inputFile;
	FILE* queryFile;
	FILE* outputFile;

	size_t lineSize=0;
	size_t lineLen=0;

	LSH* lsh;

	char* lineBuff=NULL;
	char secBuff1[256];
	char secBuff2[256];

	srand((unsigned)time(NULL));

	memset(input,'\0',sizeof(input));
	memset(output,'\0',sizeof(output));
	memset(query,'\0',sizeof(query));

	int c;
	opterr = 0;

	while ((c = getopt (argc, argv, "k:L:d:o:q:")) != -1)
		switch (c)
		{
			case 'k':
				k = atoi(optarg);
				break;
			case 'L':
				L = atoi(optarg);
				break;
			case 'd':
				inputFlag = 1;
				strcpy(input,optarg);
				break;
			case 'o':
				outputFlag = 1;
				strcpy(output,optarg);
				break;
			case 'q':
				queryFlag = 1;
				strcpy(query,optarg);
				break;
			case '?':
				printf("wrong input\n");
				return 1;
			default:
				printf("abort\n");
				//abort ();
		}

	printf("input %s, output %s, query %s, k = %d, L = %d\n",input, output, query, k, L);

	printf("Welcome.\n");
	if ( !inputFlag)
	{
		printf("Please input the data file path:\n");
		scanf("%s", input);
	}


	memset(secBuff1,'\0',sizeof(secBuff1));
	memset(secBuff2,'\0',sizeof(secBuff2));

	if((inputFile=fopen(input,"r"))==NULL){
		perror("File specified could not be opened(inp)\n");
		exit(-1);
	}

	
	specifyDataset(inputFile);

	parseData(inputFile,k);
	data.print=test;
	//data.readData(inputFile, k);
	
	BuildLSH(&lsh,L,k);

	if ( !queryFlag)
	{
		printf("Please input the query file path:\n");
		scanf("%s", query);
	}
	if ( !outputFlag)
	{
		printf("Please input the output file path:\n");
		scanf("%s", output);
	}

	if((outputFile=fopen(output,"w"))==NULL){
		perror("File specified could not be opened(outp)\n");
		exit(-1);
	}
	if((queryFile=fopen(query,"r"))==NULL){
		perror("File specified could not be opened(quer)\n");
		exit(-1);
	}
	
	data.readQueries(queryFile);

	value* q=data.getFirstQuery();

	double avTimeLSH=0.0,avDistLSH=0.0,avTimeBrut=0.0;
	int stats=1;
	int dataSet=0;
	while(q!=NULL){

		int tLSH=0,tTRUE=0;

		double distance=0.0;
		double realDist=0.0;

		List nn;
		List allRange,resList;

		value *r = NULL;

		tLSH=GetClockTimeInNanoSec();
		distance = SearchANN(lsh, *q, &r);
		tLSH=GetClockTimeInNanoSec()-tLSH;

		avTimeLSH+=tLSH;

		InitList(&nn,sizeof(value),test,data.distance, NULL);

		tTRUE = GetClockTimeInNanoSec();
		//realDist = data.bruteForce(*q,nn);
		tTRUE = GetClockTimeInNanoSec()-tTRUE;

		avTimeBrut+=tTRUE;
		avDistLSH+=(distance-realDist);

		//PrintList(nn);

		InitList(&allRange,sizeof(value),test,data.distance, NULL);
		InitList(&resList,sizeof(value),test,data.distance, NULL);

		SearchRange(lsh, *q, data.radius, allRange);
		DeduplicateList(allRange,resList);


		fprintf(outputFile,"Query: %s\n",q->name);

		fprintf(outputFile,"R-near Neighbors:\n");
		PrintListFile(resList,outputFile);

		if (r == NULL)
			fprintf(outputFile,"NULLLLL\n");
		else{
			fprintf(outputFile,"Nearest Neighbor: ");
			data.print(r,outputFile);
		}

		fprintf(outputFile,"distanceLSH: %f \n distanceTRUE: %f\n",distance ,realDist);
		fprintf(outputFile,"tLSH: ");
		PrintTime(tLSH,outputFile);
		fprintf(outputFile,"\n");
		fprintf(outputFile,"tTRUE: ");
		PrintTime(tTRUE,outputFile);
		fprintf(outputFile,"\n");

		DestroyList( &allRange);
		DestroyList( &resList);
		DestroyList( &nn);
		q=data.getNextQuery();
		dataSet++;
	}
	/**/

	if(stats){
		printf("k\tL\tAvTLSH\t\tAvTReal\t\tAvLSHDeviation(dist)\n");
		printf("%d\t%d\t%f\t%f\t%f\n",k,L,avTimeLSH/(double)dataSet,avTimeBrut/(double)dataSet,avDistLSH/(double)dataSet);
	}

	DestroyLSH(&lsh);
	data.destroyInput();

	fclose(inputFile);
	fclose(outputFile);
	fclose(queryFile);

	free(lineBuff);
	return 0;
}


