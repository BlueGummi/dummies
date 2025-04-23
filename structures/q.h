#ifndef QUEUE_H
#define QUEUE_H
#include "core.h"
typedef struct Queue {
    void **items;
    int front;
    int rear;
    int size;
    int capacity;
    NewFunction newFunc;
    FreeFunction freeFunc;
    PrintFunction printFunc;
    PrettyPrint print;
    ErrorFunction *err;

    void (*enqueue)(struct Queue *, void *);
    void *(*dequeue)(struct Queue *);
    void (*free)(struct Queue *);
} Queue;

void queue_enqueue(Queue *queue, void *item);
void *queue_dequeue(Queue *queue);
void queue_pretty_print(Queue *queue);
void queue_free(Queue *queue);
Queue *queue_create(int capacity, NewFunction newFunc, FreeFunction freeFunc, PrintFunction printFunc);
#endif
