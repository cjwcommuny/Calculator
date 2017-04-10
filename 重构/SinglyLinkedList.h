#ifndef _SinglyLinkedList_h
#define _SinglyLinkedList_h

struct node {
    void *value;
    struct node *next;
};

struct llist {
    struct node *head;
    int size;
};

struct llist *list_new(void);
void list_add(struct llist *list, void *value);
void list_append(struct llist *list, void *value);
int list_size(struct llist *list);
void list_delete(struct llist *list);
void list_destory(struct llist *list);
void list_insert(struct llist *list, void *value, int (*Compare)(void *op1, void *op2));

#endif