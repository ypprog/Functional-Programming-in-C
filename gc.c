#include "gc.h"
#include <stdio.h>
#include <stdlib.h>
#include "envobj.h"
#include "list.h"

typedef struct ref ref;
struct ref {
    void *ptr;  // pointer to the obj
    TYPE type;  // obj type
    ref *next;  // refs are stored as a list;
};

typedef struct gc gc;
struct gc {
    ref *marked;  // these will not be freed unless unmarked
    ref *last_marked;
    ref *unmarked;       // these will be freed when gc_collect is called
    ref *last_unmarked;  // we want append to be O(1)
    void (*destructor_table[TYPE_COUNT])(void *);
};

// garbage collector instance
static gc _gc;

static void gc_register_destructor(TYPE type, void (*destructor)(void *))
{
    _gc.destructor_table[type] = destructor;
}

static ref *ref_item(void *ptr, TYPE type)
{
    ref *r = malloc(sizeof(ref));
    r->ptr = ptr;
    r->type = type;
    r->next = NULL;
    return r;
}

static void append_unmarked(ref *unmarked)
{
    if (!_gc.unmarked) {
        _gc.unmarked = unmarked;
        _gc.last_unmarked = _gc.unmarked;
    } else {
        _gc.last_unmarked->next = unmarked;
        _gc.last_unmarked = unmarked;
    }
}

static ref *remove_unmarked(void *obj)
{
    ref *curr = _gc.unmarked;
    if (!curr)
        return NULL;
    if (curr->ptr == obj) {
        _gc.unmarked = curr->next;
        curr->next = NULL;
        return curr;
    }
    if (_gc.last_unmarked->ptr == obj) {
        for (; curr->next->ptr != obj; curr = curr->next)
            ;
        ref *o = _gc.last_unmarked;
        o->next = NULL;
        _gc.last_unmarked = curr;
        curr->next = NULL;
        return o;
    }
    for (; curr->next != NULL; curr = curr->next) {
        if (curr->next->ptr == obj) {
            ref *o = curr->next;
            curr->next = o->next;
            o->next = NULL;
            return o;
        }
    }
    return NULL;
}

static ref *remove_marked(void *obj)
{
    ref *curr = _gc.marked;
    if (!curr)
        return NULL;
    if (curr->ptr == obj) {
        _gc.marked = curr->next;
        curr->next = NULL;
        return curr;
    }
    if (_gc.last_marked->ptr == obj) {
        for (; curr->next->ptr != obj; curr = curr->next)
            ;
        ref *o = _gc.last_marked;
        o->next = NULL;
        _gc.last_marked = curr;
        curr->next = NULL;
        return o;
    }
    for (; curr->next != NULL; curr = curr->next) {
        if (curr->next->ptr == obj) {
            ref *o = curr->next;
            curr->next = o->next;
            o->next = NULL;
            return o;
        }
    }
    return NULL;
}

static void append_marked(ref *marked)
{
    if (!_gc.marked) {
        _gc.marked = marked;
        _gc.last_marked = _gc.marked;
    } else {
        _gc.last_marked->next = marked;
        _gc.last_marked = marked;
    }
}

static void standard_free(void *ptr)
{
    free(ptr);
}

// public functions
void gc_init(void)
{
    // register destructors here
    // all of these are basically just standard free
    // just showing that one could register other destructors
    gc_register_destructor(ENVOBJ, envobj_free);
    gc_register_destructor(LIST, list_free);
    gc_register_destructor(STANDARD, standard_free);
    // don't change these
    _gc.marked = NULL;
    _gc.unmarked = NULL;
    _gc.last_unmarked = NULL;
    _gc.last_marked = NULL;
}

void gc_remove(void *obj)
{
    free(remove_unmarked(obj));
    free(remove_marked(obj));
}

void gc_mark(void *obj)
{
    append_marked(remove_unmarked(obj));
}

void gc_unmark(void *obj)
{
    append_unmarked(remove_marked(obj));
}

// don't register objs twice; boy that could go poorly
void gc_register(void *obj, TYPE type)
{
    append_unmarked(ref_item(obj, type));
}

void gc_collect(void)
{
    for (ref *curr = _gc.unmarked; curr;) {
        ref *next = curr->next;
        (*(_gc.destructor_table[curr->type]))(curr->ptr);
        free(curr);
        curr = next;
    }
    _gc.unmarked = NULL;
}
