#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "../input/dataHamming.h"
#include "data.h"

typedef struct node_t {
	void *value;
	struct node_t *next;
	struct node_t *previous;
}Node;

struct list_t {

	Node *head;
	Node *tail;
	Node *curr;
	Node *barrier;
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
	temp->barrier=NULL;
}

void InsertValueList(ListPrivate *list, void* newValue){


	
	Node *newNode = malloc( sizeof(Node) );
	newNode->value = malloc( sizeof(char) * list->valueSize ); 
	memcpy( newNode->value, newValue, sizeof(char) * list->valueSize);

	if(list->size == 0){
		newNode->previous = NULL;
		list->head = newNode;
		list->tail = newNode;
		newNode->next = NULL;
		list->size++;

	}else{
		newNode->previous=list->tail;
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

void* GetFirstBarrier(ListPrivate* list){
	if(list->head==list->barrier)
		return NULL;
	list->curr=list->head;
	return list->curr->value;
}

void* GetNextBarrier(ListPrivate* list){
	if(list->curr == NULL || list->curr == list->barrier)
		return NULL;
	list->curr = list->curr->next;
	if(list->curr == list->barrier)
		return NULL;
	return list->curr->value;
}

void AddToBarrier(ListPrivate* list){
	if(list->barrier==NULL){
		if ( list->curr == list->tail)
		{
			list->barrier = list->tail;
			return;
		}
		list->barrier=list->curr;

		if(list->barrier != list->head){
			list->curr = list->barrier->previous;
			list->barrier->previous->next=list->barrier->next;
		}
		else{
			list->head=list->head->next;
			list->curr = list->head;
		}

		if(list->barrier != list->tail)
			list->barrier->next->previous=list->barrier->previous;


		list->barrier->previous = list->tail;
		list->barrier->next = NULL;
		list->tail->next=list->barrier;
		list->tail=list->barrier;
	}
	else{
		Node *prev;
		list->tail->next=list->curr;

		if(list->curr != list->head){
			prev = list->curr->previous;
			list->curr->previous->next=list->curr->next;
		}
		else{
			list->head=list->head->next;
			prev = list->head;
		}

		list->curr->next->previous = list->curr->previous;
		list->curr->next= NULL;
		list->curr->previous=list->tail;

		list->tail->next=list->curr;

		list->tail=list->tail->next;
		list->curr = prev;
	}
}

void InitBarrier(ListPrivate* list){
	list->barrier=NULL;
}

void* SearchList(ListPrivate *list, void* value){

    Node *curr = list->head;
    while(curr != NULL){

        if(list->distance(curr->value,value)<0.00000000001)
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
                list->head->previous=NULL;
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
                curr->next->previous = previous;
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

void ListToArr(ListPrivate* list, void** arr){

	int i=0;
	Node* temp=list->head;
	while(temp!=NULL){
		arr[i]=temp->value;
		i++;
		temp=temp->next;
	}
}

int ListToArrExcept(ListPrivate* list, void* val, void** arr){

	int i=0;
	Node* temp=list->head;
	while(temp!=NULL){
		if(list->distance(temp->value,val)>0.0000001){
			arr[i]=temp->value;
			i++;
		}
		temp=temp->next;
	}
	return i;
}
