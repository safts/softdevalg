#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

typedef enum{true=1,false=0} bool;

#define PATHSIZE 512

dataIF data;

void printHelp(int level){

	if(level==0){

		printf("To use this LSH algorithm implementation you need to:\n");
		printf("---> 1. Specify the input data\n");
		printf("---> 2. Specify the output file\n");
		printf("---> 3. Run the algorithm\n");
//		printf("If at any time you need help, enter \"h\"\n");
	}
	else if(level==1){
		printf(">|Specify the data the algorithm needs:\n");
		printf(">|---> 1. Specify input files\n");
		printf(">|---> 2. Generate automated input\n");
		printf(">|---> -1. Return to the previous options\n");
//		printf("Specify your option by entering a number below:\n");

	}
	else if(level==2){
		printf(">|>|Give the path of the input files:\n");
		printf(">|>|---> 1. Specify input data file\n");
		printf(">|>|---> 2. Specify the query file\n");
		printf(">|>|---> -1. Return to the previous options\n");
	}
	printf("Specify your option by entering a number below, "
			"or enter 9 for help:\n");
//	printf(">");
//	while(level>0){
//		printf("|>");
//		level--;
//	}

}

void CLI(){

	size_t lineSize = 0;
	char* lineBuff = NULL;
	int option0=0, option1=0, option2=0;

	char* inputFile=NULL;
	char* queryFile=NULL;
	char* outputFile=NULL;

	char delims[4]="\n\t ";
	int error=0;


	bool inputOK=false;
	bool inputDataOK=false;
	bool inputQueriesOK=false;

	bool outputOK=false;

	printf("Welcome.\n");
	printHelp(0);

	while(true){
		printf(">");
		if(getline(&lineBuff, &lineSize, stdin)==-1){
			printf("Something went wrong, exiting...\n");
			exit(-1);
		}
		error=sscanf(lineBuff,"%d\n",&option0);
		if(error <= 0){
			printf("ERROR: Option not valid.(%s) \n"
					"Please try again or enter 9 for help\n", lineBuff);
			continue;
		}
		if(option0 == 1){

			printHelp(1);

			while(true){
				printf(">|>");
				if(getline(&lineBuff, &lineSize, stdin)==-1){
					printf("Something went wrong, exiting...\n");
					exit(-1);
				}
				error=sscanf(lineBuff,"%d\n",&option1);
				if(error<=0){
					printf("ERROR: Option not valid.(%s) \n"
							"Please try again or enter 9 for help\n", lineBuff);
					continue;
				}
				if(option1==1){
					printHelp(2);
					while(true){

						printf(">|>|>");
						if(getline(&lineBuff, &lineSize, stdin)==-1){
							printf("Something went wrong, exiting...\n");
							exit(-1);
						}
						error=sscanf(lineBuff,"%d\n",&option2);
						if(error <=0){
							printf("ERROR: Option not valid.(%s) \n"
									"Please try again or enter 9 for help\n", lineBuff);
							continue;
						}
						if(option2==1){
							printf("Specify the path for the input data file:\n");
							printf(">|>|>|>");
							if(getline(&lineBuff, &lineSize, stdin)==-1){
								printf("Something went wrong, exiting...\n");
								exit(-1);
							}
							inputFile=strtok(lineBuff,delims);
							printf("Input file set as: %s Continue...\n",inputFile);
							inputDataOK=true;
							printHelp(2);
						}
						else if(option2==2){
							printf("Specify the path for the input queries file:\n");
							printf(">|>|>|>");
							if(getline(&lineBuff, &lineSize, stdin)==-1){
								printf("Something went wrong, exiting...\n");
								exit(-1);
							}
							queryFile=strtok(lineBuff,delims);
							printf("Query file set as: %s Continue...\n",queryFile);
							inputQueriesOK=true;
							printHelp(2);
						}
						else if(option2==-1){
							break;
						}
						else if(option2==9){
							printHelp(1);
						}

						if( inputDataOK==true && inputQueriesOK==true){
							inputOK=true;
							break;
						}
					}

				}
				else if(option1==2){
					printf("NOT IMPLEMENTED\n");

				}
				else if(option1==-1){
					break;
				}
				else if(option1==9){
					printHelp(1);
				}
			}
		}
		else if(option0 == 2 ){
			printf("Specify the path for the output data file:\n");
			printf(">|>");
			if(getline(&lineBuff, &lineSize, stdin)==-1){
				printf("Something went wrong, exiting...\n");
				exit(-1);
			}
			outputFile=strtok(lineBuff,delims);
			printf("Output file set as: %s Continue...\n",outputFile);
			outputOK=true;
			printHelp(0);

		}
		else if(option0 == 3){
			if(!(inputOK==true) || !(outputOK==true)){
				printf("ERROR: You need to specify the input and output files "
						"before running LSH\n");
				continue;
			}
		}
		else if(option0==9){
			printHelp(0);
		}
		if( inputDataOK==true && inputQueriesOK==true)
			inputOK=true;
	}





}

int main(void){

	CLI();

	return 0;
}
