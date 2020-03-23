// George Tang, G01068335
// CS 262, Lab Section 201
// Project 3

// Macro Guard
#ifndef ITEMLIST_H
#define ITEMLIST_H

// Struct
typedef struct _node
{
    char *data; // Food Item Name
    struct _node *next;
} node;

typedef struct _itemList
{
    node *head; // Pointer to first food item for the order (alphabetical)
    int count; // Number of food items in the order
} itemList;

// Global constant
#define strSize (50)

// Prototypes
itemList *createItem();
int insert(char *str, itemList *s);
void printItems(itemList *s);
int strcmpi(char *s, char *t);
void *dmalloc(size_t size);
void cpystr(char *target, char *source);

#endif
