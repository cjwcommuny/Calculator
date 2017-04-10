#include "genlib.h"
#include "queue2.h"
#include "SinglyLinkedList.h"

struct queueADT *queue_new(void)
{
    struct queueADT *queue;

    queue = list_new();
    return queue;
}

void *dequeue(struct queueADT *queue)
{
	void *temp;
	
    temp = queue->head->value;
    list_delete(queue);
    return temp;
}

void enqueue(struct queueADT *queue, void *value)
{
    list_append(queue, value);
}

int queuelength(struct queueADT *queue)
{
    return list_size(queue);
}

void queue_destory(struct queueADT *queue)
{
    list_destory(queue);
}
