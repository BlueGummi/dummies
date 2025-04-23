#ifndef HASHMAP_H
#define HASHMAP_H
#include "core.h"
typedef struct HashNode {
    int key;
    void *value;
    struct HashNode *next;
} HashNode;

typedef struct HashMap {
    HashNode **buckets;
    int capacity;
    NewFunction newFunc;
    FreeFunction freeFunc;
    PrintFunction printFunc;
    PrettyPrint print;
    ErrorFunction *err;

    void (*put)(struct HashMap *, int, void *);
    void *(*get)(struct HashMap *, int);
    void (*remove)(struct HashMap *, int);
    void (*free)(struct HashMap *);
} HashMap;
HashMap *hashmap_create(int capacity, NewFunction newFunc, FreeFunction freeFunc, PrintFunction printFunc);
void hashmap_put(HashMap *map, int key, void *value);
void *hashmap_get(HashMap *map, int key);
void hashmap_remove(HashMap *map, int key);
void hashmap_pretty_print(HashMap *map);
void hashmap_free(HashMap *map);
#endif
