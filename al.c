#include "al.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

Allocator *allocator_init(void) {
    Allocator *alloc = malloc(sizeof(Allocator));
    alloc->arena_size = 4096;
    alloc->arena = malloc(alloc->arena_size);
    alloc->arena_ptr = alloc->arena;
    alloc->block_allocs = 0;
    alloc->blocks = NULL;
    alloc->block_size = 0;
    return alloc;
}

void *allocator_arenalloc(Allocator *alloc, size_t size) {
    if (size > alloc->arena_size) {
        alloc->arena_size += size;
        alloc->arena = realloc(alloc->arena, alloc->arena_size);
    }
    void *returned = alloc->arena_ptr;
    alloc->arena_ptr = (void *) (size_t) alloc->arena_ptr + size;
    alloc->arena_allocs += 1;
    return returned;
}

void *allocator_alloc(Allocator *alloc, size_t size) {
    alloc->block_allocs += 1;
    if (alloc->blocks == NULL) {
        alloc->blocks = malloc(alloc->block_size * (sizeof(void *)));
    } else {
        alloc->blocks = realloc(alloc->blocks,
                                alloc->block_size * (sizeof(void *)));
    }

    alloc->blocks[alloc->block_size] = malloc(size);
    void *returned = alloc->blocks[alloc->block_size];
    alloc->block_size += 1;
    return returned;
}

void allocator_free(Allocator *alloc, void *address) {
    if (address != NULL) {
        if (address >= alloc->arena && address <=
                                           (void *) (size_t) alloc->arena_size + (size_t) alloc->arena) {
            return;
        }
        free(address);
    }
    alloc->block_allocs -= 1;
    for (size_t i = 0; i < alloc->block_size; i++) {
        if (alloc->blocks[i] == address) {
            alloc->blocks[i] == NULL;
            break;
        }
    }
    void **new_blocks = malloc(alloc->block_size - 1);
    for (size_t i = 0; i < alloc->block_size; i++) {
        if (alloc->blocks[i] != NULL)
            new_blocks[i] = alloc->blocks[i];
    }
    alloc->block_size -= 1;
    free(alloc->blocks);
    alloc->blocks = new_blocks;
}

void allocator_destroy(Allocator **alloc_addr) {
    Allocator *alloc = *alloc_addr;
    for (size_t i = 0; i < alloc->block_size; i++) {
        if (alloc->blocks[i] != NULL) {
            free(alloc->blocks[i]);
            alloc->blocks[i] = NULL;
        }
    }
    free(alloc->arena);
    free(alloc->blocks);

    alloc->arena = NULL;
    alloc->arena_ptr = NULL;
    alloc->arena_size = 0;
    alloc->arena_allocs = 0;

    alloc->blocks = NULL;
    alloc->block_allocs = 0;
    alloc->block_size = 0;
    free(alloc);
    alloc = NULL;
    *alloc_addr = NULL;
}

void allocator_info(Allocator *alloc) {
    printf("+------ allocator information ------+\n");
    printf("|                                   |\n");
    if (alloc == NULL) {
        printf("|    allocator has been destroyed   |\n");
        goto footer;
    }
    printf("| arena location : %-16p |\n", alloc->arena);
    printf("| arena pointer  : %-16p |\n", alloc->arena_ptr);
    printf("| arena size     : %-16zu |\n", alloc->arena_size);
    printf("| arena allocs   : %-16zu |\n", alloc->arena_allocs);
    if (alloc->arena_ptr > alloc->arena) {
        printf("+----------------+       value      |\n");
        for (void *i = alloc->arena; i < alloc->arena_ptr; i++) {
            printf("|  %-12p  +-> %-12hhu |\n", i, *(uint8_t *) i);
        }
        printf("+----------------+                  |\n");
    }
    printf("|                                   |\n");
    printf("| block pointer  : %-16p |\n", alloc->blocks);
    printf("| block total    : %-16zu |\n", alloc->block_size);
    printf("| block allocs   : %-16zu |\n", alloc->block_allocs);
    if (alloc->blocks != NULL) {
        if (alloc->block_size != 0) {
            printf("+----------------+      address     |\n");
            for (size_t i = 0; i < alloc->block_size; i++) {
                printf("| block %-8zu +-> %-14p |\n", i, alloc->blocks[i]);
            }
            printf("+----------------+                  |\n");
        }
    }
footer:
    printf("|                                   |\n");
    printf("+-----------------------------------+\n");
}
