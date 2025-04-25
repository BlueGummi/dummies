#include <al.h>
#include <colors.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Create an allocator
 */
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

/*
 * Allocate a region inside the allocator's arena
 */
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

/*
 * Allocate a node in the allocator
 */
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

/*
 * Free an allocated node in the allocator
 */
void allocator_free(Allocator *alloc, void *address) {
    if (address != NULL) {
        if (address >= alloc->arena && address <=
                                           (void *) (size_t) alloc->arena_size + (size_t) alloc->arena) {
            return;
        }
        free(address);
    }
    for (size_t i = 0; i < alloc->block_size; i++) {
        if (alloc->blocks[i] == address) {
            alloc->blocks[i] = NULL;
            break;
        }
    }
    if (alloc->block_size > 0) {
        alloc->block_size -= 1;
    }
    void **new_blocks = malloc(alloc->block_size);
    for (size_t i = 0; i < alloc->block_size; i++) {
        if (alloc->blocks[i] != NULL) {
            new_blocks[i] = alloc->blocks[i];
        }
    }
    alloc->block_size -= 1;
    free(alloc->blocks);
    alloc->blocks = new_blocks;
}

/*
 * Given an address to a pointer to an allocator, deallocate
 * all blocks and set the pointer to NULL
 */
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

/*
 * Print an allocator
*/
void allocator_info(Allocator *alloc) {
    printf("╭────────────────%s──────────────────╮\n", alloc == NULL ? "─" : "┬");
    if (alloc == NULL) {
        printf("│    allocator has been destroyed   │\n");
        goto footer;
    }
    printf("│ %sarena%s %saddress%s ╺╪╸%-16p │\n", ANSI_BLUE, ANSI_RESET,
           ANSI_MAGENTA, ANSI_RESET, alloc->arena);

    printf("│ %sarena%s %spointer%s ╺╪╸%-16p │\n", ANSI_BLUE, ANSI_RESET,
           ANSI_CYAN, ANSI_RESET, alloc->arena_ptr);

    printf("│ %sarena%s %ssize%s    ╺╪╸%-16zu │\n", ANSI_BLUE, ANSI_RESET,
           ANSI_BOLD, ANSI_RESET, alloc->arena_size);

    printf("│ %sarena%s %stotal%s   ╺╪╸%-16zu │\n", ANSI_BLUE, ANSI_RESET,
           ANSI_UNDERLINE, ANSI_RESET, alloc->arena_allocs);

    if (alloc->arena_ptr > alloc->arena) {
        printf("├──────%saddr%s──────┼────%sbyte%s─%svalue%s────┤\n",
               ANSI_CYAN, ANSI_RESET, ANSI_GREEN, ANSI_RESET, ANSI_GREEN, ANSI_RESET);

        for (void *i = alloc->arena; i < alloc->arena_ptr; i++) {
            printf("│ %s%-14p%s │ %s0x%-14hhx%s │\n", ANSI_CYAN, i,
                   ANSI_RESET, ANSI_GREEN, *(uint8_t *) i, ANSI_RESET);
        }
        printf("├────────────────┼──────────────────┤\n");
    }
    printf("│ %sblock%s %saddress%s ╺╪╸%-16p │\n", ANSI_YELLOW, ANSI_RESET,
           ANSI_MAGENTA, ANSI_RESET, alloc->blocks);

    printf("│ %sblock%s %ssize%s    ╺╪╸%-16zu │\n", ANSI_YELLOW, ANSI_RESET,
           ANSI_BOLD, ANSI_RESET, alloc->block_size);

    printf("│ %sblock%s %stotal%s   ╺╪╸%-16zu │\n", ANSI_YELLOW, ANSI_RESET,
           ANSI_UNDERLINE, ANSI_RESET, alloc->block_allocs);

    if (alloc->blocks != NULL) {
        if (alloc->block_size != 0) {
            printf("├────────────────┼──────────────────┤\n");
            for (size_t i = 0; i < alloc->block_size; i++) {
                printf("│ %sblock%s %-8zu │ %-16p │\n", ANSI_YELLOW, ANSI_RESET, i, alloc->blocks[i]);
            }
        }
    }
footer:
    printf("╰────────────────%s──────────────────╯\n", alloc->blocks == NULL ? "─" : "┴");
}
