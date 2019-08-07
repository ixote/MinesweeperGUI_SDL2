#ifndef _STRUCT
#define _STRUCT
#include "main.h"

typedef struct _node {
    void* value;
    struct _node* pre;
    struct _node* next;
} Node;

Node* initQueue()
{
    Node* head = (Node*)calloc(1, sizeof(Node));
    head->value = NULL;
    head->pre = head;
    head->next = NULL;
    return head;
}

void pushQueue(Node* head, void* node)
{
    Node* new = (Node*)calloc(1, sizeof(Node));
    new->value = node;
    new->next = NULL;
    new->pre = NULL;
    Node* temp = head->pre;
    temp->next = new;
    head->pre = new;
}
void pushStack(Node* head, void* node)
{
    Node* new = (Node*)calloc(1, sizeof(Node));
    new->value = node;
    Node* temp = head->next;
    head->next = new;
    new->next = temp;
}

void* popQueue(Node* head)
{
    void* value = NULL;
    Node* get = head->next;
    if (get) {
        value = (void*)get->value;
        head->next = get->next;
        free(get);
    } else {
        head->pre = head;
    }
    return value;
}

void drawQueen(Node* queen, void (*func)())
{
    while (queen->next) {
        if (queen->next->value == NULL) {
            popQueue(queen);
            continue;
        }
        func(popQueue(queen));
    }
}

#endif
