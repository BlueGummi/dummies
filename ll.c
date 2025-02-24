typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

typedef struct LinkedList {
    ListNode* head;
    NewFunction newFunc;
    FreeFunction freeFunc;
    PrintFunction printFunc;
    PrettyPrint print;

    void (*append)(struct LinkedList*, void*);
    void* (*removeLast)(struct LinkedList*);
    void (*free)(struct LinkedList*);
} LinkedList;

void linked_list_append(LinkedList* list, void* data);
void* linked_list_remove_last(LinkedList* list);
void linked_list_pretty_print(LinkedList* list);
void linked_list_free(LinkedList* list);

LinkedList* linked_list_create(NewFunction newFunc, FreeFunction freeFunc, PrintFunction printFunc) {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->newFunc = newFunc;
    list->freeFunc = freeFunc;
    list->printFunc = printFunc;
    list->print = (PrettyPrint)linked_list_pretty_print;

    list->append = linked_list_append;
    list->removeLast = linked_list_remove_last;
    list->free = linked_list_free;

    return list;
}

void linked_list_append(LinkedList* list, void* data) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->data = list->newFunc(data);
    newNode->next = NULL;

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        ListNode* temp = list->head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void* linked_list_remove_last(LinkedList* list) {
    if (list->head == NULL) {
        printf("Linked List is empty!\n");
        return NULL;
    } else if (list->head->next == NULL) {
        void* data = list->head->data;
        free(list->head);
        list->head = NULL;
        return data;
    } else {
        ListNode* temp = list->head;
        while (temp->next->next != NULL) {
            temp = temp->next;
        }
        void* data = temp->next->data;
        free(temp->next);
        temp->next = NULL;
        return data;
    }
}

void linked_list_pretty_print(LinkedList* list) {
    printf("┌───────────────────────────────┐\n");
    printf("│ Linked List                   │\n");
    printf("├───────────────────────────────┤\n");
    ListNode* temp = list->head;
    while (temp != NULL) {
        printf("│ ");
        list->printFunc(temp->data);
        printf(" ");
        for (int j = 0; j < 25 - 2; j++) printf(" ");
        printf("│\n");
        temp = temp->next;
    }
    printf("└───────────────────────────────┘\n");
}

void linked_list_free(LinkedList* list) {
    ListNode* temp = list->head;
    while (temp != NULL) {
        ListNode* next = temp->next;
        list->freeFunc(temp->data);
        free(temp);
        temp = next;
    }
    free(list);
}
