//#include <stdio.h>
#include "lapackeIF.h"


double getFrobNorm(double* mol,int molSize){
	//				  MAJOR		   NORM_T   NCOL NROW ARR  LEAD_DIM
	return LAPACKE_dlange(LAPACK_COL_MAJOR, 'F', molSize, 3, mol, molSize);
}

void AddMatr(double* matr1, double* matr2, int size,int mult){
    cblas_daxpy(size, mult ,matr1,1,matr2, 1);
}

/*                    mol1      mol2        res       N         3*/
void MultiMatr(double *X, double* Y, double*RES, int dimx, int dimy){
				/* row_order      transform     transform     rowsA colsB K  alpha  a  lda  b  ldb beta c   ldc */
	cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans , dimy, dimy, dimx, 1.0, X, dimy, Y, dimy, 0.0, RES, dimy );
}

void CalcSVD(double* A, int dimx,int dimy,double* U, double* S, double* V){
	/*      jobu  jobvt    ...   ...   ... */

	double *superb = malloc( sizeof(double*) * (dimx> dimy? dimy: dimx) );
	LAPACKE_dgesvd( LAPACK_ROW_MAJOR, 'A', 'A', dimx, dimy, A, dimx,
			S, U, dimx, V, dimx, superb );
}

double DetMatr(double* Q, int dimx, int dimy){
	int INFO;
	int *IPIV = malloc(sizeof(int)*( dimx > dimy ? dimy : dimx));
	LAPACK_dgetrf(&dimx ,&dimy , Q, &dimx, IPIV, &INFO);
	double det = 1;
	int i;
	for (i = 0; i < (dimx > dimy ? dimy : dimx); ++i)
		det *= Q[i*dimy + i];
	return det;
}

void MultiMatrNoTrans(double *X, double* Y, double*RES, int dimx, int dimy){
				/* row_order      transform     transform     rowsA colsB K  alpha  a  lda  b  ldb beta c   ldc */
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans , dimx, dimy, dimy, 1.0, X, dimy, Y, dimy, 0.0, RES, dimy );
}
