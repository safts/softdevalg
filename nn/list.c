#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "dataHamming.h"
#include "data.h"

typedef struct node_t {
	void *value;
	struct node_t *next;
}Node;

struct list_t {
	Node *head;
	Node *tail;
	Node *curr;
	int size;
	int valueSize;
	void (*print)();
    double (*distance)();
	void (*destroyValue)();
};

typedef struct list_t ListPrivate;

void InitList(ListPrivate **list, int valueSize, void (*print)(), 
		double(*distance)(), void (*destroyValue)()){

	*list = malloc(sizeof(ListPrivate));
	ListPrivate *temp = *list;
	temp->head = NULL;
	temp->tail = NULL;
	temp->curr = NULL;
	temp->size = 0;
	temp->valueSize = valueSize;
	temp->print = print;
	temp->distance = distance;
	temp->destroyValue = destroyValue;
}

void InsertValueList(ListPrivate *list, void* newValue){


	
	Node *newNode = malloc( sizeof(Node) );
	newNode->value = malloc( sizeof(char) * list->valueSize ); 
	memcpy( newNode->value, newValue, sizeof(char) * list->valueSize);
    

	if(list->size == 0){
		list->head = newNode;
		list->tail = newNode;
		newNode->next = NULL;
		list->size++;

	}else{
		list->tail->next = newNode;
		list->tail = newNode;
		newNode->next = NULL;
		list->size++;
	}

}

void* GetFirst(ListPrivate* list){

	if(list->head==NULL)
		return NULL;
	list->curr=list->head;
	return list->curr->value;
}

void* GetNext(ListPrivate* list){

	if(list->curr == NULL)
		return NULL;
	list->curr = list->curr->next;
	if(list->curr == NULL)
		return NULL;
	return list->curr->value;

}


void* SearchList(ListPrivate *list, void* value){

    Node *curr = list->head;
    while(curr != NULL){

        if(list->distance(curr->value,value)==0.0)
            return curr->value;
        curr = curr->next;
    }
    return NULL;     
}   


void DeleteValueList(ListPrivate *list, void *value){

    Node *curr = list->head;
    Node *previous;

    while(curr != NULL){

        if(list->distance(curr->value,value)==0){
            if(curr == list->head){
                list->head = curr->next;
                free(curr->value);
                free(curr);
                list->size--;
                break;
            }else if(curr == list->tail){
                list->tail = previous;
                list->tail->next =NULL;
                free(curr->value);
                free(curr);
                list->size--;
                break;
            }else{
                previous->next = curr->next;
                free(curr->value);
                free(curr);
                list->size--;
                break;
            }
        }

    previous = curr;
    curr = curr->next;
    }
}

        
void DeleteAllFromList(ListPrivate *list){
	int i;

	Node *curr = list->head;
	for(i = 0; i < list->size; i++){
		list->head = list->head->next;
		free(curr->value);
		free(curr);
        
		curr = list->head;
	}
    list->size=0;
}


void DeduplicateList(ListPrivate *old, ListPrivate *new){
    int i;
    int size = old->size;
    Node *curr = old->head;
    for(i=0; i<size; i++){
        if(SearchList(new,curr->value) == NULL){
            InsertValueList(new, curr->value);
        }
        curr = curr->next;
    }
}


void DestroyList(ListPrivate **list){
	int i;
    
	ListPrivate *temp = *list;
	if (temp == NULL) 
		return;
	Node *curr = temp->head;
	for(i = 0; i < temp->size; i++){
		temp->head = temp->head->next;
		if ( temp->destroyValue != NULL) 
			temp->destroyValue(curr->value);
		free(curr->value);
		free(curr);
		curr = temp->head;
	}
	free(*list);
}


void PrintList(ListPrivate *list){

	void* temp=GetFirst(list);
	value* v=temp;
	while(temp!=NULL){
		list->print(v);
		temp=GetNext(list);
		v=temp;
	}
}

void PrintListFile(ListPrivate *list, FILE* file){

	void* temp=GetFirst(list);
	value* v=temp;
	while(temp!=NULL){
		list->print(v,file);
		temp=GetNext(list);
		v=temp;
	}
}

int SizeList(ListPrivate* list){
	if(list!=NULL)
		return list->size;
	return 0;
}
