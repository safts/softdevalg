#include <stdio.h>

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

	FILE* input=NULL;
	FILE* output=NULL;
	FILE* output1=NULL;
	FILE* config=NULL;

	int confNums[5];

	input=fopen("./inputFiles/bio_small_input.dat","r");
	output1=fopen("./inputFiles/bla.dat","w");
	output=fopen("./inputFiles/conform.dat","w");
	config=fopen("./inputFiles/cluster_5.conf","r");

	//ReadDataMolecule(NULL, input);
	SpecifyDatasetMolConf("molecule");

	ParseConfig(config,confNums);
	parseData(input,4);
	void (*init)() = KMedoidPlusPlusInit;
	double (*assigment)() = PamAssign;
	int (*update)() = ClaransUpdate;
	int kCluster = 10;
	kCluster = atoi(argv[1]);
	printf("%d\n", kCluster);
	//int j = 0;
	int j = 0;
	int i;
	value v1,v2;


	// double dist,tdist=0,max=0,min=9999999999;
	// for(i=0; i<GetDataSize(); i++){
	// 	for(j=0; j<GetDataSize(); j++){
	// 		if( i == j)
	// 			continue;
	// 		GetIthData(i,&v1);
	// 		GetIthData(j, &v2);
	// 		dist = data.distance(&v1, &v2);
	// 		tdist +=dist;
	// 		if(dist > max)
	// 			max = dist;
	// 		if(dist < min)
	// 			min = dist;

	// 	}
	// }
	// tdist = tdist/(GetDataSize()*GetDataSize());
	// printf("avg cRMSD: %f max: %f min: %f\n",tdist,max,min);
	// //return 0;

	init(kCluster);

	FirstAssignment();

	assigment(confNums[1],confNums[2]);

	j = 0;
	while(update(assigment,confNums)){
		fprintf(output1,"Update loop #%d\n",j);
		printf("Update loop #%d\n",j);
		j++;
	}
	PrintClusters(output1);
	double silhouette=Silhouette(output1);
	fprintf(output, "%d\n%f\n",kCluster,silhouette );
	PrintConform(output);

	
	printf("Silhouette: %f\n",silhouette);

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

