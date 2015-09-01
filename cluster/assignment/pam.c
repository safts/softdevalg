#include <stdio.h>
#include <stdlib.h>

#include "../data.h"

#define INF 999999

extern dataIF data;


double PamAssign(int L, int kLSH ){

	//printf("*****************************************************\n");
	double prev = 99999999999;
	int i=0;
	int j=0;

	int k = GetNoOfCluster();

	double objFuncJ=0;
	//for(i=0;i<GetDataSize() - k;i++){			//Loop on points, NOT clusters
	i = 0;
	while( i < GetDataSize() - k ){
		double dist=INF;
		double dist2 = INF;
		int kVal=0;
		int kVal2=0;
		value IthData;
		GetIthData(i, &IthData);
		for(j=0;j<k;j++){
			value v;
			GetIthCentroid(j, &v);
			double tempDist=data.distance(&IthData, &v);

			if (tempDist < dist2)
			{
				if(tempDist < dist){
					kVal2 = kVal;
					dist2 = dist;
					kVal=j;
					dist=tempDist;
				}
				else {
					kVal2 = j;
					dist2 = tempDist;
				}
			}
		}

		//AssignClustExtra(i,kVal2,dist2,kVal,dist);
		AssignClustExtra(i,kVal,dist,kVal2,dist2);
		//ADD TO CLUSTER KVAL
		if( !AddToClust(i,kVal) ){
			objFuncJ+=dist;
			i++;
			//printf("asdfsadflkkj\n");
		}
		//printf("Assigned %d\n",i );
		/**
		double res = 0.0; 
		int l;
		for(l=0;l<GetDataSize() - GetNoOfCluster();l++){			//Loop on points, NOT clusters
			value IthData, IthCentroid;
			GetIthData(l, &IthData);
			GetIthCentroid(GetClusterOf(l), &IthCentroid);
			res+=data.distance(&IthData, &IthCentroid);
		}
		if (res > prev) 
			printf("la8os.....res = %f,,, prev = %f\n",res, prev);
		prev = res;
		/**/

		//printf("%f\n",res);
	}
	//printf("---------------------------------------------------\n");
	return objFuncJ;
}
