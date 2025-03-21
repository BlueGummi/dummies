#include <stdio.h>
#include <stdlib.h>
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

typedef void *(*NewFunction)(void *);
typedef void (*FreeFunction)(void *);
typedef void (*PrintFunction)(void *);
typedef void (*PrettyPrint)(void *);
