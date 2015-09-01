#ifndef __RANDOM_H__
#define __RANDOM_H__


/* 
 *Gets the range and the size of a matrix, and creates a matrix with numbers that follow uniform distribution
*/
void GetKUniform(int, int, int ,int* );

/*
 * Gets the range and returns a number that follows uniform distribution
 */
double GetUniform(int , unsigned long long int);

/*
 * Gets mu and sigma and returns a number that follows normal distribution
 */
double GetNormal(int, int);


#endif
