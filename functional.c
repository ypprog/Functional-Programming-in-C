#include "functional.h"
#include <stdio.h>
#include <stdlib.h>
#include "envobj.h"
#include "list.h"

// some standard functional programming functions
void iter(list *l, void (*fn)(void *, void *), void *args)
{
    for (list *curr = l; curr; curr = curr->next)
        (*fn)(curr->val, args);
}

list *map(list *l, void *(*fn)(void *, void *), void *args)
{
    list *o = NULL;
    for (list *curr = l; curr; curr = curr->next)
        o = append(o, (*fn)(curr->val, args));
    return o;
}

list *filter(list *l, bool (*fn)(void *, void *), void *args)
{
    list *o = NULL;
    for (list *curr = l; curr; curr = curr->next) {
        if ((*fn)(curr->val, args))
            o = append(o, copy_item(curr->val));
    }
    return o;
}

// not lazy
list *range(int start, int end)
{
    list *temp = NULL;
    for (int i = start; i <= end; i++) {
        int *aloc = malloc(sizeof(int));
        *aloc = i;
        temp = append(temp, (void *) aloc);
    }
    return temp;
}

list *rangeChar(char start, char end)
{
    if (start < 0x21 || end > 0x7F)
        return NULL;
    list *temp = NULL;
    for (char curr = start; curr <= end; curr++) {
        char *pcurr = (char *) malloc(sizeof(char));
        *pcurr = curr;
        temp = append(temp, (void *) pcurr);
    }
    return temp;
}
