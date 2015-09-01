#include <stdio.h>
#include <stdlib.h>
#include "concentrate.h"
#include "../../dataStruct/data.h"


extern dataIF data;




double CalculateV(double**,double*, int);
void QuickSortDouble (double *,int* ,int );
void QuickSortInt(int * ,int );




void ConcentrateInit(int k){

	double **dm = malloc(sizeof(double*)* GetDataSize());	
	int i;
	for (i = 0; i < GetDataSize(); ++i)
		dm[i] = malloc(sizeof(double)* GetDataSize());
	int j;
	for (i = 0; i < GetDataSize(); ++i)
		for (j = i; j < GetDataSize(); ++j){
			value v1, v2;
			GetIthData(i, &v1);
			GetIthData(j, &v2);
			dm[i][j] = data.distance(&v1, &v2);
			dm[j][i] = dm[i][j];
		}
	double *v = malloc(sizeof(double)* GetDataSize());
	int *vPos = malloc(sizeof(int)* GetDataSize());

	double *lineSum = malloc(sizeof(double)* GetDataSize());
	for (i = 0; i < GetDataSize(); ++i){
		lineSum[i] = 0;
		for (j = 0; j < GetDataSize(); ++j)
			lineSum[i] += dm[i][j];
	}
	for (i = 0; i < GetDataSize(); ++i)
		vPos[i]=i;
	for (i = 0; i < GetDataSize(); ++i)
		v[i] = CalculateV(dm, lineSum, i);
	QuickSortDouble(v,vPos,GetDataSize());
	QuickSortInt(vPos,k);
	for (i = k-1; i >= 0; --i)
		AddCentroid(vPos[i]);
	for (i = 0; i < GetDataSize(); ++i)
	{
		free(dm[i]);
	}
	free(dm);
	free(v);
	free(vPos);
	free(lineSum);
}




double CalculateV(double** dm, double* lineSum, int i ){
	double v = 0;
	int j;
	for (j = 0; j < GetDataSize(); ++j){
			v += dm[i][j] / lineSum[j];
	}
	return v;
}


void QuickSortDouble (double *a,int* b, int n) {
    if (n < 2)
        return;
    double p = a[n / 2];
    double *l = a;
    int *ll =b;
    double *r = a + n - 1;
    int *rr = b +n -1;
    while (l <= r) {
        if (*l < p) {
            l++;
            ll++;
        }
        else if (*r > p) {
            r--;
            rr--;
        }
        else {
            double t = *l;
            double tt= *ll;
            *l = *r;
            *ll = *rr;
            *r = t;
            *rr = tt;
            l++;
            ll++;
            r--;
            rr--;
        }
    }
    QuickSortDouble(a,b, r - a + 1);
    QuickSortDouble(l,ll, a + n - l);
}

void QuickSortInt (int *a, int n) {
    if (n < 2)
        return;
    int p = a[n / 2];
    int *l = a;
    int *r = a + n - 1;
    while (l <= r) {
        if (*l < p) {
            l++;
        }
        else if (*r > p) {
            r--;
        }
        else {
            int t = *l;
            *l = *r;
            *r = t;
            l++;
            r--;
        }
    }
    QuickSortInt(a, r - a + 1);
    QuickSortInt(l, a + n - l);
}



