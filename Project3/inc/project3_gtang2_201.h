// George Tang, G01068335
// CS 262, Lab Section 201
// Project 3

// Macro Guard
#ifndef PROJECT3_H
#define PROJECT3_H

// Header files
#include "itemList_gtang2_201.h"

// Struct 
typedef struct _ordernode
{
	itemList *data;
	char *deliverTo;
	char *restaurant;
	struct _ordernode *next;
} ordernode;

// Global constant
#define strSize (50)

// Prototypes
int getUserChoice();
void insertOrder(ordernode **orderHead, ordernode *newOrder);
void printOrders(ordernode **orderHead);
void deleteOrders(ordernode **orderHead);

#endif
