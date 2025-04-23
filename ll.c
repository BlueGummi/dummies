#include <inttypes.h>
#include <ll.h>
#include <stdio.h>
#include <stdlib.h>

LinkedList *linked_list_create(NewFunction n, FreeFunction f, PrintFunction p, ErrorFunction e) {
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));
    list->head = NULL;
    list->new_data = n;
    list->free_data = f;
    list->print_data = p;
    list->err = e;
    list->print = (PrettyPrint) linked_list_pretty_print;

    list->append = linked_list_append;
    list->removeLast = linked_list_remove_last;
    list->free = linked_list_free;

    return list;
}

void linked_list_append(LinkedList *list, void *data) {
    ListNode *newNode = (ListNode *) malloc(sizeof(ListNode));
    newNode->data = list->new_data(data);
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        ListNode *temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void *linked_list_remove_last(LinkedList *list) {
    if (list->head == NULL) {
        printf("Linked List is empty!\n");
        return NULL;
    } else if (list->head->next == NULL) {
        void *data = list->head->data;
        free(list->head);
        list->head = NULL;
        return data;
    } else {
        ListNode *temp = list->head;
        while (temp->next->next != NULL) {
            temp = temp->next;
        }
        void *data = temp->next->data;
        free(temp->next);
        temp->next = NULL;
        return data;
    }
}

void linked_list_pretty_print(LinkedList *list) {
    printf("╭───────────────────────────────╮\n");
    printf("│ Linked List                   │\n");
    printf("├─────────┬─────────────────────┤\n");
    printf("│  Index  │  Value              │\n");
    printf("├─────────┼─────────────────────┤\n");
    ListNode *temp = list->head;
    int i = 0;
    while (temp != NULL) {
        printf("│  %-5d  │  ", i);
        list->print_data(temp->data);
        for (int j = 0; j < 13; j++)
            printf(" ");
        printf("│\n");
        temp = temp->next;
        i++;
    }
    printf("╰─────────┴─────────────────────╯\n");
}

void linked_list_free(LinkedList *list) {
    ListNode *temp = list->head;
    while (temp != NULL) {
        ListNode *next = temp->next;
        list->free_data(temp->data);
        free(temp);
        temp = next;
    }
    free(list);
}
