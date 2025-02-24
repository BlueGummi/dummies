#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void* new_int(void* data) {
    int* newInt = (int*)malloc(sizeof(int));
    *newInt = *(int*)data;
    return newInt;
}

void free_int(void* data) {
    free(data);
}

void print_int(void* data) {
    printf("%6d", *(int*)data);
}
int compare_int(void* a, void* b) {
    return *(int*)a - *(int*)b;
}

typedef void* (*NewFunction)(void*);
typedef void (*FreeFunction)(void*);
typedef void (*PrintFunction)(void*);
typedef void (*PrettyPrint)(void*);

#include "ll.c"
#include "q.c"
#include "bt.c"
#include "hm.c"


int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("%s: simple data structures from scratch in C\n", argv[0]);
        printf("Usage: %s <DS_ID>\n", argv[0]);
        printf("Data structures: 1 (HashMap), 2 (LinkedList), 3 (Queue), 4 (BinaryTree)\n");
        return 1;
    }

    int ds_id = atoi(argv[1]);
    if (ds_id < 1 || ds_id > 4) {
        printf("Invalid DS_ID. Choose between 1 and 4.\n");
        return 1;
    }

    srand(time(NULL));

    HashMap* map = NULL;
    LinkedList* list = NULL;
    Queue* queue = NULL;
    BinaryTree* tree = NULL;

    switch (ds_id) {
        case 1:
            map = hashmap_create(10, new_int, free_int, print_int);
            break;
        case 2:
            list = linked_list_create(new_int, free_int, print_int);
            break;
        case 3:
            queue = queue_create(10, new_int, free_int, print_int);
            break;
        case 4:
            tree = binary_tree_create(new_int, free_int, print_int);
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
                    map->put(map, key, &value);
                    printf("Inserted key %d with value %d into HashMap\n", key, value);
                } else {
                    void* removedValue = map->get(map, key);
                    if (removedValue != NULL) {
                        printf("Removed key %d with value %d from HashMap\n", key, *(int*)removedValue);
                        map->remove(map, key);
                    } else {
                        printf("Key %d not found in HashMap\n", key);
                    }
                }
                map->print(map);
                break;

            case 2: 
                if (operation == 0) {
                    list->append(list, &value);
                    printf("Appended %d to Linked List\n", value);
                } else {
                    void* removedValue = list->removeLast(list);
                    if (removedValue != NULL) {
                        printf("Removed %d from Linked List\n", *(int*)removedValue);
                        free_int(removedValue);
                    }
                }
                list->print(list);
                break;

            case 3: 
                if (operation == 0) {
                    queue->enqueue(queue, &value);
                    printf("Enqueued %d to Queue\n", value);
                } else {
                    void* dequeuedValue = queue->dequeue(queue);
                    if (dequeuedValue != NULL) {
                        printf("Dequeued %d from Queue\n", *(int*)dequeuedValue);
                        free_int(dequeuedValue);
                    }
                }
                queue->print(queue);
                break;

            case 4: 
                if (operation == 0) {
                    tree->insert(tree, &value);
                    printf("Inserted %d into Binary Tree\n", value);
                } else {
                    tree->remove(tree, &value, compare_int);
                    printf("Removed %d from Binary Tree\n", value);
                }
                tree->print(tree);
                break;
        }

        printf("\n");
    }

    switch (ds_id) {
        case 1:
            map->free(map);
            break;
        case 2:
            list->free(list);
            break;
        case 3:
            queue->free(queue);
            break;
        case 4:
            tree->free(tree);
            break;
    }

    return 0;
}
