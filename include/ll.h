#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <core.h>
typedef struct ListNode {
    void *data;
    struct ListNode *next;
} ListNode;

typedef struct LinkedList {
    ListNode *head;
    NewFunction new_data;
    FreeFunction free_data;
    PrintFunction print_data;
    PrettyPrint print;
    ErrorFunction err;

    void (*append)(struct LinkedList *, void *);
    void *(*removeLast)(struct LinkedList *);
    void (*free)(struct LinkedList *);
} LinkedList;
LinkedList *linked_list_create(NewFunction n, FreeFunction f, PrintFunction p, ErrorFunction e);
void linked_list_append(LinkedList *list, void *data);
void *linked_list_remove_last(LinkedList *list);
void linked_list_pretty_print(LinkedList *list);
void linked_list_free(LinkedList *list);
#endif
