#ifndef CORE_H
#define CORE_H
typedef void *(*NewFunction)(void *);
typedef void (*FreeFunction)(void *);
typedef void (*PrintFunction)(void *);
typedef void (*PrettyPrint)(void *);
typedef void (ErrorFunction)(char *);
#endif
