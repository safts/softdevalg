#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bruteforce.h"
#include "data.h"
#include "list.h"
#include "dataHamming.h"


dataIF data;
int main(){
    
    value* temp=NULL;    

    FILE* inputFile=NULL;
	size_t lineSize=0;
	size_t lineLen=0;


	char* lineBuff=NULL;
	char secBuff1[256];
	char secBuff2[256];

	int i=0;

	memset(secBuff1,'\0',sizeof(secBuff1));
	memset(secBuff2,'\0',sizeof(secBuff2));

	if((inputFile=fopen("./inputFiles/DataHamming.csv","r"))==NULL){
		perror("File specified could not be opened\n");
	}

	if((lineLen=getline(&lineBuff, &lineSize, inputFile))==-1) {
		//something went wrong
		free(lineBuff);
		lineBuff=NULL;
		exit(-1);
	}

	if(sscanf(lineBuff,"%s %s",secBuff1,secBuff2)<2){
		perror("Malformed file - Exiting...\n");
		exit(-1);
	}

	free(lineBuff);
	lineBuff=NULL;
	lineSize=0;
	lineLen=0;

	if(strcmp(secBuff2,"hamming")==0){

		data.distance=Hamming;
		data.inRange=InRangeHamming;
		data.print=PrintDataHamming;
		InitList(&data.values,sizeof(value),data.print,data.distance);

		while((lineLen=getline(&lineBuff, &lineSize, inputFile))!=-1){

            if(i==20)
                break;            
            
			memset(secBuff1,'\0',sizeof(secBuff1));
			memset(secBuff2,'\0',sizeof(secBuff2));

			if(sscanf(lineBuff,"%s %s",secBuff1,secBuff2)<2){
				printf("Error! Input file: line %d\n",i+1);
				perror("Malformed file - Exiting...\n");
				exit(-1);
			}
			

			temp=malloc(sizeof(value));
			temp->name=malloc((strlen(secBuff1)+1)*sizeof(char));
			strcpy(temp->name,secBuff1);
			temp->name[strlen(secBuff1)]='\0';
			temp->content=malloc(sizeof(bitStr));
			bitStr bit=CharTobitStr(secBuff2);

            memcpy(temp->content,&bit, sizeof(bitStr));

			InsertValueList(data.values,temp);
            
			i++;
		}
		
	}


	fclose(inputFile);
    value *query = malloc(sizeof(value));
  
    bitStr bit2 = CharTobitStr("0010111010100100101010010001100000011001000101011111100001100100");
    query->name = malloc(strlen("query")+1);
    strcpy(query->name,"query");
    query->content=malloc(sizeof(bitStr));
    memcpy(query->content,&bit2,sizeof (bitStr));

    PrintList(data.values);
    PrintDataHamming(query);
    List nn;
    InitList(&nn,sizeof(value),data.print,data.distance);
    BruteForceANN(data.values,*query, nn);
    printf("\n Nearest Neighbours\n");
    PrintList(nn);
    DeleteAllFromList(nn);
    BruteForceInRange(data.values,*query, 8, nn);
    printf("\nNeighbours in Range\n");
    PrintList(nn);
    printf("Gia ton kwnno\n");
    return 0;
}
