#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <getopt.h>

#include "dataStruct/data.h"
#include "dataStruct/list.h"
#include "recommend.h"
#include "dataStruct/data.h"


dataIF data;



 unsigned long int GetClockTimeInMilliSec()
{
	struct timeval t2; 
	gettimeofday(&t2,NULL);
	//printf("%ld\t", t2.tv_usec);
	//printf("%ld\n", t2.tv_sec);
	return t2.tv_sec*1000000 + t2.tv_usec ;
	if(t2.tv_sec > t2.tv_usec)
		return (t2.tv_sec/1000)-(t2.tv_usec/1000);
	else
		return (t2.tv_usec/1000)-(t2.tv_sec/1000);
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
	fprintf(file,"]");
}

int main(int argc, char *argv[])
{

	FILE* input=NULL;
	FILE* output=NULL;
	FILE* cluster=NULL;
	FILE* config=NULL;
	char inputFile[256], outputFile[256], confFile[256];
	int inputFlag = 0, outputFlag = 0, confFlag = 0;
	int completFlag =0;
	int choice;
	input=fopen("./inputFiles/yahoo_music_small.dat","r");
	output=fopen("./out.dat","w");
	cluster=fopen("./clusters.dat","w");
	config=fopen("./inputFiles/cluster_5.conf","r");
	int confNums[5];

	int LLSH=5;
	int kLSH=4;
	int validate=0;
	int neighbors=25;
	int j;
	time_t tLSH,tClust,tValLSH,tValClust;

	srand((unsigned)time(NULL));
	while (1)
	{
		static struct option long_options[] =
		{
			/* Use flags like so:
			{"verbose",	no_argument,	&verbose_flag, 'V'}*/
			/* Argument styles: no_argument, required_argument, optional_argument */
			//{"version", no_argument,	0,	'v'},
			{"help",	no_argument,	0,	'h'},
			{"inputFile", required_argument, 0, 'd'},
			{"conf", required_argument, 0, 'c'},
			{"output", required_argument, 0, 'o'},
			{"validate", no_argument, 0, 'v'},
			{0,0,0,0}
		};
	
		int option_index = 0;
	
		/* Argument parameters:
			no_argument: " "
			required_argument: ":"
			optional_argument: "::" */
	
		choice = getopt_long( argc, argv, "vhd:c:o:I:A:U:a",
					long_options, &option_index);
	
		if (choice == -1)
			break;
	
		switch( choice )
		{
	
			case 'h':
				printf("ussage <executable> –d <inputFile file> –c <configuration file> -o <output file> --validate\n");
				
				break;
			case 'd':
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
			case 'v':
				validate = 1;

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
	printf("inputFile %s, output %s, confingure %s, validate = %d\n",inputFile, outputFile, confFile, validate );

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
	if((cluster=fopen("./clusters.dat","w")) == NULL){
		perror("./clusters.dat");
		exit(-1);
	}

	List results;
	InitList(&results,sizeof(Rating),NULL,data.distance,NULL);

	printf("Stage #0: Parsing data\n");

	SpecifyDatasetRecommend("cosine",kLSH);
	ParseConfig(config,confNums);
	parseData(input,confNums[1]);


	if(confNums[3] == 0){
		if(0.012*(confNums[0]*(GetDataSize()-confNums[0])) > 250)
			confNums[3] = 0.012*(confNums[0]*(GetDataSize()-confNums[0]));
		else
			confNums[3] = 250;
	}

	printf("Stage #0 COMPLETE\n");

	printf("Stage #1: Nearest Neighbor LSH Recommend\n");
	fprintf(output,"NN LSH\n");
	tLSH = GetClockTimeInMilliSec();
	NNLSHRecommend(output,confNums[2],confNums[1],neighbors, results, 0);
	tLSH = GetClockTimeInMilliSec() - tLSH;
	DeleteAllFromList(results);

	printf("Stage #1 COMPLETE\n");

	if(validate){

		printf("Extra Step: NNLSH Validation\n");

		tValLSH = GetClockTimeInMilliSec();
		KFoldValidateLSH(output,LLSH,kLSH,neighbors, validate);
		tValLSH = GetClockTimeInMilliSec() - tValLSH;

		printf("VALIDATION COMPLETE\n");

		printf("Maintenance: Reparsing Data\n");

		DestroyData();
		rewind(input);
		parseData(input,confNums[1]);

		printf("Reparsing COMPLETE\n");


	}

	printf("Stage #2: Cluster Recommend\n");


	fprintf(output,"Clustering\n");
	tClust = GetClockTimeInMilliSec();
	int clust = ClustRecommend(output,cluster,confNums,neighbors,0,results,0);
	tClust = GetClockTimeInMilliSec()- tClust;
	printf("Stage #2: COMPLETE\n");


	if(validate){
		printf("Extra Step: Cluster Validation\n");
		tValClust = GetClockTimeInMilliSec();
		KFoldValidateCluster(output,cluster,confNums,neighbors,clust,results,validate);
		tValClust = GetClockTimeInMilliSec() - tValClust;
		printf("VALIDATION COMPLETE\n");
	}

	fprintf(output, "Time LSH %d\n",(int)tLSH );
	fprintf(output, "Time Validation LSH %d\n",(int)tValLSH );
	fprintf(output, "Time Clustering %d\n",(int)tClust );
	fprintf(output, "Time Validation Clustering %d\n",(int)tValClust );

	DestroyData();
	data.destroyInput();

	fclose(config);
	fclose(input);
	fclose(output);

	return 0;
}
