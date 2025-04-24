#include <ds.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

bool is_term = false;

void *new_int(void *data) {
    int *newInt = (int *) malloc(sizeof(int));
    *newInt = *(int *) data;
    return newInt;
}

void free_int(void *data) {
    free(data);
}

void print_int(void *data) {
    printf("%-6d", *(int *) data);
}
int compare_int(void *a, void *b) {
    return *(int *) a - *(int *) b;
}

void handle_error(char *c) {
    printf("%s\n", c);
}

int main() {
    Allocator *alloc = allocator_init();
    is_term = is_terminal();

    srand(time(NULL));

    HashMap *map =
        hashmap_create(10, new_int, free_int, print_int, handle_error);
    LinkedList *list = linked_list_create(new_int, free_int, print_int, handle_error);
    Queue *queue = queue_create(10, new_int, free_int, print_int, handle_error);
    BinaryTree *tree = binary_tree_create(new_int, free_int, print_int, handle_error);

    int num_operations = 2;
    for (int i = 0; i < num_operations; i++) {
        int operation = rand() % 2;
        int key = rand() % 100;
        int value = rand() % 100;

        if (operation == 0) {
            map->put(map, key, &value);
            printf("Inserted key %d with value %d into HashMap\n", key, value);
        } else {
            void *removedValue = map->get(map, key);
            if (removedValue != NULL) {
                printf("Removed key %d with value %d from HashMap\n", key, *(int *) removedValue);
                map->remove(map, key);
            } else {
                printf("Key %d not found in HashMap\n", key);
            }
        }
        map->print(map);
        if (operation == 0) {
            list->append(list, &value);
            printf("Appended %d to Linked List\n", value);
        } else {
            void *removedValue = list->removeLast(list);
            if (removedValue != NULL) {
                printf("Removed %d from Linked List\n", *(int *) removedValue);
                free_int(removedValue);
            }
        }
        list->print(list);
        if (operation == 0) {
            queue->enqueue(queue, &value);
            printf("Enqueued %d to Queue\n", value);
        } else {
            void *dequeuedValue = queue->dequeue(queue);
            if (dequeuedValue != NULL) {
                printf("Dequeued %d from Queue\n", *(int *) dequeuedValue);
                free_int(dequeuedValue);
            }
        }
        queue->print(queue);
        if (operation == 0) {
            tree->insert(tree, &value, compare_int);
            printf("Inserted %d into Binary Tree\n", value);
        } else {
            tree->remove(tree, &value, compare_int);
            printf("Removed %d from Binary Tree\n", value);
        }
        tree->print(tree);
    }

    printf("\n");

    map->free(map);
    list->free(list);
    queue->free(queue);
    tree->free(tree);

    int *p = allocator_arenalloc(alloc, sizeof(int));
    *p = 42;
    int *d = allocator_alloc(alloc, sizeof(int));
    *d = 33;
    allocator_info(alloc);
    allocator_destroy(&alloc);
    return 0;
}
