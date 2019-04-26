#ifndef ENVOBJ_H
#define ENVOBJ_H
#include <unistd.h>
#include "gc.h"
#include "list.h"

typedef struct {
    void *val;
    ssize_t size;
} envobj;

static inline envobj *env_item(void *var, ssize_t size)
{
    envobj *env = malloc(sizeof(envobj));
    env->val = var;
    env->size = size;
    gc_register((void *) env, ENVOBJ);
    return env;
}

static inline void *unbox(list *l)
{
    return ((envobj *) l->val)->val;
}

static inline void envobj_free(void *obj)
{
    free((envobj *) obj);
}

#endif
