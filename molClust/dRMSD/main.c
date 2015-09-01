#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <getopt.h>

#include "./implMolConf.h"
#include "./dataStruct/data.h"
#include "./lapackeIF.h"

#include "./cluster/initialization/kMedoidPlusPlus.h"
#include "./cluster/assignment/pam.h"
#include "./cluster/update/claransUpdate.h"
#include "./cluster/update/lloyds.h"


#include "./input/dataMolecule.h"
dataIF data;

void print_matrix( char* desc, int m, int n, double* a, int lda );


int main(int argc, char *argv[]){
	char inputFile[256], outputFile[256], confFile[256];
	int inputFlag = 0, outputFlag = 0, confFlag = 0;
	FILE* input=NULL;
	FILE* output=NULL;
	FILE* config=NULL;
	srand(time(NULL));

	int confNums[5];
	int r = 365;
	int choice;
	while (1)
	{
		static struct option long_options[] =
		{
			/* Use flags like so:
			{"verbose",	no_argument,	&verbose_flag, 'V'}*/
			/* Argument styles: no_argument, required_argument, optional_argument */
			{"version", no_argument,	0,	'v'},
			{"help",	no_argument,	0,	'h'},
			{"inputFile", required_argument, 0, 'i'},
			{"conf", required_argument, 0, 'c'},
			{"output", required_argument, 0, 'o'},
			{"rSize", required_argument, 0, 'r'},
			{"default", no_argument, 0, 'd'},
			{0,0,0,0}
		};
	
		int option_index = 0;
	
		/* Argument parameters:
			no_argument: " "
			required_argument: ":"
			optional_argument: "::" */
	
		choice = getopt_long( argc, argv, "vhi:c:o:r:d",
					long_options, &option_index);
	
		if (choice == -1)
			break;
	
		switch( choice )
		{
			case 'v':
				printf("beta version\n");
				
				break;
	
			case 'h':
				printf("ussage <executable> –d <inputFile file> –c <configuration file> -o <output file> -I <init fun> -A <assign fun> -U <update fun> --complete\n");
				
				break;
			case 'd':
				strcpy(inputFile, "./inputFiles/bio_small_input.dat");
				strcpy(outputFile, "./inputFiles/conform.dat");
				strcpy(confFile,"./inputFiles/cluster_5.conf" );
				inputFlag = 1;
				outputFlag = 1;
				confFlag = 1;

				break;
			case 'i':
				strcpy(inputFile, optarg);
				inputFlag = 1;
				break;
			case 'c':
				strcpy(confFile, optarg);
				confFlag = 1;

				break;
			case 'o':
				strcpy(outputFile, optarg);
				outputFlag = 1;

				break;
			case 'r':
				r = atoi(optarg);;

				break;
	
			case '?':
				/* getopt_long will have already printed an error */
				break;
	
			default:
				/* Not sure how to get here... */
				return EXIT_FAILURE;
		}
	}
	
	/* Deal with non-option arguments here */
	if ( optind < argc )
	{
		while ( optind < argc )
		{
			
		}
	}
	if ( !inputFlag )
	{
		printf("Give me an input file\n");
		scanf("%s", inputFile);
	}
	if ( !confFlag )
	{
		printf("Give me an conf file\n");
		scanf("%s", confFile);
	}
	if ( !outputFlag )
	{
		printf("Give me an output file\n");
		scanf("%s", outputFile);
	}
	printf("inputFile %s, output %s, confingure %s,r = %d \n",inputFile, outputFile, confFile, r );


	if((input=fopen(inputFile,"r")) == NULL){
		perror(inputFile);
		exit(-1);
	}
	if((output=fopen(outputFile,"w")) == NULL){
		perror(outputFile);
		exit(-1);
	}
	
	if((config=fopen(confFile,"r")) == NULL){
		perror(confFile);
		exit(-1);
	}

	//ReadDataMolecule(NULL, input);
	SpecifyDatasetMolConf("molecule");

	ParseConfig(config,confNums);
	parseData(input, r);
	void (*init)() = KMedoidPlusPlusInit;
	double (*assigment)() = PamAssign;
	int (*update)() = ClaransUpdate;
	int kCluster = 19;
	//int j = 0;
	int j = 0;



	init(kCluster);

	FirstAssignment();

	assigment(confNums[1],confNums[2]);

	j = 0;
	while(update(assigment,confNums)){
		fprintf(output,"Update loop #%d\n",j);
		printf("Update loop #%d\n",j);
		j++;
	}
	PrintClusters(output);
	double silhouette=Silhouette(output);
	fprintf(output, "%d\n%f\n",kCluster,silhouette );
	
	PrintConform(output);
	data.destroyInput();
	DestroyData();

	
	printf("Silhouette: %f\n",silhouette);
	fclose(input);
	fclose(output);
	fclose(config);


	return 0;
}


void PotUnitTestMult(){
	double x[5*3] = { 1, 2, 3,
				      4, 5, 6,
				      7, 8, 9,
				      10,11,12,
				      13,14,15};
	double y[5*3] = { 1, 0, 0,
				      0, 1, 0,
				      0, 0, 1,
				      0, 0, 0,
				      0, 0, 0};
	double c[3*3];
	MultiMatr(x, y , c,5 ,3 );
	int i,j;
	for (i = 0; i < 3; ++i){
		for (j = 0; j < 3; ++j)
			printf("%f|",c[i*3 + j]);
		printf("\n");
	}

}

void PotUnitTestSVD(){

	double M[3*3] = { 1,0,-1,
					 -2,1,4,
					  1,1,1};


	double U[3*3];
	double S[3];
	double V[3*3];

	CalcSVD(M, 3, 3, U, S, V);
	print_matrix("U", 3, 3, U, 3);
	print_matrix("V", 3, 3, V, 3);
	print_matrix("S", 3, 1, S, 1);

}

void print_matrix( char* desc, int m, int n, double* a, int lda ) {
        int i, j;
        printf( "\n %s\n", desc );
        for( i = 0; i < m; i++ ) {
                for( j = 0; j < n; j++ ) printf( " %f", a[i*lda+j] );
                printf( "\n" );
        }
}

