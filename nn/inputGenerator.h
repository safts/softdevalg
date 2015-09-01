#ifndef __INPUT_GENERATOR__
#define __INPUT_GENERATOR__



char* generateInputHamming(int /*number*/,int /*length*/,
		int /*neighborhoods*/, double /*seed*/);

char* generateInputEuclidean(int /*number*/, int /*dimensions*/,
		int /*neighborhoods*/,double /*seed*/);

char* generateInputDistanceMatrix(int /*number*/, double /*seed*/);


#endif
