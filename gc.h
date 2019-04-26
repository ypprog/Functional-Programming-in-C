#ifndef GC_H
#define GC_H

typedef enum {
    LIST = 0,
    ENVOBJ,
    STANDARD,  // gc's an generic obj
    TYPE_COUNT
} TYPE;

void gc_mark(void *obj);
void gc_unmark(void *obj);
void gc_register(void *obj, TYPE type);
void gc_remove(void *obj);
void gc_init(void);
void gc_collect(void);

#endif
