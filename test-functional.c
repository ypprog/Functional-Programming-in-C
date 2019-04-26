#include <stdio.h>
#include <stdlib.h>
#include "functional.h"
#include "gc.h"
#include "list.h"

// a function to play with iter
void print_int(void *v, void *args)
{
    printf("[ op:%s ] %d\n", (char *) args, (int) (*(int *) v));
}

void print_char(void *v, void *args)
{
    static int count = 0;
    printf("%c ", (char) (*(char *) v));
    if (++count > 10) {
        printf("\n");
        count = 0;
    }
}

// returns true if int v is isOdd, false otherwise
bool isOdd(void *v, void *args)
{
    return (bool) (*((int *) v) & 1);
}

// returns twice int v
void *twice(void *v, void *args)
{
    int *o = malloc(sizeof(int));
    *o = *((int *) v) * 2;
    return o;
}

int main(int argc, char **argv)
{
    gc_init();  // initialize the garbage collector

    iter(map(range(0, 10), twice, NULL), print_int,
         "twice"); /* 0, 2, 4, .., 20 */
    iter(filter(range(0, 10), isOdd, NULL), print_int,
         "odd");                                  /* 1, 3, 5, 7, 9 */
    iter(rangeChar('a', 'z'), print_char, NULL);  /* a, b, c, ..., z */
    iter(rangeChar(' ', 'z'), print_char, NULL);  // won't get printed
    iter(rangeChar('1', 'z'), print_char, NULL);

    gc_collect();

    return 0;
}
