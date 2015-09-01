#ifndef LIST_H
#define LIST_H value


typedef struct list_t* List;

/*
 * Initializes list an passes fuctions at data members
 */
void InitList(List *, int, void (*print)(), double (*distance)(),
	   	void (*destroyValue)());

/*
 * Inserts value at list
 */
void InsertValueList(List , void*);

/* 
 * Searches list for value
 * Returns value if it will be found,
 * NULL otherwise
 */
void* SearchList(List , void*);

/*
 * Delete a value from list if it will be found
 */
void DeleteValueList(List , void*);

/*
 * Delete all values from list, if it is not empty
 */
void DeleteAllFromList(List );

/*
 * Deduplicates list
 */
void DeduplicateList(List , List );

/*
 * Destroys list
 */
void DestroyList(List *);

/*
 * Prints List
 */
void PrintList(List );

/*
 * write list to file
 **/
void PrintListFile(List list, FILE* );

/*
 * Returns the first element of the list
 */
void* GetFirst(List );

/* 
 * Returns the next element 
 * of the position of the iterator
 */
void* GetNext(List );

/*
 * Returns size of list
 */
int SizeList(List );

void* GetFirstBarrier(List );

void* GetNextBarrier(List );

void AddToBarrier(List );

void DestroyBarrier(List );

void ListToArr(List, void** );

int ListToArrExcept(List, void* ,void** );


#endif
