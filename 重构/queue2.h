/*This library is construct by singly linked list*/

#ifndef _queue2_h
#define _queue2_h

#include "SinglyLinkedList.h"

#define queueADT llist

struct queueADT *queue_new(void);
void *dequeue(struct queueADT *queue);
void enqueue(struct queueADT *queue, void *value);
int queuelength(struct queueADT *queue);
int queuelength(struct queueADT *queue);
void queue_destory(struct queueADT *queue);

#endif
