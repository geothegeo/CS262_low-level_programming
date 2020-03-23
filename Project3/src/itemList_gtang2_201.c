// George Tang, G01068335
// CS 262, Lab Section 201
// Project 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// header file
#include "itemList_gtang2_201.h"

// function to creating the whole food items LL
itemList *createItem()
{
    itemList *items = NULL;
	char* tempChar = NULL;
    int control = 1;

    // initialize the itemsList
    items = dmalloc(sizeof(itemList));
    items -> head = NULL;
    items -> count = 0;

    do
    {   
        // get the food item from user
        tempChar = dmalloc(sizeof(char) * strSize);
        printf("food item: ");
        fgets(tempChar, strSize, stdin);
        tempChar[strlen(tempChar)-1] = 0;
        printf("\n");
        // if the food item is empty, then end the loop
        if(strlen(tempChar) == 0)
        {
            control = 0;           
        }
        // if the food insert is successful, then increase count
        else if(insert(tempChar, items) == 0)
        {
            items -> count ++; 
        } 
        // free the char pointer malloc
        free(tempChar);             
    } while(control);
    
    return items;
}

// function that creates a node for the new food item and inserts it alphabetically into the LL
int insert(char *str, itemList *s)
{
    // create and initialize the node
    node *newNode = NULL;
    newNode = dmalloc(sizeof(node));
    newNode -> data = NULL;
    newNode -> data = dmalloc(sizeof(char) * strSize);
    cpystr(newNode -> data, str);
    newNode -> next = NULL;

    // if head is null, make newNode the head
    if(s -> head == NULL)
        s -> head = newNode;
    // or if they are the same, then return 1 for unsuccessful input
    else if(strcmpi(s -> head -> data, newNode -> data) == 0)
        return 1;
    // if head is larger, then make newNode point to it and set newNode as head
    else if(strcmpi(s -> head -> data, newNode -> data) > 0)
    {
        newNode -> next = s -> head;
        s -> head = newNode;
    }
    // else if find the node is smaller in value to the newNode and insert it after
    else
    {
        node *temp = s -> head;
        while((temp -> next != NULL) && (strcmpi(temp -> next -> data, newNode -> data) < 0))
        {
            temp = temp -> next;
        }
        newNode -> next = temp -> next;
        temp -> next = newNode;
    }  
    return 0; 
}

// function that formats and prints all the char lines
void printItems(itemList *s)
{
    // if head is null, indicate it
    if(s -> head == NULL)
        printf("no items!\n");
    // else go through each node and print it
    else
    {
        node *temp = s -> head;
        while(temp != NULL)
        {
            printf("%s \n", temp -> data);
            temp = temp -> next;
        }
    }
}

// compares strings for alphabetical ordering
int strcmpi(char *s, char *t)
{
    while (*s && tolower(*s) == tolower(*t))
    {
        s++;
        t++;
    }
    return tolower(*s) - tolower(*t);
}

// allocates memory with a check for successful allocation
void *dmalloc(size_t size)
{
    void *p = malloc(size);
    if (!p)
    {
        printf("memory allocation failed\n");
        exit(1);
    }
    return p;
}

// copy the cstring of the source pointer into the target pointer
void cpystr(char *target, char *source)
{
    while(*source)
    {
        *target = *source;        
        source++;        
        target++;
    }    
}


