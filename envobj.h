#ifndef ENVOBJ_H
#define ENVOBJ_H
#include <unistd.h>
#include "list.h"
#include "gc.h"

typedef struct envobj envobj;

struct envobj {
    void *val;
    ssize_t size;
};

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

static inline void envobj_free(void *_obj)
{
    envobj *obj = _obj;
    free(obj);
}

#endif
