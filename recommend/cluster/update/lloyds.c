#include <stdio.h>
#include <stdlib.h>

#include "../../dataStruct/data.h"

#define INF 999999

extern dataIF data;



int LloydsUpdate(double (*assign)(),int* confNums){

	int i=0;
	int medoid=0;
	int flag = 0;
	static double oldJ=-1;
	double bestDJ = 0;
	for(i=0;i< GetNoOfCluster();i++){
		medoid=GetMedoid(i);
		double dj = ComputeDJ(i, medoid);
		if (dj< bestDJ){
			SwapCentroid(medoid, i);
			double j = assign(confNums[1],confNums[2]);
			if( oldJ != -1 && j > oldJ )
				return 0;
			oldJ = j;
			bestDJ = dj;
			flag = 1;
		}
	}

	return flag;
}
