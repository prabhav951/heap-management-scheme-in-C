//Function defintions
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Heap.h"

//Function to create free_Node.
//returns pointer to free_Node.
free_Node* free_MakeNode(int address, int size)
{
    free_Node* nptr = (free_Node*)malloc(sizeof(free_Node));

    nptr->size = size;
    nptr->address = address;
    nptr->next = NULL;
    nptr->prev = NULL;

    return nptr;
}

//Function to create var_Node.
//returns pointer to var_Node.
var_Node* var_MakeNode(char* name, int address, int size)
{
    var_Node* nptr = (var_Node*)malloc(sizeof(var_Node));

    strcpy(nptr->name, name);
    nptr->size = size;
    nptr->address = address;
    nptr->next = NULL;
    nptr->prev = NULL;

    return nptr;
}

//Function to initialize a Heap with number of bytes = size
void intializeHeap(Heap *hptr, int size)
{
    hptr->size = size;
    hptr->freelist = free_MakeNode(0, hptr->size);
    hptr->varlist = NULL;
}

//Function to insert "nptr" into the free list, where headptr is dereferenced to get the head of the free list.
void free_sortedInsert(free_Node **headptr, free_Node *nptr)
{
    free_Node *ptr, *prev_node, *head;
	head = *headptr;
	nptr->next = nptr->prev = NULL;

	if(head == NULL)	//Empty list case
	{
	    head = nptr;
	}
	else
	{
	    ptr = head;
	    while(ptr->next != NULL && ptr->address < nptr->address)	//Skip all nodes having address lesser than nptr->address
	    {
	        ptr = ptr->next;
	    }
	    
	    if(ptr->next == NULL && ptr->address < nptr->address)  	//If inserted node address is more than address of all nodes in the list
	    {
	        ptr->next = nptr;
	        nptr->prev = ptr;
	    }
	    else
	    {
	        if(ptr == head) 	//If inserted node address is less than address of all nodes in the list
	        {
			    head = nptr;
	        }    
	        prev_node = ptr->prev;

			//Joining all links
	        if(prev_node != NULL)
	        {
	            prev_node->next = nptr;
	        }
	        nptr->prev = prev_node;
	        nptr->next = ptr;
	        ptr->prev = nptr;
	    }
	}
	*headptr = head;
}

//Function to insert "nptr" into the variable list, where headptr is dereferenced to get the head of the variable list.
void var_sortedInsert(var_Node **headptr, var_Node *nptr)
{
    var_Node *ptr, *prev_node, *head;
	head = *headptr;
	nptr->next = nptr->prev = NULL;

	if(head == NULL)	//Empty list case
	{
	    head = nptr;
	}
	else
	{
	    ptr = head;
	    while(ptr->next != NULL && ptr->address < nptr->address)	//Skip all nodes having address lesser than nptr->address
	    {
	        ptr = ptr->next;
	    }
	    
	    if(ptr->next == NULL && ptr->address < nptr->address)  	//If inserted node address is more than address of all nodes in the list
	    {
	        ptr->next = nptr;
	        nptr->prev = ptr;
	    }
	    else
	    {
	        if(ptr == head) 	//If inserted node address is less than address of all nodes in the list
	        {
			    head = nptr;
	        }    
	        prev_node = ptr->prev;

			//Joining all links
	        if(prev_node != NULL)
	        {
	            prev_node->next = nptr;
	        }
	        nptr->prev = prev_node;
	        nptr->next = ptr;
	        ptr->prev = nptr;
	    }
	}
	*headptr = head;
}

//Function to delete a node "p" which is present in the free list, where lpptr is dereferenced to get the head of the free list.
//return SUCCESS or FAILURE
status_code free_deleteNode(free_Node** lpptr, free_Node* p)
{
    status_code ret_val = SUCCESS;
    free_Node *prev_node, *next_node;
    
    if(p == NULL)
    {
        ret_val = FAILURE;
    }
    else
    {
        prev_node = p->prev;
        next_node = p->next;

        if(prev_node != NULL)   //If we are not deleting the first node
        {
            prev_node->next = next_node;
        }
        else    //If we are deleting the first node
        {
            *lpptr = next_node;
        }

        if(next_node != NULL)   //If we are not deleting the last node
        {
            next_node->prev = prev_node;
        }

        free(p);
    }

    return ret_val;
}

//Function to delete a node "p" which is present in the variable list, where lpptr is dereferenced to get the head of the variable list.
//return SUCCESS or FAILURE
status_code var_deleteNode(var_Node** lpptr, var_Node* p)
{
    status_code ret_val = SUCCESS;
    var_Node *prev_node, *next_node;
    
    if(p == NULL)
    {
        ret_val = FAILURE;
    }
    else
    {
        prev_node = p->prev;
        next_node = p->next;

        if(prev_node != NULL)   //If we are not deleting the first node
        {
            prev_node->next = next_node;
        }
        else    //If we are deleting the first node
        {
            *lpptr = next_node;
        }

        if(next_node != NULL)   //If we are not deleting the last node
        {
            next_node->prev = prev_node;
        }

        free(p);
    }

    return ret_val;
}

//Function to allocate memory according to first fit strategy.
//return SUCCESS or FAILURE
status_code firstFitMalloc(Heap *hptr, char* varName, int varSize)
{
    status_code sc = SUCCESS;
    free_Node *free_lptr, *fptr;
    var_Node *var_lptr, *vptr;

    free_lptr = hptr->freelist;
    fptr = free_lptr;
    var_lptr = hptr->varlist;
    vptr = var_lptr;
    

    int found = 0;

    while(fptr != NULL && !found)	//Search for the first fit block in the free list.
    {
        if(fptr->size >= varSize)
        {
            found = 1;
        }
        else
        {
            fptr = fptr->next;
        }
    }

    if(!found || varSize == 0)	// If not found, failed to allocate memory
    {
        sc = FAILURE;
    }
    else	// If found
	{
		var_Node *var_nptr = var_MakeNode(varName, fptr->address, varSize);		// make a new var_Node of same size and address, and given name
		var_sortedInsert(&var_lptr, var_nptr);	// and sorted insert into the varlist

		fptr->address = fptr->address + varSize;	//Updating the free block after partition
		fptr->size = fptr->size - varSize;

		if(fptr->size == 0)		//If size becomes 0, delete from free list
		{
			free_deleteNode(&free_lptr, fptr);
		}
	}

    hptr->freelist = free_lptr;
    hptr->varlist = var_lptr;
    return sc;
}

//Function to free memory
//return SUCCESS or FAILURE
status_code mfree(Heap *hptr, char *varName)
{
	status_code sc = SUCCESS;
	free_Node *free_lptr, *fptr, *free_prev_node;
    var_Node *var_lptr, *vptr;
	int found = 0;

    free_lptr = hptr->freelist;
    fptr = free_lptr;
    var_lptr = hptr->varlist;
    vptr = var_lptr;

	while(vptr != NULL && !found)	//Search the varlist for the var_Node of given name
	{
		if(strcmp(varName, vptr->name) == 0)
		{
			found = 1;
		}
		else
		{
			vptr = vptr->next;
		}
	}

	if(!found)	//If not found, failed to free memory
	{
		sc = FAILURE;
	}
	else	//If found
	{
		free_Node *free_nptr = free_MakeNode(vptr->address, vptr->size);	// make a new free_Node of same size and address.
		free_sortedInsert(&free_lptr, free_nptr);	//and sorted insert into free list
		var_deleteNode(&var_lptr, vptr);	//delete the var_Node from varlist

		//Merging free nodes
		free_Node *free_next_node, *free_prev_node;
		free_next_node = free_nptr->next;
		free_prev_node = free_nptr->prev;

		if(free_next_node != NULL)
		{
			if(free_nptr->address + free_nptr->size == free_next_node->address)	//If next free_Node is adjacent to the newly inserted free_Node
			{
				free_nptr->size = free_nptr->size + free_next_node->size;
				free_deleteNode(&free_lptr, free_next_node);
			}
		}
		if(free_prev_node != NULL)
		{
			if(free_prev_node->address + free_prev_node->size == free_nptr->address)	//If previous free_Node is adjacent to the newly inserted free_Node
			{
				free_prev_node->size = free_prev_node->size + free_nptr->size;
				free_deleteNode(&free_lptr, free_nptr);
			}
		}
	}

	hptr->freelist = free_lptr;
    hptr->varlist = var_lptr;
	return sc;
}

//Function to print the free list in address sorted manner.
void free_printList(free_Node *free_ptr)
{
	free_Node *ptr = free_ptr;
	printf("Free list display:\n");
	if(free_ptr == NULL)
	{
		printf("No free space\n");
	}
	else
	{
		printf("Start_Address\tEnd_Address\tSize\n");
		while(ptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\n", ptr->address, ptr->address + ptr->size - 1, ptr->size);
			ptr = ptr->next;
		}
	}
	printf("\n");
}

//Function to print the variable list in address sorted manner.
void var_printList(var_Node *var_lptr)
{
	var_Node *ptr = var_lptr;
	printf("Variable list display:\n");
	if(var_lptr == NULL)
	{
		printf("No allocated space\n");
	}
	else
	{
		printf("Start_Address\tEnd_Address\tSize\tVariable_name\n");
		while(ptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\t%s\n", ptr->address, ptr->address + ptr->size - 1, ptr->size, ptr->name);
			ptr = ptr->next;
		}
	}
	printf("\n");
}

//Function to print the heap blocks (Both allocated and free) in address sorted manner.
void printHeap(Heap *hptr)
{
	var_Node *vptr = hptr->varlist;
	free_Node *fptr = hptr->freelist;
	printf("Total heap space = %d bytes\n", hptr->size);
	printf("Heap memory display:\n");
	printf("Start_Address\tEnd_Address\tSize\tAllocation_Status\n");
	while(vptr != NULL && fptr != NULL)
	{
		if(vptr->address < fptr->address)
		{
			printf("%d\t\t%d\t\t%d\tAllocated\n", vptr->address, vptr->address + vptr->size - 1 ,vptr->size);
			vptr = vptr->next;
		}
		else
		{
			printf("%d\t\t%d\t\t%d\tFree\n", fptr->address, fptr->address + fptr->size - 1 ,fptr->size);
			fptr = fptr->next;
		}
	}

	if(vptr != NULL)
	{
		while(vptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\tAllocated\n", vptr->address, vptr->address + vptr->size - 1 ,vptr->size);
			vptr = vptr->next;
		}
	}
	else
	{
		while(fptr != NULL)
		{
			printf("%d\t\t%d\t\t%d\tFree\n", fptr->address, fptr->address + fptr->size - 1 ,fptr->size);
			fptr = fptr->next;
		}
	}
	printf("\n");
}

//Function to free both freelist and varlist completely
void freeHeap(Heap * hptr)
{
	free_Node *free_lptr = hptr->freelist;
	free_Node *fptr = free_lptr;
	var_Node *var_lptr = hptr->varlist;
	var_Node *vptr = var_lptr;

	while(free_lptr != NULL)
	{
		fptr = free_lptr;
		free_lptr = free_lptr->next;
		free(fptr);
	}

	while(var_lptr != NULL)
	{
		vptr = var_lptr;
		var_lptr = var_lptr->next;
		free(vptr);
	}

	hptr->freelist = NULL;
	hptr->varlist = NULL;
}