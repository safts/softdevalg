#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

#include "../input/dataHamming.h"
#include "../input/dataEuclidean.h"
#include "../input/dataDistanceMatrix.h"


/*void Print(value* v) {
	if (v == NULL || v->name == NULL) printf("NULL");
	printf("%s", v->name);
}
*/

struct dataAct_t {
	value* data;
	int dataSize;


	int centroidsIndex;  // array index :data[centroidsIndex] contains
	// the LAST centroid

	// The FIRST centroid is in data[dataSize-1]

	int* clustIndexes;  // array of size K. clustIndexes[i] contains
	// the location of the first point in cluster i.

	// NOTE: the last element of the last
	// cluster is in data[centroidsIndex-1]

	int centroidNum;  // Number of centroids currently in the data
	// array. Always <=k

	int k;



	// All data (points, cluster numbers, centroids are
	// First centroid lies in data[dataSize-k]
};

typedef struct clustExtra_t {
	int flagAssign;
	int clust;
	double clustDist;
	int secClust;
	double secClustDist;
	int secFlagAssign;

} clustExtra;

Data dataAct;
extern dataIF data;

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ClustExtra
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void InitClustExtra(int point) {
	value* pointVal = &dataAct.data[point];
	clustExtra* val;
	val = malloc(sizeof(clustExtra));
	val->clust = -1;
	val->clustDist = -1;
	val->flagAssign = 0;
	val->secClust = -1;
	val->secClustDist = -1;
	val->secFlagAssign = -1;
	val->secFlagAssign = 0;
	pointVal->clustExtra = val;
}

void PrintClusters(FILE* output){

	int i=0;
	int j=0;
	for(i=0;i<dataAct.k;i++){

		int clustMin = dataAct.clustIndexes[i];
		int clustMax = 0;
		if (i == dataAct.k - 1)
			clustMax = dataAct.centroidsIndex ;
		else
			clustMax = dataAct.clustIndexes[i + 1];
		value clustVal;
		GetIthCentroid(i,&clustVal);
		int size=clustMax-clustMin;
		if(size<0)
			size=0;
		fprintf(output,"CLUSTER-%d {size: %d, medoid: %s}\n",i,
				size, clustVal.name);
	}
}

int GetClustSize(int i){

	int clustMin = dataAct.clustIndexes[i];
	int clustMax = 0;
	if (i == dataAct.k - 1)
		clustMax = dataAct.centroidsIndex ;
	else
		clustMax = dataAct.clustIndexes[i + 1];
	int size=clustMax-clustMin;
	if(size<0)
		size=0;
	return size+1;
}

void PrintClustersComplete(FILE* output){

	int i=0;
	int j=0;
	for(i=0;i<dataAct.k;i++){

		int clustMin = dataAct.clustIndexes[i];
		int clustMax = 0;
		if (i == dataAct.k - 1)
			clustMax = dataAct.centroidsIndex ;
		else
			clustMax = dataAct.clustIndexes[i + 1];
		value clustVal;
		GetIthCentroid(i,&clustVal);
		int size=clustMax-clustMin;
		if(size<0)
			size=0;
		fprintf(output,"CLUSTER-%d {size: %d, medoid: %s}\n",i,
				size, clustVal.name);
		if(clustMin<clustMax)
			fprintf(output,"CLUSTER-CONTENTS: {");
		else
			fprintf(output,"CLUSTER-CONTENTS: (empty)");
		for(j=clustMin;j<clustMax;j++){

			value clustDat;
			GetIthData(j,&clustDat);
			fprintf(output,"%s,",clustDat.name);
		}
		if(clustMin<clustMax)
			fprintf(output,"}");
		fprintf(output,"\n");
	}
}


void AssignClustExtra(int point, int clust, double clustDist, int secClust,
		double secClustDist) {
	value pointVal;
	GetIthData(point, &pointVal);

	// if(pointVal.clustExtra==NULL) // den exoun noima... den ginontai init
	// return;

	clustExtra* info = pointVal.clustExtra;

	info->clust = clust;
	info->clustDist = clustDist;
	info->secClust = secClust;
	info->secClustDist = secClustDist;
}

double GetDistFirstCentr(int point) {
	value pointVal;
	GetIthData(point, &pointVal);

	//if (pointVal.clustExtra == NULL) return -1;

	return ((clustExtra*)pointVal.clustExtra)->clustDist;
}

double GetDistSecCentr(int point) {
	value pointVal;
	GetIthData(point, &pointVal);

	//if (pointVal.clustExtra == NULL) return -1;

	return ((clustExtra*)pointVal.clustExtra)->secClustDist;
}

int GetExtrasCluster(int point){
	value pointVal;
	GetIthData(point, &pointVal);
	//if (pointVal.clustExtra == NULL) return -1;
	return ((clustExtra*)pointVal.clustExtra)->clust;
}

// void UpdateClustExtra(int point,int clust,double clustDist){
//
//	value* pointVal=GetIthData(point);
//
//	if(pointVal == NULL || pointVal->clustExtra==NULL)
//		return;
//
//	clustExtra* info=pointVal->clustExtra;
//
//	if(clustDist < info->secClustDist ){
//
//		if(clustDist < info->clustDist){
//			info->secClustDist=info->clustDist;
//			info->secClust=info->clust;
//			info->clust=clust;
//			info->clustDist=clustDist;
//		}
//		else{
//			info->secClustDist=clustDist;
//			info->secClust=clust;
//		}
//	}
//}

int IsAssigned(int point) {
	value pointVal;
	GetIthData(point, &pointVal);
	//if (pointVal.clustExtra == NULL) return -1;
	if ((((clustExtra*)(pointVal.clustExtra))->flagAssign == 1) &&
			(((clustExtra*)(pointVal.clustExtra))->secFlagAssign == 1)) {
		return 1;
	}
	return 0;
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ ClustExtra
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~  Cluster
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

void Swap(int ind1, int ind2) {
	if (ind1 >= dataAct.dataSize || ind2 >= dataAct.dataSize)
		printf("Swap indexes over datasize\n");
	if (ind1 < 0 || ind2 < 0) 
		printf("Swap indexes under zero\n");
	value v;
	if (ind1 == ind2)
		return;
	memcpy(&v, &dataAct.data[ind1], sizeof(value));
	memcpy(&dataAct.data[ind1], &dataAct.data[ind2], sizeof(value));
	memcpy(&dataAct.data[ind2], &v, sizeof(value));
}

int GetMedoid(int cluster) {
	int clustMin = dataAct.clustIndexes[cluster];
	int clustMax;
	int i, j, minDistIndex;
	double sumDist = 0, minDist;

	if (cluster == dataAct.k - 1)
		clustMax = dataAct.centroidsIndex - 1;
	else
		clustMax = dataAct.clustIndexes[cluster + 1] - 1;

	double* avgDist = malloc(sizeof(double) * (clustMax - clustMin + 1));
	minDist = 99999999999;
	minDistIndex = 0;
	for (i = clustMin; i < clustMax + 1; ++i) {
		for (j = clustMin; j < clustMax + 1; ++j) {
			if (i == j) continue;
			sumDist += data.distance(&dataAct.data[i], &dataAct.data[j]);
			if(sumDist > minDist)
				continue;
		}
		if(sumDist > minDist){
			sumDist = 0;
			continue;
		}
		value centroid;
		GetIthCentroid( cluster, &centroid);
		minDist = sumDist;
		minDistIndex = i;
		sumDist = 0;
	}

	
	

	free(avgDist);
	return minDistIndex;
}

// does not preserve the cluster functionality
int AddCentroid(int centrIndex) {
	if (centrIndex > dataAct.dataSize) return -1;
	dataAct.centroidNum++;
	dataAct.k++;
	dataAct.centroidsIndex--;
	Swap(centrIndex, dataAct.centroidsIndex);
	return dataAct.centroidsIndex;
}

void FirstAssignment() {
	int offset = (int)( (dataAct.dataSize - dataAct.k) / (float)dataAct.k);
	int i = 0;
	dataAct.clustIndexes = malloc(sizeof(int) * dataAct.k);
	for (i = 0; i < dataAct.k; i++) {
		dataAct.clustIndexes[i] = i * offset;
	}
	for (i = 0; i < dataAct.dataSize; ++i) InitClustExtra(i);
}

int AddToClust(int index, int clustNum) {
	int i = 0;
	if (index < 0) 
		printf("aaaaaaaaaaaaaaaaaaaaaaaaa\n");
	int pointCluster = GetClusterOf2(index);
	//printf("clust %d\n",GetClusterOf(index));
		
	if (clustNum > dataAct.k - 1) return -1;

	int clustMin = dataAct.clustIndexes[clustNum];
	int clustMax = 0;
	if (clustNum == dataAct.k - 1){
		clustMax = dataAct.centroidsIndex - 1;
		//printf("%d\n", clustMax);
	}
	else
		clustMax = dataAct.clustIndexes[clustNum + 1] - 1;

	//	while( index > clustMax +1) 
	//if(pointCluster != clustNum){ 
		if (index > clustMax ){		//moves left
			for (i = pointCluster; i > clustNum; i--) {
				//printf("%d\n", dataAct.clustIndexes[pointCluster]);
				Swap(index, dataAct.clustIndexes[pointCluster]);
				dataAct.clustIndexes[pointCluster]++;
				index = dataAct.clustIndexes[pointCluster] - 1;
				pointCluster--;
			}
			return 0;
		}
		else if (index < clustMin ){  // moves right
			for (i = pointCluster; i < clustNum; i++) {
				//printf("%d,%d\n",index, dataAct.clustIndexes[pointCluster]);
				Swap(index, dataAct.clustIndexes[pointCluster + 1] - 1);
				dataAct.clustIndexes[pointCluster + 1]--;
				index = dataAct.clustIndexes[pointCluster + 1];
				pointCluster++;
			}
			return 1;
		}
		else{
			//printf("aa %d\n", clustNum);
			return 0;
		}

	//}
}

int GetClusterOf(int index) {
	value v;
	GetIthData(index, &v);
	clustExtra* extra = v.clustExtra;
	int ret = extra->clust;
	return ret;
}

int GetClusterOf2(int index) {
	int i;
	for (i = 0; i < GetNoOfCluster(); ++i) {
		if (i == GetNoOfCluster() - 1)
		   return i;
		if (dataAct.clustIndexes[i] <= index &&
				dataAct.clustIndexes[i + 1] > index)
			return i;
	}
	return -1;
}

void SwapCentroid(int medoid, int oldCentr) {
	Swap(medoid, oldCentr + dataAct.centroidsIndex);
}

double ComputeAproximateDJ(int oldCentr, int centroid, int *sample, int sSize) {
	// wakawaka wikiwikiwoooooosh

	int i = 0;
	double DJ = 0.0;
	value v1, v2;
	GetIthData(centroid, &v2);
	for (i = 0; i < sSize; i++) {
		GetIthData(sample[i], &v1);
		double iFromCentr = data.distance(&v1, &v2);
		if (GetClusterOf(sample[i]) == oldCentr) {
			if (iFromCentr >= GetDistSecCentr(i))
				DJ += GetDistSecCentr(sample[i]) - GetDistFirstCentr(sample[i]);
			else
				DJ += iFromCentr - GetDistFirstCentr(sample[i]);
		} else if (iFromCentr < GetDistFirstCentr(sample[i]))
			DJ += iFromCentr - GetDistFirstCentr(sample[i]);
	}
	double bestDist = 9999999999;
	GetIthCentroid( oldCentr, &v2);
	for (i = 0; i < dataAct.k; ++i)
	{
		if ( i == oldCentr) 
			continue;
		GetIthCentroid(i, &v1);
		double dist = data.distance(&v1, &v2);
		if ( dist < bestDist)
		{
			bestDist = dist;
		}
	}
	GetIthData( centroid, &v1);
	double dist = data.distance(&v1, &v2);
	if (dist >= bestDist)
		DJ += bestDist;
	else
		DJ += dist;
	return DJ;
	return -1;
}

double ComputeDJ(int oldCentr, int centroid) {
	// wakawaka wikiwikiwoooooosh

	int i = 0;
	double DJ = 0.0;
	value v1, v2;
	for (i = 0; i < GetDataSize() - dataAct.k; i++) {
		GetIthData(i, &v1);
		GetIthData(centroid, &v2);
		double iFromCentr = data.distance(&v1, &v2);
		if (GetClusterOf(i) == oldCentr) {
			if (iFromCentr >= GetDistSecCentr(i))
				DJ += GetDistSecCentr(i) - GetDistFirstCentr(i);
			else
				DJ += iFromCentr - GetDistFirstCentr(i);
		} else if (iFromCentr < GetDistFirstCentr(i))
			DJ += iFromCentr - GetDistFirstCentr(i);
	}
	double bestDist = 9999999999;
	int bestPos = -1;
	GetIthCentroid( oldCentr, &v2);
	for (i = 0; i < dataAct.k; ++i)
	{
		if ( i == oldCentr) 
			continue;
		GetIthCentroid(i, &v1);
		double dist = data.distance(&v1, &v2);
		if ( dist < bestDist)
		{
			bestDist = dist;
			bestPos = i;
		}
	}
	GetIthData( centroid, &v1);
	double dist = data.distance(&v1, &v2);
	if (dist >= bestDist)
		DJ += bestDist;
	else
		DJ += dist;
	return DJ;
	return -1;
}

void InitAssigned() {
	int i;
	for (i = 0; i < GetDataSize(); ++i){
		value* pointVal = &dataAct.data[i];
		clustExtra* val;
		val = pointVal->clustExtra;
		val->clust = -1;
		val->clustDist = -1;
		val->flagAssign = 0;
		val->secClust = -1;
		val->secClustDist = -1;
		val->secFlagAssign = -1;
		val->secFlagAssign = 0;
	}
}

double MinDistCendroids() { 
	int i,j;
	double dist,minDist = 99999999999;
	value centr1,centr2;

	for(i=0; i<dataAct.k; i++){
		GetIthCentroid(i,&centr1);
		for(j=0;j<dataAct.k; j++){
			if(i==j) continue;
			GetIthCentroid(j,&centr2);
			dist = data.distance(&centr1,&centr2);
			if(dist < minDist)
				minDist = dist;
		}
	}
	return minDist; 
}

double MaxDistCendroids() { 
	int i,j;
	double dist,maxDist = 0;
	value centr1,centr2;

	for(i=0; i<dataAct.k; i++){
		GetIthCentroid(i,&centr1);
		for(j=0;j<dataAct.k; j++){
			if(i==j) continue;
			GetIthCentroid(j,&centr2);
			dist = data.distance(&centr1,&centr2);
			if(dist > maxDist)
				maxDist = dist;
		}
	}
	return maxDist; 
}

int AssignLSH(value *v, int dist, int clustNum, int loop){
	clustExtra* extra = v->clustExtra;
	//mark 1st assigned
	int ret = extra->flagAssign && extra->secFlagAssign;
	if(extra->flagAssign == 0){
		extra->clust = clustNum;
		extra->clustDist = dist;
		extra->flagAssign = 1;
		return 0;
	}
	//make second first and first dist/clustNum
	if(extra->clustDist > dist){
		extra->secClust = extra->clust;
		extra->secClustDist = extra->clustDist;
		extra->clust = clustNum;
		extra->clustDist = dist;
		extra->secFlagAssign = 1;
		return !ret;
	}
	if(extra->secFlagAssign == 0){
		if(extra->clust!=clustNum){
			extra->secClust = clustNum;
			extra->secClustDist = dist;
			extra->secFlagAssign = 1;
			return !ret;
		}
		return 0;
	}
	if(extra->secClustDist > dist){
		extra->clust = clustNum;
		extra->clustDist = dist;
	}
	return !ret;
}

void GetScoreMatrix(int item, Rating* rating){
	int i;
	for(i=0; i<GetDataSize(); ++i){
		rating[i].rate = data.getScore(i,item);
		rating[i].id = data.getId(i,item);
	}
}

void SetScores(int item, Rating* rating){
	int i;
	for ( i = 0; i < GetDataSize(); ++i)
	{
		data.setScore(i,item, (int)rating[i].rate);
	}
}

void DeleteItems(int item){
	int i;
	for ( i = 0; i < GetDataSize(); ++i)
	{
		data.deleteItem(i,item);
	}
}

double AvgDistToClust(int index, int clust){
	int min = dataAct.clustIndexes[clust];
	int max;
	if(clust == dataAct.k-1)
		max = dataAct.centroidsIndex;
	else
		max = dataAct.clustIndexes[clust+1];
	int i;
	value v1, v2;
	GetIthData( index, &v1);
	double sumDist = 0.0;
	for (i = min; i < max; ++i )
	{
		GetIthData( i, &v2);
		sumDist += data.distance( &v1, &v2);
	}
	GetIthCentroid(clust , &v2);
	sumDist += data.distance( &v1, &v2);

	//if ( max == min) 
		//return -1;
	//printf("size of Cluster %d %d\n", clust, max - min);
	//printf("%f\n", sumDist);
	return sumDist/(max - min+1);
}



double Silhouette(FILE* output){
	double* a,*b,*s, *sAvg;
	double sTotal = 0;
	a = malloc((GetDataSize()-dataAct.k)*sizeof(double));
	b = malloc((GetDataSize()-dataAct.k)*sizeof(double));
	s = malloc((GetDataSize()-dataAct.k)*sizeof(double));
	sAvg = malloc(dataAct.k*sizeof(double));
	int i;
	for(i=0;i<dataAct.k;i++){
		sAvg[i] = 0;
	}
	#pragma omp parallel for 
	for(i=0; i<GetDataSize()-dataAct.k; i++){
		

		a[i] = AvgDistToClust(i, GetClusterOf(i));
		double minAvgClusterDist = 9999999999;
		int j;
		for (j = 0; j < GetNoOfCluster(); ++j)
		{
			if ( j == GetClusterOf(i)) 
				continue;
			double avgClusterDist = AvgDistToClust(i, j);
			if ( avgClusterDist < minAvgClusterDist)
				minAvgClusterDist = avgClusterDist;
		}
		b[i] = minAvgClusterDist;
		//printf("%f, %f\n", b[i], a[i]);

		if(a[i] > b[i])
			s[i] = b[i]/a[i] - 1;
		else if(a[i] < b[i])
			s[i] = 1- a[i]/b[i];
		else
			s[i] = 0;
		//sTotal += s[i];
	}
	for (i = 0; i < GetDataSize()-dataAct.k; ++i)
	{
		sTotal += s[i];
		sAvg[GetClusterOf(i)]+=s[i];
	}
//	fprintf(output,"Silhouette: [");
//	for(i=0;i<dataAct.k;i++){
//		if(i>0)
//			fprintf(output,",");
//		fprintf(output,"%2f",sAvg[i]/GetClustSize(i));
//	}

	sTotal = sTotal/(GetDataSize()-dataAct.k);
//	fprintf(output,",%2f]\n",sTotal);
	free(a);
	free(b);
	free(s);
	free(sAvg);
	return sTotal;
}

void DestroyClusters(){

	free(dataAct.clustIndexes );
	dataAct.k=0;
	dataAct.centroidNum=0;
	dataAct.centroidsIndex=dataAct.dataSize;
}

void DestroyData(){
	int i;
	for (i = 0; i < GetDataSize(); ++i)
	{
		value v;
		GetIthData(i, &v);
		if(v.name != NULL)
			free(v.name);
		if(v.clustExtra != NULL)
			free(v.clustExtra);
		free(v.content);
	}
	free(dataAct.data);
	free(dataAct.clustIndexes);
}




void SetDataSize(int size) { dataAct.dataSize = size; }

int GetAvgClustSize(){
	int i,sum = 0;;
	for(i=0; i< dataAct.k; ++i){
		sum+= GetClustSize(i);
	}
	return sum/dataAct.k;
}

int GetDataSize() { return dataAct.dataSize; }

void GetIthData(int i, value* v) { 
	memcpy(v, &dataAct.data[i], sizeof(value)); 
}


void GetIthCentroid(int i, value* v) {
	if (i > dataAct.k) return;
	memcpy(v, &dataAct.data[i + dataAct.centroidsIndex], sizeof(value));
}

int GetNoOfCluster() {
	return dataAct.k;
}

void SetNoOfCluster(int k){
	dataAct.k = k;
}

void PrintData() {
	int i;
	for (i = 0; i < GetDataSize(); i++) {
		data.print(&dataAct.data[i]);
	}
}

void parseData(FILE* file, int kLSH) {
	dataAct.data = NULL;
	dataAct.clustIndexes = NULL;
	dataAct.centroidNum = 0;
	dataAct.k = 0;
	data.readData(&dataAct.data, file, kLSH);
	dataAct.centroidsIndex = dataAct.dataSize;
	
}

void ParseConfig(FILE* file, int* confNums){
	char* lineBuff = NULL;
	char secBuff1[256];
	char secBuff2[256];
	int temp[5];
	size_t lineSize = 0;
	size_t lineLen = 0;
	int i=0,count=1;
	memset(secBuff1, '\0', sizeof(secBuff1));
	memset(secBuff2, '\0', sizeof(secBuff2));

	for(i=0; i<5; i++)
		confNums[i] = 0;
	while ((lineLen = getline(&lineBuff, &lineSize, file)) != -1) {
		count++;
		if (sscanf(lineBuff, "%s %s", secBuff1, secBuff2) < 2) {
			perror("Malformed file - Exiting...\n");
			exit(-1);
		}
		if(!strcmp(secBuff1,"number_of_clusters:")){
			confNums[0] = 1;
			temp[0] = atoi(secBuff2);
		}
		if(!strcmp(secBuff1,"number_of_hash_functions:")){
			confNums[1] = 1;
			temp[1] = atoi(secBuff2);
		}
		if(!strcmp(secBuff1,"number_of_hash_tables:")){
			confNums[2] = 1;
			temp[2] = atoi(secBuff2);
		}
		if(!strcmp(secBuff1,"clarans_set_fraction:")){
			confNums[3] = 1;
			temp[3] = atoi(secBuff2);
		}
		if(!strcmp(secBuff1,"clarans_iterations:")){
			confNums[4] = 1;
			temp[4] = atoi(secBuff2);
		}

	}
	
	for(i = 0; i<5; i++){
		if(confNums[i] == 1)
			confNums[i] = temp[i];
		else{
			if(i == 1)
				confNums[i] = 4;
			if(i == 2)
				confNums[i] = 5;
			if(i == 4)	
				confNums[i] = 2;
		}
	}
	free(lineBuff);

	

}


int GetDataOfCluster(int clust, value*** vptr){
	if ( clust > dataAct.k)
		return -1;
	int min = dataAct.clustIndexes[clust];
	int max;
	if(clust == dataAct.k-1)
		max = dataAct.centroidsIndex;
	else
		max = dataAct.clustIndexes[clust+1];
	*vptr = malloc(sizeof(value*)*(max-min));
	int i;
	for (i = min; i < max; ++i){
//		printf("%d %d %d\n",i,min,max);
		(*vptr)[i-min] = &(dataAct.data[i]);
	}
	return max - min;

}

void specifyDataset(FILE* file) {
	char* lineBuff = NULL;
	char secBuff1[256];
	char secBuff2[256];

	size_t lineSize = 0;
	size_t lineLen = 0;

	memset(secBuff1, '\0', sizeof(secBuff1));
	memset(secBuff2, '\0', sizeof(secBuff2));
	if ((lineLen = getline(&lineBuff, &lineSize, file)) == -1) {
		free(lineBuff);
		lineBuff = NULL;
		exit(-1);
	}

	if (sscanf(lineBuff, "%s %s", secBuff1, secBuff2) < 2) {
		perror("Malformed file - Exiting...\n");
		exit(-1);
	}

	if (strcmp(secBuff2, "hamming") == 0) {
		data.distance = Hamming;
		data.inRange = InRangeHamming;
		data.print = PrintDataHamming;
		data.readData = ReadDataHamming;
		data.readQueries = ReadQueriesHamming;
		data.G = GHamming;
		data.getFirstQuery = GetFirstQueryHamming;
		data.getNextQuery = GetNextQueryHamming;
		data.initStruct = InitStructHamming;
		// data.bruteForce = BruteForceANNHamming;
		// data.bruteForceRange = BruteForceInRangeHamming;
		data.destroyStruct = DestroyStructHamming;
		data.destroyInput = DestroyInputHamming;
		data.destroyValue = DestroyValueHamming;

	} else if (strcmp(secBuff2, "vector") == 0) {
		memset(secBuff1, '\0', sizeof(secBuff1));
		memset(secBuff2, '\0', sizeof(secBuff2));

		if ((lineLen = getline(&lineBuff, &lineSize, file)) == -1) {
			// something went wrong
			free(lineBuff);
			lineBuff = NULL;
			exit(-1);
		}
		if (sscanf(lineBuff, "%s %s", secBuff1, secBuff2) < 2) {
			perror("Malformed file - Exiting...\n");
			exit(-1);
		}

		if (strcmp(secBuff2, "euclidean") == 0) {
			data.distance = Euclidean;
			data.inRange = InRangeEuclidean;
			data.print = PrintDataEuclidean;
			data.G = FEuclidean;
			data.initStruct = InitStructEuclidean;
			data.destroyStruct = DestroyStructEuclidean;
			data.destroyInput = DestroyInputEuclidean;
			data.destroyValue = DestroyValueEuclidean;
		}

	} else if (strcmp(secBuff2, "matrix") == 0) {
		data.distance = DistanceMatrixDistance;
		data.inRange = InRangeDistanceMatrix;
		data.print = printEverything;
		data.readData = ReadDataDistanceMatrix;
		data.readQueries = ReadQueriesDistanceMatrix;
		data.G = GDistanceMatrix;
		data.getFirst = GetFirstDistanceMatrix;
		data.getNext = GetNextDistanceMatrix;
		data.getFirstQuery = GetFirstQueryDistanceMatrix;
		data.getNextQuery = GetNextQueryDistanceMatrix;
		data.initStruct = InitStructDistanceMatrix;
		data.bruteForce = BruteForceANNDistanceMatrix;
		data.destroyStruct = DestroyStructDistanceMatrix;
		data.destroyInput = DestroyInputDistanceMatrix;
		data.destroyValue = DestroyValueDistanceMatrix;

	} else {
		printf(
				"\nWrong metric space\nIt is \"%s\" instead of \"hamming\", "
				"\"vector\" or \"matrix\"\n\n",
				secBuff2);
	free(lineBuff);
		exit(-1);
	}
	free(lineBuff);
}
