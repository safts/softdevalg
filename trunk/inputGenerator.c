#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "dataHamming.h"
#include "dataEuclidean.h"
#include "dataDistanceMatrix.h"
#include "random.h"

#define BUFFERSIZE_I 256
#define LABELSIZE_I 15

char* mdHammingInput="@metric_space hamming";

char* mdVectorInput="@metric_space vector";
char* mdEuclideanMetric="@metric euclidean";

char* mdDistanceMatrix="@metric_space matrix";


char* generateInputHamming(int valuesNum,int numOfClust,
		double seed,char* name){

	FILE* inputFile=NULL;
	char* buffer=malloc(BUFFERSIZE_I*sizeof(char));
	int i,j;
//	int digits=getDigits(valuesNum);

	memcpy(buffer,"\0",256*sizeof(char));
	sprintf(buffer,"inputHamming_auto_%d.csv",valuesNum);
	inputFile=fopen(buffer,"w");

	if(name==NULL){
		name=malloc(LABELSIZE_I*sizeof(char));
		strcpy(name,"item");
	}

	//printf("%s\n",mdHammingInput);
	fprintf(inputFile,"%s\n",mdHammingInput);
	for(i=0;i<valuesNum;i++){
		char bitString[65];
		memset(bitString,'\0',65*sizeof(char));
		for(j=0;j<64;j++){
			if(rand()%2 == 1)
				bitString[j]='1';
			else
				bitString[j]='0';
		}
		//printf("%s%d\t %s\n",name,i,bitString);
		fprintf(inputFile,"%s%d\t %s\n",name,i,bitString);
	}

	fclose(inputFile);
	return buffer;
}

char* generateQueriesHamming(int valuesNum,	int numOfClust,
		double seed,char* name,double radius){


	FILE* inputFile=NULL;
	char* buffer=malloc(BUFFERSIZE_I*sizeof(char));
	int i,j,k;
//	int digits=getDigits(valuesNum);

	memcpy(buffer,"\0",256*sizeof(char));
//	printf("%d\n",valuesNum);
	sprintf(buffer,"queriesHamming_auto_%d.csv",valuesNum);
	inputFile=fopen(buffer,"w");

	if(name==NULL){
		name=malloc(LABELSIZE_I*sizeof(char));
		strcpy(name,"item");
	}

	//printf("Radius: %f\n",radius);
	fprintf(inputFile,"Radius: %f\n",radius);
	for(i=0;i<valuesNum;i++){
		char bitString[65];
		memset(bitString,'\0',65*sizeof(char));
		for(j=0;j<64;j++){
			if(rand()%2 == 1)
				bitString[j]='1';
			else
				bitString[j]='0';
		}
		//printf("%s%d\t %s\n",name,i,bitString);
		fprintf(inputFile,"%s%d\t %s\n",name,i,bitString);
	}

	fclose(inputFile);
	return buffer;
}

char* generateInputEuclidean(int valuesNum, int dimensions,
		int neighborhoods ,double spread, char* name){


	FILE* inputFile=NULL;
	char* buffer=malloc(BUFFERSIZE_I*sizeof(char));
	int i,j;

	memcpy(buffer,"\0",256*sizeof(char));
	sprintf(buffer,"inputEuclidean_auto_%d.csv",valuesNum);
	inputFile=fopen(buffer,"w");

	if(name==NULL){
		name=malloc(LABELSIZE_I*sizeof(char));
		strcpy(name,"item");
	}

	//printf("%s\n",mdVectorInput);
	//printf("%s\n",mdEuclideanMetric);

	fprintf(inputFile,"%s\n",mdVectorInput);
	fprintf(inputFile,"%s\n",mdEuclideanMetric);

	for(i=0;i<valuesNum;i++){
		//printf("%s%d\t",name,i);
		fprintf(inputFile,"%s%d\t",name,i);
		for(j=0;j<dimensions;j++){
			double coord=GetNormal(0.0,spread);
			fprintf(inputFile,"%f",coord);
			//printf("%f",coord);
			if(j<dimensions-1){
				//printf("\t");
				fprintf(inputFile,"\t");
			}
		}
		//printf("\n");
		fprintf(inputFile,"\n");
	}

	fclose(inputFile);
	return buffer;

}

char* generateQueriesEuclidean(int valuesNum, int dimensions,
		int neighborhoods ,double spread, char* name, double radius){


	FILE* inputFile=NULL;
	char* buffer=malloc(BUFFERSIZE_I*sizeof(char));
	int i,j,k;

	memcpy(buffer,"\0",256*sizeof(char));
	sprintf(buffer,"queriesEuclidean_auto_%d.csv",valuesNum);
	inputFile=fopen(buffer,"w");

	if(name==NULL){
		name=malloc(LABELSIZE_I*sizeof(char));
		strcpy(name,"item");
	}

	//printf("Radius: %f\n",radius);
	fprintf(inputFile,"Radius: %f\n",radius);

	for(i=0;i<valuesNum;i++){
		//printf("%s%d\t",name,i);
		fprintf(inputFile,"%s%d\t",name,i);
		for(j=0;j<dimensions;j++){
			double coord=GetNormal(0.0,spread);
			fprintf(inputFile,"%f",coord);
			//printf("%f",coord);
			if(j<dimensions-1){
				//printf("\t");
				fprintf(inputFile,"\t");
			}
		}
		//printf("\n");
		fprintf(inputFile,"\n");
	}

	fclose(inputFile);
	return buffer;

}


char* generateInputDistanceMatrix(int valuesNum, double spread,char* name){

	FILE* inputFile=NULL;
	char* buffer=malloc(BUFFERSIZE_I*sizeof(char));
	int i,j;
	int skipped=0;
	memcpy(buffer,"\0",256*sizeof(char));
	sprintf(buffer,"inputDistanceMatrix_auto_%d.csv",valuesNum);
	inputFile=fopen(buffer,"w");

	if(name==NULL){
		name=malloc(LABELSIZE_I*sizeof(char));
		strcpy(name,"item");
	}
	//printf("%s\n@items\t",mdDistanceMatrix);
	fprintf(inputFile,"%s\n@items\t",mdDistanceMatrix);

	for(i=0;i<valuesNum;i++){
		//printf("%s%d\t",name,i);
		if(i<valuesNum-1)
			fprintf(inputFile,"%s%d\t",name,i);
		else
			fprintf(inputFile,"%s%d",name,i);

	}
	//printf("\n");
	fprintf(inputFile,"\n");
	int** tempArray=malloc(valuesNum*sizeof(int*));
	for(i=0;i<valuesNum;i++)
		tempArray[i]=malloc(valuesNum*sizeof(int));

	for(i=0;i<valuesNum;i++){
		for(j=0;j<valuesNum;j++){
			if(j == i)
				tempArray[i][j]=0;
			else{
				int randVal=0;
				while((randVal=abs((int)GetNormal(spread,spread)))==0)
					skipped++;
				tempArray[i][j]=tempArray[j][i]=randVal;

			}
			//printf("%d\t",tempArray[i][j]);
			if(j<valuesNum-1)
				fprintf(inputFile,"%d\t",tempArray[i][j]);
			else
				fprintf(inputFile,"%d",tempArray[i][j]);

		}
		//printf("\n");
		fprintf(inputFile,"\n");
	}
	for(i=0;i<valuesNum;i++)
		free(tempArray[i]);
	free(tempArray);

	fclose(inputFile);
	return buffer;
}

char* generateQueriesDistanceMatrix(int valuesNum, int points,double spread,
		char* name,double radius){

	FILE* inputFile=NULL;
	char* buffer=malloc(BUFFERSIZE_I*sizeof(char));
	int i,j;
	int skipped=0;
	memcpy(buffer,"\0",256*sizeof(char));
	sprintf(buffer,"queriesDistanceMatrix_auto_%d.csv",valuesNum);
	inputFile=fopen(buffer,"w");

	if(name==NULL){
		name=malloc(LABELSIZE_I*sizeof(char));
		strcpy(name,"item");
	}
	//printf("Radius: %f\n",radius);
	fprintf(inputFile,"Radius: %f\n",radius);


	for(i=0;i<valuesNum;i++){

		//printf("%s%d\t",name,i);
		fprintf(inputFile,"%s%d\t",name,i);

		for(j=0;j<points;j++){

			int randVal=0;
			while((randVal=abs((int)GetNormal(spread,spread)))==0)
				skipped++;
			//printf("%d\t",randVal);

			if(j<points-1)
				fprintf(inputFile,"%d\t",randVal);
			else
				fprintf(inputFile,"%d",randVal);
		}
		//printf("\n");
		fprintf(inputFile,"\n");
	}
	fclose(inputFile);
	return buffer;
}

int main(int argc, char* argv[]){

	srand((unsigned)time(NULL));

	int select=4;
	int valuesNum=1000;
	int c;
	opterr = 0;

	while ((c = getopt (argc, argv, "s:N:")) != -1)
		switch (c)
		{
			case 's':
				select = atoi(optarg);
				break;
			case 'N':
				valuesNum = atoi(optarg);
				break;
			case '?':
				printf("wrong input\n");
				return 1;
			default:
				printf("abort\n");
				//abort ();
		}


	printf("%d\n",valuesNum);

	char* temp1=NULL;
	char* temp2=NULL;

	//if(argc <3 ){
		//select=4;
		//valuesNum=1000;
	//}

	if(select==1){
		temp1=generateInputHamming(valuesNum,0,0,"item");
		temp2=generateQueriesHamming(valuesNum/10,0,0,"query",5.0);
		free(temp1);
		free(temp2);
	}
	else if(select==2){
		temp1=generateInputEuclidean(valuesNum,8,0,40,"item");
		temp2=generateQueriesEuclidean(valuesNum/10,8,0,40,"query",5.0);
		free(temp1);
		free(temp2);
	}
	else if(select==3){
		temp1=generateInputDistanceMatrix(valuesNum,40,"item");
		temp2=generateQueriesDistanceMatrix(valuesNum/10,valuesNum,40,"query",5.0);
		free(temp1);
		free(temp2);
	}
	else{
		temp1=generateInputHamming(valuesNum,0,0,"item");
		temp2=generateQueriesHamming(valuesNum/10,0,0,"query",5.0);
		free(temp1);
		free(temp2);
		temp1=generateInputEuclidean(valuesNum,8,0,40,"item");
		temp2=generateQueriesEuclidean(valuesNum/10,8,0,40,"query",5.0);
		free(temp1);
		free(temp2);
		temp1=generateInputDistanceMatrix(valuesNum,40,"item");
		temp2=generateQueriesDistanceMatrix(valuesNum/10,valuesNum,40,"query",5.0);
		free(temp1);
		free(temp2);
	}


	return 0;
}


