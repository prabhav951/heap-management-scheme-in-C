//Driver program to implement Heap management scheme using DLL and first fit allocation strategy
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"Heap.h"

int main()
{
	char control_line;
    Heap myHeap;
    intializeHeap(&myHeap, 20);

	char varName[NAME_SIZE];
	int size;
	status_code sc;

	control_line = 'p';
	while(control_line != 'e')
	{
		printf("\n___________________________________________________________________________________________________________\n");
		printf("e					//Exit terminal. For Ex: \"e\"\n");
		printf("p					//Print Heap and lists. For Ex: \"p\"\n");
		printf("m [VariableName] [Size]			//Memory allocation with first fit strategy. For Ex: \"m a 40\"\n");
		printf("f [VariableName]			//Memory free. For Ex: \"f a\"\n");
		printf("\nEnter the proper syntax for the function you want to execute : ");
		scanf(" %c",&control_line);
		printf("\n");
		switch(control_line) 
		{
			case 'e':		//Exit terminal
			{
				printf("Terminal exited\n");
				break;
			}
			case 'p':		//Print Heap, free list and variable list
			{
				printHeap(&myHeap);
				free_printList(myHeap.freelist);
				var_printList(myHeap.varlist);
				break;
			}
			case 'm':		//Memory allocation with first fit strategy
			{
				scanf("%s", varName);
				scanf("%d", &size);
				sc = firstFitMalloc(&myHeap, varName, size);
				if(sc == SUCCESS)
					printf("Malloc Successful\n");
				else
					printf("Malloc unsuccessful\n");
				break;
			}
			case 'f':		//Memory free
			{
				scanf("%s", varName);
				sc = mfree(&myHeap, varName);
				if(sc == SUCCESS)
					printf("Memory free Successful\n");
				else
					printf("Memory free unsuccessful\n");
				break;
			}
			default:
			{
				printf("Syntax error\n");
				break;
			}
		}
	}
	freeHeap(&myHeap);
    return 0;
}