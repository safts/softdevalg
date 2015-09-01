#ifndef __LAPACKE_IF__
#define __LAPACKE_IF__

#include <lapacke.h>
#include <cblas.h>



double getFrobNorm(double* ,int );

void AddMatr(double* , double* , int ,int );

void MultiMatr(double *, double* , double*, int , int );

void MultiMatrNoTrans(double *, double* , double*, int , int );

void CalcSVD(double*, int ,int ,double*, double*, double*);

double DetMatr(double*, int, int);

#endif
