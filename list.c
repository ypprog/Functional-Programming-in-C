#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "gc.h"

static list *prepare_item(void)
{
    list *item = (list *) malloc(sizeof(list));
    if (!item)
        exit(1);
    item->val = NULL;
    item->next = NULL;
    return item;
}

list *new_item(void *v)
{
    list *o = prepare_item();
    o->val = v;
    gc_register((void *) o, LIST);
    return o;
}

list *copy_item(list *l)
{
    list *o = prepare_item();
    o->val = l->val;
    o->next = l->next;
    return o;
}

list *append(list *l, void *v)
{
    list *ni = new_item(v);
    if (!l)
        return ni;
    list *curr;
    for (curr = l; curr->next; curr = curr->next)
        ;
    curr->next = ni;
    return l;
}

list *concat(list *h, list *t)
{
    list *curr;
    for (curr = h; curr->next; curr = curr->next)
        ;
    curr->next = t;
    return h;
}

// shallow copy
list *copy(list *l)
{
    list *o = NULL;
    for (list *curr = l; curr; curr = curr->next)
        o = append(o, curr->val);
    return o;
}

// objs flag set to true will also free the objects in the list
void list_free(void *_l)
{
    list *l = _l;
    free(l);
}
