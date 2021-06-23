//Declaration of structure Heap
#ifndef HEAP_H
#define HEAP_H

#define NAME_SIZE 10
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef enum{FAILURE, SUCCESS} status_code;

typedef struct free_Node_tag
{
    int address;
	int size;
    struct free_Node_tag* next;
    struct free_Node_tag* prev;
} free_Node;

typedef struct var_Node_tag
{
    char name[NAME_SIZE];
    int address;
	int size;
    struct var_Node_tag* next;
    struct var_Node_tag* prev;
} var_Node;

typedef struct Heap_tag
{
    int size;
    free_Node* freelist;
    var_Node* varlist;
} Heap;

free_Node* free_MakeNode(int address, int size);    //Creates free_Node
var_Node* var_MakeNode(char* name, int address, int size);  //Creates var_Node
void intializeHeap(Heap *hptr, int size);   //Initialize heap 

void free_sortedInsert(free_Node **headptr, free_Node *nptr);   //Inserts free_Node into freelist in a sorted manner
void var_sortedInsert(var_Node **headptr, var_Node *nptr);   //Inserts var_Node into varlist in a sorted manner
status_code free_deleteNode(free_Node** lpptr, free_Node* p);   //Deletes Node "p" from freelist
status_code var_deleteNode(var_Node** lpptr, var_Node* p);  //Deletes Node "p" from varlist

status_code firstFitMalloc(Heap *hptr, char* varName, int varSize); //Allocates memory of size "varSize" in the heap and alots that to variable name "varName"
status_code mfree(Heap *hptr, char *varName);   //Frees memory of space pointed by the variable name "varName" 
void free_printList(free_Node *free_ptr);   //Prints the freelist
void var_printList(var_Node *var_lptr); //Prints the varlist
void printHeap(Heap *hptr); //Prints all the heap blocks (allocated and free)
void freeHeap(Heap * hptr); //Frees both freelist and varlist completely

#endif