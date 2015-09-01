#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void print(int *a){
	printf("%d\n",*a);
}

int equal(int *a, int *b){
    if(*a==*b)
        return 1;
    else
        return 0;
}


typedef struct _test_struct {
	int sss;
} test_struct;

void printStruct(test_struct *b)
{
	printf("%d\n",b->sss);
}
void printStructPointer(test_struct **b)
{
	printf("%d\n",(*b)->sss);
}

int equalStruct(test_struct *a, test_struct *b){
    if(a->sss == b->sss)
        return 1;
    else
        return 0;
}

int equalStructPointer(test_struct **a, test_struct **b){
    if((*a)->sss == (*b)->sss)
        return 1;
    else
        return 0;
}


int main(void)
{
	printf("test 1\n");
	List test;
	//test = malloc(sizeof(List));

	int a1 = 5;
	int a2 = 6;
	int a3 = 7;
    int a4 = 8;
	int a5 = 9;
	int a6 = 10;

	InitList(&test, sizeof(int), print,equal);
	InsertValueList(test, &a1);
	InsertValueList(test, &a2);
	InsertValueList(test, &a3);
    InsertValueList(test, &a4);
	InsertValueList(test, &a5);
	InsertValueList(test, &a6);
    if(SearchList(test,&a4))
        printf("FOUND!\n");
    else
        printf("NOT FOUND!\n");
	//PrintList(test);
    printf("after delete\n");
    DeleteValueList(test, &a1);
    DeleteValueList(test, &a4);
    DeleteValueList(test, &a6);
    DeleteValueList(test, &a5);
    PrintList(test);
    if(SearchList(test,&a4))
        printf("FOUND!\n");
    else
        printf("NOT FOUND!\n");
	DestroyList(&test);

	printf("test 2\n");
	List testStruct;
	//testStruct = malloc(sizeof(List));

	test_struct b1;
	b1.sss = 10;
	test_struct b2;
	b2.sss = 11;
	test_struct b3;
	b3.sss = 12;
    test_struct b4;
	b4.sss = 13;
    test_struct b5;
	b5.sss = 14;
    test_struct b6;
	b6.sss = 15;

	InitList(&testStruct, sizeof(test_struct), printStruct,equalStruct);
	InsertValueList(testStruct, &b1);
	InsertValueList(testStruct, &b2);
	InsertValueList(testStruct, &b3);
    InsertValueList(testStruct, &b4);
    InsertValueList(testStruct, &b5);
    InsertValueList(testStruct, &b6);
	PrintList(testStruct);
	DestroyList(&testStruct);

	printf("test 3\n");
	List testPointer;
	//testPointer = malloc(sizeof(List));

	test_struct *c1 = malloc(sizeof(test_struct));
	c1->sss = 15;
	test_struct *c2 = malloc(sizeof(test_struct));
	c2->sss = 16;
	test_struct *c3 = malloc(sizeof(test_struct));
	c3->sss = 17;
    test_struct *c4 = malloc(sizeof(test_struct));
	c4->sss = 18;
    test_struct *c5 = malloc(sizeof(test_struct));
	c5->sss = 19;
    test_struct *c6 = malloc(sizeof(test_struct));
	c6->sss = 20;

	InitList (&testPointer, sizeof(test_struct*), printStructPointer, equalStructPointer);
	InsertValueList(testPointer, &c1);
	InsertValueList(testPointer, &c2);
	InsertValueList(testPointer, &c3);
    InsertValueList(testPointer, &c4);
	InsertValueList(testPointer, &c5);
	InsertValueList(testPointer, &c6);
    if(SearchList(testPointer,&c5))
        printf("FOUND!\n");
    else
        printf("NOT FOUND!\n");

	PrintList(testPointer);
    printf("after delete\n");
    DeleteValueList(testPointer, &c1);
    DeleteValueList(testPointer, &c4);
    DeleteValueList(testPointer, &c6);
    DeleteValueList(testPointer, &c5);
    PrintList(testPointer);
    if(SearchList(testPointer,&c5))
        printf("FOUND!\n");
    else
        printf("NOT FOUND!\n");

	DestroyList(&testPointer);
	free(c1);
	free(c2);
	free(c3);
    free(c4);
	free(c5);
	free(c6);
	return 0;
}
