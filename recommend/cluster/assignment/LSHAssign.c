#include <stdio.h>
#include <stdlib.h>

#include "../../LSH/LSH.h"
#include "../../dataStruct/list.h"
#include "LSHAssign.h"

extern dataIF data;

static LSH *lsh;



void BuidLSHAssign(LSH **lshptr,int L,int kLSH)
{

	BuildLSH(lshptr, L ,kLSH);			//kalo gia distance matrix
}

double LSHAssign(int L, int kLSH)
{
	int k = GetNoOfCluster();
	static int flag = 0;
	
	if (flag++ == 0) 
		BuidLSHAssign(&lsh,L,kLSH);
	int assigned = 0;
	int i=0;
	double threshold = MaxDistCendroids();
	InitAssigned();
	double radius = MinDistCendroids()/2;
//	List list,newList;
//	InitList(&list, sizeof(value), NULL, data.distance, NULL);
/**/
	while(assigned < GetDataSize() - k){
		int i;
		for (i = 0; i < k; ++i){
			assigned += MarkPointsOf(lsh, i, radius);
		}
		radius *=2;
		if(radius >=threshold*10)
			break;
	}
	//handle unassigned points
	//DestroyList(&list);
//	for(i=0;i<GetDataSize()-k;i++)
/**/
	double J =0.0;
	i = 0;
	while(i< GetDataSize() - k){
		//printf("%d\n", IsAssigned(i));
		if(IsAssigned(i) != 1)
			BFAssign(i);

		//printf("%f\n", JCurr);

		if(!AddToClust(i, GetExtrasCluster(i))){
			//printf("%Lf\n", JCurr);
			double JCurr = GetDistFirstCentr(i);
			J+= JCurr;
			i++;
		}

	}
	InitBarrierLSH(lsh);
	return J;
}



void BFAssign(int index){
	int i;
	double dist = 9999999999;
	double dist2 = 9999999999;
	int kVal = 0;
	int kVal2 = 0;
	value v1,v2;
	GetIthData(index, &v2);
	for(i = 0 ; i< GetNoOfCluster(); i++){
		GetIthCentroid(i, &v1);
		double tempDist = data.distance(&v1, &v2);

		if (tempDist < dist2) {
			if (tempDist < dist) {
				kVal2 = kVal;
				dist2 = dist;
				kVal = i;
				dist = tempDist;
			} else {
				kVal2 = i;
				dist2 = tempDist;
			}
		}
	}
	AssignClustExtra(index,kVal,dist,kVal2,dist2);
}


void DestroyLSHAssignment(){
	DestroyLSH(&lsh);
}
