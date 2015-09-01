#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>


#include "../cluster/assignment/pam.h"
#include "../cluster/assignment/LSHAssign.h"
#include "../cluster/initialization/kMedoidPlusPlus.h"
#include "../cluster/initialization/concentrate.h"
#include "../cluster/update/lloyds.h"
#include "../cluster/update/claransUpdate.h"


#include "../dataStruct/data.h"


/*
 ####################""N########NN#####################################D.H.
 ################N@" _, #######F  0########################################
 ###############@" _g# j#@FN#@ _  0########################################
 #############NF _g##F 0" _ " j# j#########################################
 #############F g#### - _g   g@ q####@" "N##F""9N##N"P"N###@"3#@"0#########
 #############NJ####   g#"  d@  0###F g@ g#P _  0##' , j##@ j0# j##########
 ##################"  d#F  0#  J###" +"_d#@ J" g##F 0  0##  ##  0##########
 ################NF _0#@ _0## j##NF  _g##F gF j#N"_A  0##  0#  J###########
 ###############N" _0#@  d##  0N@"  d##@"_d# qNF _#F J@"   @  jPT##########
 ###############F _0##_ d###p ""_g#_`""_g###p  jg###_ _g0k _p F_A##########
 ################g#####N#####pgg####Ngg######NN#####NNN###N#  j############
 ##########################################################  j#############
 ######N@" "N#####NN######################################F j##############
 ####NF  ___ ##### j######################################L 0##############
 ###@   gN# j###@ j#############@"N########################N###############
 ##F  a0"jF d##N" ###############g0#########" B############################
 #F  j##   j###F N######NNNN####N#####F"2NNF dN###N#####N@0#####@@N#####@""
 #  j###pgg###@ g@""###@    ###F J###" j#E, ,_0#@ j@""0@  j##N@"_, j###@  d
 F  #########@     d##@ jF J##F j### j d##' 0##@   _f ", j###  g#  ###@ p #
 F j########P   d j##@ jF j##@ j##@ gP 0#F j##P   d@  g' ###  0#  d##F d  #
 # 3#####@F   _#F 0N@ g@ j##"  0N@  #  N"  N#"_ _0# _0# jN@  g#" j#@" t@ j@
 #L_""""" _g" 0#_ P"_d#& 9" ap F"_p " ` arJ@ a' 0# _0## ?"__ " j_`F g_ ' "_
 ##Np___gg##pg###_jg####L_g0##__g##p_ag0##_jg#Ld##m0###p_g##p_g##_a0##__gg#

*/

 unsigned long int GetClockTimeInNanoSec()
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
	if(nano_seconds) {if(!first) fprintf(file,":"); fprintf(file,"%dns", nano_seconds); first=0;}
	fprintf(file,"]");
}


dataIF data;



int main(int argc, char *argv[]){

	int choice;
	char inputFile[256], outputFile[256], confFile[256];
	int inputFlag = 0, outputFlag = 0, confFlag = 0;
	int completFlag =0;
	int flag;
	void (*init)() = KMedoidPlusPlusInit;
	double (*assigment)() = PamAssign;
	int (*update)() = ClaransUpdate;
	while (1)
	{
		static struct option long_options[] =
		{
			/* Use flags like so:
			{"verbose",	no_argument,	&verbose_flag, 'V'}*/
			/* Argument styles: no_argument, required_argument, optional_argument */
			{"version", no_argument,	0,	'v'},
			{"help",	no_argument,	0,	'h'},
			{"inputFile", required_argument, 0, 'd'},
			{"conf", required_argument, 0, 'c'},
			{"output", required_argument, 0, 'o'},
			{"intialization", required_argument, 0, 'I'},
			{"assigment", required_argument, 0, 'A'},
			{"update", required_argument, 0, 'U'},
			{"complete", no_argument, 0, 'a'},
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
			case 'v':
				printf("beta version\n");
				
				break;
	
			case 'h':
				printf("ussage <executable> –d <inputFile file> –c <configuration file> -o <output file> -I <init fun> -A <assign fun> -U <update fun> --complete\n");
				
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
			case 'I':
				flag = atoi(optarg);
				if ( flag == 1) 
					init = KMedoidPlusPlusInit;
				else
					init = ConcentrateInit;

				break;
			case 'A':
				flag = atoi(optarg);
				if (flag == 1) 
					assigment = PamAssign;
				else
					assigment = LSHAssign;

				break;
			case 'U':
				flag = atoi(optarg);
				if ( flag == 1) 
					update = LloydsUpdate;
				else 
					update = ClaransUpdate;

				break;
			case 'a':
				completFlag = 1;

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
	printf("inputFile %s, output %s, confingure %s, completFlag = %d\n",inputFile, outputFile, confFile, completFlag );
	//FILE* config = fopen("./inputFiles/cluster_10.conf","r");
//	FILE* config = fopen("./inputFiles/cluster_10.conf","r");
	//FILE* config = fopen("./inputFiles/cluster_15.conf","r");
	//FILE* input=fopen("./inputFiles/DataHamming.csv","r");

	//FILE* input=fopen("./inputFiles/Ex2_Datasets/DataHamming_5_1000x64.csv","r");
	//FILE* input=fopen("./inputFiles/Ex2_Datasets/DataHamming_15_10000x64.csv","r");
	//FILE* input=fopen("./inputFiles/Ex2_Datasets/DataHamming_10_1000x64.csv","r");
	//FILE* input=fopen("./inputFiles/Ex2_Datasets/DataHamming_5_100000x64.csv","r");
	//FILE* input=fopen("./inputFiles/Ex2_Datasets/DataEuclidean_5_500x100.csv","r");
	//FILE* input=fopen("./inputFiles/Ex2_Datasets/DataEuclidean_5_1000x500.csv","r");
	//FILE* input=fopen("./inputFiles/Ex2_Datasets/DataEuclidean_15_1000x100.csv","r");
	//FILE* input=fopen("./inputFiles/Ex2_Datasets/DataEuclidean_10_1000x100.csv","r");
	//FILE* input=fopen("./inputFiles/Ex2_Datasets/DataEuclidean_15_1000x500.csv","r");
//	FILE* input=fopen("./inputFiles/Ex2_Datasets/DataDistanceMatrix_10_1000x1000.csv","r");

	FILE* input=fopen(inputFile,"r");
	FILE* config=fopen(confFile,"r");
	FILE* output=fopen(outputFile,"w");

	int confNums[5];
	time_t tInit=0,tAssign=0,tTotal=0;

	srand((time(NULL)));
	specifyDataset(input);
	printf("aaaaaaaa\n");

	ParseConfig(config,confNums);
	parseData(input,confNums[1]);
	
	if(confNums[3] == 0){
		if(0.012*(confNums[0]*(GetDataSize()-confNums[0])) > 250)
			confNums[3] = 0.012*(confNums[0]*(GetDataSize()-confNums[0]));
		else
			confNums[3] = 250;
	}

	tTotal =GetClockTimeInNanoSec();
	tInit = GetClockTimeInNanoSec();
	//printf("tInit = %ld\n",tInit);
	
	
	init(confNums[0]);
	
	time_t tmp  = GetClockTimeInNanoSec();
	//printf("tmp = %ld\n",tmp);
	tInit = tmp - tInit;
	
	FirstAssignment();
	
	tAssign = GetClockTimeInNanoSec();
	//printf("tAssign = %ld\n",tAssign);
	
	assigment(confNums[1],confNums[2]);
	int count = 0;

	while(update(assigment,confNums))
		count++;
	
	tmp  = GetClockTimeInNanoSec();
	//printf("tmp = %ld\n",tmp);
	tAssign = tmp - tAssign;
	

	tTotal = GetClockTimeInNanoSec() - tTotal;
	fprintf(output,"LOOPS: %d\n", count);

	PrintClusters(output);
	if(completFlag)
		PrintClustersComplete(output);

	fprintf(output,"Initialization time\t\tAssigment and Update time\t\tTotal time\n");
	fprintf(output,"%d\t\t\t\t%d\t\t\t\t\t%d\n",(int)tInit,(int)tAssign,(int)tTotal);
	Silhouette(output);
	DestroyData();
	if ( assigment == LSHAssign)
		DestroyLSHAssignment();
	data.destroyInput();
	fclose(input);
	fclose(config);
	return 0;
}
