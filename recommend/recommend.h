#include "./dataStruct/list.h"

void ReadDataRecommend(FILE* , int );

void NNLSHRecommend(FILE* ,int ,int, int, List,  int );
int ClustRecommend(FILE*, FILE* ,int*,int, int, List,int );

void KFoldValidateLSH( FILE* ,int , int, int, int );

void KFoldValidateCluster(FILE*, FILE* ,int*, int, int, List, int );

void SpecifyDatasetRecommend(char* ,int );
