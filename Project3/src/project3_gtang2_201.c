// George Tang, G01068335
// CS 262, Lab Section 201
// Project 3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Header files
#include "project3_gtang2_201.h"
#include "itemList_gtang2_201.h"

int main()  
{  	
	int stay = 0;
    ordernode *orderHead = NULL;
    ordernode *newOrder = NULL;

	stay = getUserChoice();
	while(stay) 
	{
		newOrder = dmalloc(sizeof(ordernode));		
		newOrder -> deliverTo = dmalloc(sizeof(char) * strSize);
		newOrder -> restaurant = dmalloc(sizeof(char) * strSize);

        // get deliver address
		printf("Delivery Address for new delivery order: ");
		fgets(newOrder -> deliverTo, strSize, stdin);
        (newOrder -> deliverTo)[strlen(newOrder -> deliverTo)-1] = 0;

        // get restuarant name
		printf("Restaurant from which to pick up food: ");
		fgets(newOrder -> restaurant, strSize, stdin);
        (newOrder -> restaurant)[strlen(newOrder -> restaurant)-1] = 0;
        
        // initilized the other two elements
        newOrder -> data = createItem();
        newOrder -> next = NULL;
        printf("\n");

        // insert the order into the order LL
        insertOrder(&orderHead, newOrder);
		stay = getUserChoice();
	}
    // print all the orders
    printOrders(&orderHead);
    // erase all the orders
    deleteOrders(&orderHead);

	return 0;
}

// function that makes sure that the user enters "Y"/"y" or "N"/"n"
int getUserChoice()
{
	char choice[20];
	do
	{
		printf("New delivery order? (y/n) ");
		fgets(choice, 20, stdin);
		choice[strlen(choice)-1] = '\0';
		printf("\n");
        // converting the user choice to an integer ...
		if(strcmpi("y", choice) == 0)
		{
			return 1;
		}
		else if(strcmpi("n", choice) == 0)
		{
			return 0;
		}
        // or give error message and run the loop again
		else
		{
            printf("Invalid Input. Try again.\n");
		}
	} while( 1 );
}

// function that inserts new orders to the beginning of the list
void insertOrder(ordernode **orderHead, ordernode *newOrder)
{
    // if head is null, make newOrder the head
    if(*orderHead == NULL)
        *orderHead = newOrder;
    // make newOrder the new head and point its next to he old head
    else
    {
        newOrder -> next = *orderHead;
        *orderHead = newOrder;
    }
}

// function that formats and prints all the char lines
void printOrders(ordernode **orderHead)
{
    // if head is null, indicate it
    if(*orderHead == NULL)
        printf("There are no food delivery to be made\n");
    // else go through each node and print it
    else
    {
        printf("List of Deliveries: \n\n");
        ordernode *temp = *orderHead;
        // until it's null, print its properties and print the list it countains using the printItems function
        while(temp != NULL)
        {
            printf("Delivery order from %s has %d food item(s) \n", temp -> restaurant, temp -> data -> count);
            printItems(temp -> data);
            printf("Deliver to: %s \n\n", temp -> deliverTo);
            temp = temp -> next;
        }
    }
}

// function that deletes the orders from the itemList nodes then the order node that contains it
void deleteOrders(ordernode **orderHead)
{
    // if head is null, do nothing
    if(*orderHead == NULL)
        return ;
    // delete each node from the beginning by freeing the memory spaces
    else
    {
        // order nodes
        ordernode *oHead = *orderHead;
        ordernode *oNext = NULL;
        // item nodes
        node *iHead = oHead -> data -> head;  
        node *iNext = NULL;    
        
        // while there are order nodes ...
        while(oHead != NULL)
        {    
            // and while there are no item nodes ...
            while(iHead != NULL)
            {              
                // hold the next item and free all the mallocs for the struct          
                iNext = iHead -> next;
                free(iHead -> data);
                free(iHead);                
                iHead = iNext;             
            }
            // hold the next order and free all the mallocs for the struct   
            oNext = oHead -> next;
            free(oHead -> data);
            free(oHead -> deliverTo);
            free(oHead -> restaurant);   
            free(oHead);
            oHead = oNext;
        }
    }
}