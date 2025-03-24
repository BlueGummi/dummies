#include <hm.h>
#include <stdio.h>
#include <stdlib.h>

HashMap *hashmap_create(int capacity, NewFunction newFunc, FreeFunction freeFunc, PrintFunction printFunc) {
    HashMap *map = (HashMap *) malloc(sizeof(HashMap));
    map->buckets = (HashNode **) calloc(capacity, sizeof(HashNode *));
    map->capacity = capacity;
    map->newFunc = newFunc;
    map->freeFunc = freeFunc;
    map->printFunc = printFunc;
    map->print = (PrettyPrint) hashmap_pretty_print;

    map->put = hashmap_put;
    map->get = hashmap_get;
    map->remove = hashmap_remove;
    map->free = hashmap_free;

    return map;
}

int hash(int key, int capacity) {
    return (31 * key) % capacity;
}

void hashmap_put(HashMap *map, int key, void *value) {
    int index = hash(key, map->capacity);
    HashNode *newNode = (HashNode *) malloc(sizeof(HashNode));
    newNode->key = key;
    newNode->value = map->newFunc(value);
    newNode->next = map->buckets[index];
    map->buckets[index] = newNode;
}

void *hashmap_get(HashMap *map, int key) {
    int index = hash(key, map->capacity);
    HashNode *current = map->buckets[index];
    while (current != NULL) {
        if (current->key == key) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

void hashmap_remove(HashMap *map, int key) {
    int index = hash(key, map->capacity);
    HashNode *current = map->buckets[index];
    HashNode *prev = NULL;
    while (current != NULL) {
        if (current->key == key) {
            if (prev == NULL) {
                map->buckets[index] = current->next;
            } else {
                prev->next = current->next;
            }
            map->freeFunc(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void hashmap_pretty_print(HashMap *map) {
    printf("╭───────────────────────────────╮\n");
    printf("│           Hashmap             │\n");
    printf("├─────────┬─────────────────────┤\n");
    printf("│  Keys   │  Values             │\n");
    printf("├─────────┼─────────────────────┤\n");
    for (int i = 0; i < map->capacity; i++) {
        HashNode *current = map->buckets[i];
        while (current != NULL) {
            printf("│  %-6d │  ", current->key);
            map->printFunc(current->value);
            printf(" ");
            for (int j = 0; j < 12; j++)
                printf(" ");
            printf("│\n");
            current = current->next;
        }
    }
    printf("╰─────────┴─────────────────────╯\n");
}

void hashmap_free(HashMap *map) {
    for (int i = 0; i < map->capacity; i++) {
        HashNode *current = map->buckets[i];
        while (current != NULL) {
            HashNode *next = current->next;
            map->freeFunc(current->value);
            free(current);
            current = next;
        }
    }
    free(map->buckets);
    free(map);
}
