#include "core.h"
#include <q.h>
#include <stdio.h>
#include <stdlib.h>

Queue *queue_create(int capacity, NewFunction n, FreeFunction f, PrintFunction p, ErrorFunction e) {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    queue->items = (void **) malloc(capacity * sizeof(void *));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = capacity;
    queue->newFunc = n;
    queue->freeFunc = f;
    queue->printFunc = p;
    queue->err = e;

    queue->print = (PrettyPrint) queue_pretty_print;

    queue->enqueue = queue_enqueue;
    queue->dequeue = queue_dequeue;
    queue->free = queue_free;

    return queue;
}

void queue_enqueue(Queue *queue, void *item) {
    if (queue->size < queue->capacity) {
        queue->rear = (queue->rear + 1) % queue->capacity;
        queue->items[queue->rear] = queue->newFunc(item);
        queue->size++;
    } else {
        printf("Queue Overflow!\n");
    }
}

void *queue_dequeue(Queue *queue) {
    if (queue->size > 0) {
        void *item = queue->items[queue->front];
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return item;
    } else {
        printf("Queue Underflow!\n");
        return NULL;
    }
}

void queue_pretty_print(Queue *queue) {
    printf("╭───────────────────────────────╮\n");
    printf("│ Queue - Size: %-3d             │\n", queue->size);
    printf("├─────────┬─────────────────────┤\n");
    printf("│  Index  │  Value              │\n");
    printf("├─────────┼─────────────────────┤\n");
    for (int i = 0; i < queue->size; i++) {
        int index = (queue->front + i) % queue->capacity;
        printf("│  %-5d  │  ", index);
        queue->printFunc(queue->items[index]);
        printf(" ");
        for (int j = 0; j < 12; j++)
            printf(" ");
        printf("│\n");
    }
    printf("╰─────────┴─────────────────────╯\n");
}

void queue_free(Queue *queue) {
    for (int i = 0; i < queue->size; i++) {
        int index = (queue->front + i) % queue->capacity;
        queue->freeFunc(queue->items[index]);
    }
    free(queue->items);
    free(queue);
}
