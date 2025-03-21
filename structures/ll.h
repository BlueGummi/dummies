#include "core.h"
typedef struct ListNode {
    void *data;
    struct ListNode *next;
} ListNode;

typedef struct LinkedList {
    ListNode *head;
    NewFunction newFunc;
    FreeFunction freeFunc;
    PrintFunction printFunc;
    PrettyPrint print;

    void (*append)(struct LinkedList *, void *);
    void *(*removeLast)(struct LinkedList *);
    void (*free)(struct LinkedList *);
} LinkedList;
LinkedList *linked_list_create(NewFunction newFunc, FreeFunction freeFunc, PrintFunction printFunc);
void linked_list_append(LinkedList *list, void *data);
void *linked_list_remove_last(LinkedList *list);
void linked_list_pretty_print(LinkedList *list);
void linked_list_free(LinkedList *list);
