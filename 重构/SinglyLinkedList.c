#include <stdio.h>
#include <stdlib.h>
#include "genlib.h"
#include "SinglyLinkedList.h"

struct llist *list_new(void)
{
    struct llist *llist;

    llist = GetBlock(sizeof(struct llist));
    llist->size = 0;
    llist->head = NULL;
    return llist;
}

void list_add(struct llist *list, void *value)
{
    struct node *newnode;

    newnode = GetBlock(sizeof(struct node));
    newnode->value = value;
    newnode->next = list->head;
    list->head = newnode;
    list->size++;
}

void list_append(struct llist *list, void *value)
{
    struct node *newnode, **travel;

    newnode = GetBlock(sizeof(struct node));
    newnode->value = value;
    newnode->next = NULL;
    travel = &(list->head);
    while (*travel) {
        travel = &((*travel)->next);
    }
    *travel = newnode;
}

int list_size(struct llist *list)
{
    return list->size;
}

void list_delete(struct llist *list)
{
    struct node *temp;

    temp = list->head->next;
    free(list->head);
    list->head = temp;
}

void list_insert(struct llist *list, void *value, int (*Compare)(void *op1, void *op2))
{
    struct node **nodeP;
    struct node *newnode;

    nodeP = &list->head;
    while (*nodeP) {
        if (Compare(value, (*nodeP)->value) <= 0) break;
        nodeP = &(*nodeP)->next;
    }
    newnode = GetBlock(sizeof(struct node));
    newnode->value = value;
    newnode->next = *nodeP;
    *nodeP = newnode;
}

void list_destory(struct llist *list)
{
    while (list->head) {
        list_delete(list);
    }
    free(list);
}





