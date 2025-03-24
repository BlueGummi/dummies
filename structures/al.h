#include "core.h"
#include <stddef.h>
#include <stdbool.h>
typedef struct {
    void *arena;
    size_t arena_size;
    void *arena_ptr;
    size_t arena_allocs;
    void **blocks;
    size_t block_size;
    size_t block_allocs;

} Allocator;
extern bool is_term;
Allocator *allocator_init(void);
void *allocator_arenalloc(Allocator *alloc, size_t size);
void *allocator_alloc(Allocator *alloc, size_t size);
void allocator_free(Allocator *alloc, void *address);
void allocator_destroy(Allocator **alloc_addr);
void allocator_info(Allocator *alloc);
