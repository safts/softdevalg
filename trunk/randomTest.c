#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "random.h"

int main(){

    int i;
    srand(time(NULL));

    for(i=0; i<20; i++){

        printf("Uniform: %d\n",(int)GetUniform(0,10));
        printf("Normal:  %f\n",GetNormal(0,1));
    }
    return 0;
        
}
