#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef void (*PrintFunction)(void*); // pointer for printing generic data

// ==================== DYNAMIC ARRAY ====================
typedef struct {
    void** data;
    int size;
    int capacity;
    PrintFunction printFunc;
} DynamicArray;

DynamicArray* dynamic_array_create(int capacity, PrintFunction printFunc) {
    DynamicArray* arr = (DynamicArray*)malloc(sizeof(DynamicArray));
    arr->data = (void**)malloc(capacity * sizeof(void*));
    arr->size = 0;
    arr->capacity = capacity;
    arr->printFunc = printFunc;
    return arr;
}

void dynamic_array_append(DynamicArray* arr, void* item) {
    if (arr->size == arr->capacity) {
        arr->capacity *= 2;
        arr->data = (void**)realloc(arr->data, arr->capacity * sizeof(void*));
    }
    arr->data[arr->size++] = item;
}

void* dynamic_array_remove_last(DynamicArray* arr) {
    if (arr->size > 0) {
        return arr->data[--arr->size];
    } else {
        printf("Dynamic Array is empty!\n");
        return NULL;
    }
}

void dynamic_array_pretty_print(DynamicArray* arr) {
    printf("┌───────────────────────────────┐\n");
    printf("│ Dynamic Array (Size: %3d)     │\n", arr->size);
    printf("├───────────────────────────────┤\n");
    for (int i = 0; i < arr->size; i++) {
        printf("│ ");
        arr->printFunc(arr->data[i]);
        printf(" ");
        for (int j = 0; j < 25 - 2; j++) printf(" "); 
        printf("│\n");
    }
    printf("└───────────────────────────────┘\n");
}

// ==================== LINKED LIST ====================
typedef struct ListNode {
    void* data;
    struct ListNode* next;
} ListNode;

typedef struct {
    ListNode* head;
    PrintFunction printFunc;
} LinkedList;

LinkedList* linked_list_create(PrintFunction printFunc) {
    LinkedList* list = (LinkedList*)malloc(sizeof(LinkedList));
    list->head = NULL;
    list->printFunc = printFunc;
    return list;
}

void linked_list_append(LinkedList* list, void* data) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->data = data;
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

// ==================== STACK ====================
typedef struct {
    void** items;
    int top;
    int capacity;
    PrintFunction printFunc;
} Stack;

Stack* stack_create(int capacity, PrintFunction printFunc) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->items = (void**)malloc(capacity * sizeof(void*));
    stack->top = -1;
    stack->capacity = capacity;
    stack->printFunc = printFunc;
    return stack;
}

void stack_push(Stack* stack, void* item) {
    if (stack->top < stack->capacity - 1) {
        stack->items[++stack->top] = item;
    } else {
        printf("Stack Overflow!\n");
    }
}

void* stack_pop(Stack* stack) {
    if (stack->top >= 0) {
        return stack->items[stack->top--];
    } else {
        printf("Stack Underflow!\n");
        return NULL;
    }
}

void stack_pretty_print(Stack* stack) {
    printf("┌───────────────────────────────┐\n");
    printf("│ Stack (Top: %3d)              │\n", stack->top + 1);
    printf("├───────────────────────────────┤\n");
    for (int i = stack->top; i >= 0; i--) {
        printf("│ ");
        stack->printFunc(stack->items[i]);
        printf(" ");
        for (int j = 0; j < 25 - 2; j++) printf(" ");
        printf("│\n");
    }
    printf("└───────────────────────────────┘\n");
}

// ==================== QUEUE ====================
typedef struct {
    void** items;
    int front;
    int rear;
    int size;
    int capacity;
    PrintFunction printFunc;
} Queue;

Queue* queue_create(int capacity, PrintFunction printFunc) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->items = (void**)malloc(capacity * sizeof(void*));
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    queue->capacity = capacity;
    queue->printFunc = printFunc;
    return queue;
}

void queue_enqueue(Queue* queue, void* item) {
    if (queue->size < queue->capacity) {
        queue->rear = (queue->rear + 1) % queue->capacity;
        queue->items[queue->rear] = item;
        queue->size++;
    } else {
        printf("Queue Overflow!\n");
    }
}

void* queue_dequeue(Queue* queue) {
    if (queue->size > 0) {
        void* item = queue->items[queue->front];
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size--;
        return item;
    } else {
        printf("Queue Underflow!\n");
        return NULL;
    }
}

void queue_pretty_print(Queue* queue) {
    printf("┌───────────────────────────────┐\n");
    printf("│ Queue (Size: %3d)             │\n", queue->size);
    printf("├───────────────────────────────┤\n");
    for (int i = 0; i < queue->size; i++) {
        int index = (queue->front + i) % queue->capacity;
        printf("│ ");
        queue->printFunc(queue->items[index]);
        printf(" ");
        for (int j = 0; j < 25 - 2; j++) printf(" "); 
        printf("│\n");
    }
    printf("└───────────────────────────────┘\n");
}

// ==================== BINARY TREE ====================
typedef struct TreeNode {
    void* data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

typedef struct {
    TreeNode* root;
    PrintFunction printFunc;
} BinaryTree;

TreeNode* tree_node_create(void* data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

void tree_inorder(TreeNode* root, PrintFunction printFunc) {
    if (root == NULL) return;
    tree_inorder(root->left, printFunc);
    printFunc(root->data);
    printf(" ");
    tree_inorder(root->right, printFunc);
}

void binary_tree_pretty_print(TreeNode* root, int space, PrintFunction printFunc) {
    if (root == NULL) return;

    space += 5;

    binary_tree_pretty_print(root->right, space, printFunc);

    printf("\n");
    for (int i = 5; i < space; i++) printf(" ");
    printFunc(root->data);
    printf("\n");

    binary_tree_pretty_print(root->left, space, printFunc);
}

void binary_tree_print(BinaryTree* tree) {
    printf("┌───────────────────────────────┐\n");
    printf("│ Binary Tree (Inorder)         │\n");
    printf("├───────────────────────────────┤\n");
    tree_inorder(tree->root, tree->printFunc);
    printf("\n");
    printf("└───────────────────────────────┘\n");
}
// ==================== HASH MAP ====================
typedef struct HashNode {
    int key;
    void* value;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode** buckets;
    int capacity;
    PrintFunction printFunc;
} HashMap;

HashMap* hashmap_create(int capacity, PrintFunction printFunc) {
    HashMap* map = (HashMap*)malloc(sizeof(HashMap));
    map->buckets = (HashNode**)calloc(capacity, sizeof(HashNode*));
    map->capacity = capacity;
    map->printFunc = printFunc;
    return map;
}

int hash(int key, int capacity) {
    return key % capacity;
}

void hashmap_put(HashMap* map, int key, void* value) {
    int index = hash(key, map->capacity);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->key = key;
    newNode->value = value;
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
}

void* hashmap_get(HashMap* map, int key) {
    int index = hash(key, map->capacity);
    HashNode* current = map->buckets[index];
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void hashmap_remove(HashMap* map, int key) {
    int index = hash(key, map->capacity);
    HashNode* current = map->buckets[index];
    HashNode* prev = NULL;
    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                map->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void hashmap_pretty_print(HashMap* map) {
    printf("┌───────────────────────────────┐\n");
    printf("│ Hash Map                      │\n");
    printf("├───────────────────────────────┤\n");
    for (int i = 0; i < map->capacity; i++) {
        HashNode* current = map->buckets[i];
        while (current != NULL) {
            printf("│ Key: %3d, Value: ", current->key);
            map->printFunc(current->value);
            printf(" ");
            for (int j = 0; j < 6; j++) printf(" ");
            printf("│\n");
            current = current->next;
        }
    }
    printf("└───────────────────────────────┘\n");
}

// ==================== MAIN ====================
void print_int(void* data) {
    printf("%6d", *(int*)data);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("%s: simple data structures from scratch in C\n", argv[0]);
        printf("Usage: %s <DS_ID>\n", argv[0]);
        printf("Data structures: 1 (HashMap), 2 (LinkedList), 3 (Stack), 4 (Queue), 5 (BinaryTree)\n");
        return 1;
    }

    int ds_id = atoi(argv[1]);
    if (ds_id < 1 || ds_id > 5) {
        printf("Invalid DS_ID. Choose between 1 and 5.\n");
        return 1;
    }

    srand(time(NULL));

    HashMap* map = NULL;
    LinkedList* list = NULL;
    Stack* stack = NULL;
    Queue* queue = NULL;
    BinaryTree tree;
    tree.root = NULL;
    tree.printFunc = print_int;

    switch (ds_id) {
        case 1:
            map = hashmap_create(10, print_int);
            break;
        case 2:
            list = linked_list_create(print_int);
            break;
        case 3:
            stack = stack_create(10, print_int);
            break;
        case 4:
            queue = queue_create(10, print_int);
            break;
        case 5:
            tree.root = NULL;
            break;
    }

    int num_operations = 5;
    for (int i = 0; i < num_operations; i++) {
        int operation = rand() % 2;
        int key = rand() % 100;
        int value = rand() % 100;

        switch (ds_id) {
            case 1:
                if (operation == 0) {
                    int* newValue = (int*)malloc(sizeof(int));
                    *newValue = value;
                    hashmap_put(map, key, newValue);
                    printf("Inserted key %d with value %d into HashMap\n", key, value);
                } else {
                    void* removedValue = hashmap_get(map, key);
                    if (removedValue != NULL) {
                        printf("Removed key %d with value %d from HashMap\n", key, *(int*)removedValue);
                        hashmap_remove(map, key);
                        free(removedValue);
                    } else {
                        printf("Key %d not found in HashMap\n", key);
                    }
                }
                hashmap_pretty_print(map);
                break;

            case 2:
                if (operation == 0) {
                    int* newValue = (int*)malloc(sizeof(int));
                    *newValue = value;
                    linked_list_append(list, newValue);
                    printf("Appended %d to Linked List\n", value);
                } else {
                    void* removedValue = linked_list_remove_last(list);
                    if (removedValue != NULL) {
                        printf("Removed %d from Linked List\n", *(int*)removedValue);
                        free(removedValue);
                    }
                }
                linked_list_pretty_print(list);
                break;

            case 3:
                if (operation == 0) {
                    int* newValue = (int*)malloc(sizeof(int));
                    *newValue = value;
                    stack_push(stack, newValue);
                    printf("Pushed %d to Stack\n", value);
                } else {
                    void* poppedValue = stack_pop(stack);
                    if (poppedValue != NULL) {
                        printf("Popped %d from Stack\n", *(int*)poppedValue);
                        free(poppedValue);
                    }
                }
                stack_pretty_print(stack);
                break;

            case 4:
                if (operation == 0) {
                    int* newValue = (int*)malloc(sizeof(int));
                    *newValue = value;
                    queue_enqueue(queue, newValue);
                    printf("Enqueued %d to Queue\n", value);
                } else {
                    void* dequeuedValue = queue_dequeue(queue);
                    if (dequeuedValue != NULL) {
                        printf("Dequeued %d from Queue\n", *(int*)dequeuedValue);
                        free(dequeuedValue);
                    }
                }
                queue_pretty_print(queue);
                break;

            case 5:
                if (operation == 0) {
                    int* newValue = (int*)malloc(sizeof(int));
                    *newValue = value;
                    TreeNode* newNode = tree_node_create(newValue);
                    if (tree.root == NULL) {
                        tree.root = newNode;
                    } else {
                        TreeNode* current = tree.root;
                        while (1) {
                            if (value < *(int*)current->data) {
                                if (current->left == NULL) {
                                    current->left = newNode;
                                    break;
                                } else {
                                    current = current->left;
                                }
                            } else {
                                if (current->right == NULL) {
                                    current->right = newNode;
                                    break;
                                } else {
                                    current = current->right;
                                }
                            }
                        }
                    }
                    printf("Inserted %d into Binary Tree\n", value);
                } else {
                    printf("Binary Tree removal not implemented for simplicity.\n");
                }
                binary_tree_print(&tree);
                break;
        }

        printf("\n");
    }

    return 0;
}
