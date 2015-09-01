#ifndef LSH_H
#define LSH_H value

#include "data.h"

typedef struct LSH_t LSH;

/*
 * Builds structs and all L hash tables given L and K
 */
void BuildLSH(LSH**, int, int );

/*
 * Searches for nearest neighbour according to the algorithm
 */
double SearchANN(LSH* ,value, value**);

/*
 * Search for neighbours in range of radius and place them in a list
 */
void SearchRange(LSH* ,value , double, List);

/*
 * Destroys all structs for LSH 
 */
void DestroyLSH(LSH**);

void InitBarrierLSH(LSH*);

int MarkPointsOf(LSH* ,int , double );

#endif
