#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "random.h"



void GetKUniform(int K, int from, int to, int* randomArr){

	int i=0;
	for(i=0;i<K;i++){
		randomArr[i]=(int)GetUniform(from, to);
	}
}

double GetUniform(int from, unsigned long long int to){
    
    double uniform_num = from + (rand()/(RAND_MAX + 1.0))*(to-from+1);
    
    return uniform_num;
}

double GetNormal(int mu, int sigma){
    
    double U1, U2, W, mult;
    static double X1, X2;
    static int call = 0;

    if (call){
        call = 0;
        return (mu + sigma * (double) X2);
    }
 
    do{
        U1 = -1 + ((double) rand () / RAND_MAX) * 2;
        U2 = -1 + ((double) rand () / RAND_MAX) * 2;

        W = U1*U1 + U2*U2;

    }while (W>=1|| W==0);
    
    mult = sqrt ((-2 * log (W)) / W);
    X1 = U1 * mult;
    X2 = U2 * mult;

    call = 1;

    return (mu + sigma * (double) X1);
}

