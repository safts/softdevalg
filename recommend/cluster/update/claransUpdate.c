#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "claransUpdate.h"
#include "../../input/random.h"
#include "../../dataStruct/data.h"


int ClaransUpdate(double (*assign)(), int* confNums)
{
	int q = confNums[3];
	int s = confNums[4]; 
	static double oldJ = -1;
	int k= GetNoOfCluster();
	int flag = 0;
	int i, j;
	int l,oldCentr,newCentr;
	//int q = (GetDataSize()- k)*k /sqrt(GetDataSize());
	int *subsetQ = malloc(sizeof(int)* q);
//	printf("|| q=%d , s=%d\n",q, s);
	double bestDJ = 0;
	int newClust = -1;
	int oldClust = -1;
	int x, uniSup = k*GetDataSize()-1;
	for (l = 0; l < s; ++l)
	{

		GetKUniform(q, 0, GetDataSize()-k, subsetQ);
		for (j = 0; j < q; ++j)
		{
			x = GetUniform(0,uniSup);
			oldCentr = x%k;
			newCentr = x/k;
			// for (i = 0; i < k; ++i)
			// {
				double dj = ComputeAproximateDJ(oldCentr, newCentr, subsetQ, q);
					//printf("%d\n", j);
				if (dj< bestDJ){
					flag = 1;
					newClust = newCentr;
					oldClust = oldCentr;
					bestDJ = dj;
					//printf("i = %d ,, s[j]=%d\n",i, subsetQ[j]);
					//SwapCentroid(subsetQ[j], i);
					//double j  = assign();
					//printf("dj===%f,,, j= %f\n", dj, j);
				}
			//}
		}
	}
	//double aaa = ComputeDJ(oldClust, newClust);
	//printf("aaa= === %f\n", aaa);
	free(subsetQ);
	if ( flag == 1 )//&& aaa <0)
	{
		SwapCentroid(newClust , oldClust);
		double j = assign(confNums[1],confNums[2]);
		if( oldJ != -1 && j > oldJ)
			return 0;
		oldJ = j;
//		printf("j====%f\n", j);
		return flag;
	}
	return 0;
}
