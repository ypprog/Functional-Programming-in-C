#ifndef LIST_H
#define LIST_H
#include <stdbool.h>

typedef struct list list;

struct list {
    void *val;
    list *next;
};

list *new_item(void *v);
list *copy_item(list *i);
list *append(list *l, void *v);
void list_free(void *);

#endif
