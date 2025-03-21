void *new_int(void *data);
void free_int(void *data);
void print_int(void *data);
int compare_int(void *a, void *b);
typedef void *(*NewFunction)(void *);
typedef void (*FreeFunction)(void *);
typedef void (*PrintFunction)(void *);
typedef void (*PrettyPrint)(void *);
