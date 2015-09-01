#include <stdio.h>
#include <assert.h>
#include "dataHamming.h"
#include "data.h"


dataIF data;

int main(void)
{

	data.distance=Hamming;
	data.inRange=InRangeHamming;
	data.print=PrintDataHamming;
	bitStr a1 = (1<<10) + (1<<5);
	bitStr a2 = (1<<11) + (1<<5) + (1<<4);
	//printf("%llu,,,, %llu\n",&a1, &a2);
	//printf("%d\n",hamming(&a1,&a2));

	//printf("a1 %lld\n",a1);
	assert( Hamming(&a1, &a2) == 3);
	//printf("a1 %lld\n",a1);
	assert( data.inRange(&a1, &a2, 4) );
	assert( data.inRange(&a1, &a2, 2)  == 0);
	a2 = (1<<11) + (1<<5) + (1<<4) + (1<<2);

	assert( data.inRange(&a1, &a2, 3)  == 0);
	assert( Hamming(&a1, &a2) == 4);
	//PrintHamming(&a1);
	//PrintHamming(&a2);

	bitStr i = 1;
	bitStr a3 = (i<<33) + (i<<50);
	bitStr a4 = (i<<42) + (i<<50) + (i<<40);

	assert(Hamming(&a3, &a4) == 3);
	//printf("%d\n",Hamming(a3, a4));

	assert( Hi(&a1, 10) == 1);
	assert( Hi(&a1, 5) == 1);
	assert( Hi(&a1, 9) == 0);
	assert( Hi(&a1, 55) == 0);

	bitStr mporek = CharTobitStr("0010111010100100101010010001100000011001000101011111100001100100");
	assert( Hamming(&mporek, &mporek) == 0);
	printf("Data Hamming Test ok\n");
	return 0;
}



