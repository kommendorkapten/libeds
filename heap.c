#include "heap.h"
#include <stdlib.h>

#define LEFT(x) (2 * x + 1)
#define RIGHT(x) (2 * x + 2)
#define PARENT(x) ((x - 1) / 2)

struct heap
{
        void** items;
        heap_cmp cmp;
        size_t size;
        size_t cap;
};

struct heap* heap_create(heap_cmp cmp)
{
        struct heap* h = malloc(sizeof(struct heap));
        size_t cap = 128; /* first guess of size */
        
        h->items = malloc(cap * sizeof(void*));
        h->cmp = cmp;
        h->size = 0L;
        h->cap = cap;

        return h;
}

void heap_clear(struct heap* h)
{
        h->size = 0L;
}

void* heap_min(struct heap* h)
{
        if (h->size == 0)
        {
                return NULL;
        }

        void* ret = h->items[0];
        void* k = h->items[h->size - 1];

        /* Restore partial ordering property */
        size_t m = 0;

        /* Take the least ordered element and let it move down the
           tree */
        while ((LEFT(m) < h->size && 
                h->cmp(h->items[LEFT(m)], k) > 0 ) ||
               (RIGHT(m) < h->size && 
                h->cmp(h->items[RIGHT(m)], k) > 0 ))
        {
                size_t p;

                if (RIGHT(m) < h->size)
                {
                        /* Two children */
                        if (h->cmp(h->items[LEFT(m)], h->items[RIGHT(m)]) > 0)
                        {
                                p = LEFT(m);
                        }
                        else
                        {
                                p = RIGHT(m);
                        }
                }
                else 
                {
                        /* Only one children, last leaf in tree */
                        p = h->size - 1;
                }
                /* Move child up */
                h->items[m] = h->items[p];
                /* Continue down the tree */
                m = p;
        }
        h->items[m] = k;
        h->size--;

        return ret;
}

int heap_insert(struct heap* h, void* e)
{
        size_t m = h->size;
        size_t p;

        /* Time to expand? */
        if (h->size == h->cap)
        {
                size_t new_cap = h->cap * 2;
                void* new = realloc(h->items, new_cap * sizeof(void*));

                if (new == NULL)
                {
                        return -1;
                }

                h->cap = new_cap;
                h->items = new;
        }

        /* Restore partial ordering property */
        p = PARENT(m);
        while (m > 0 && h->cmp(h->items[p], e) < 0)
        {
                /* Move all items down in the tree that are of lesser
                   order than e */
                h->items[m] = h->items[p];
                m = p;
                p = PARENT(m);
        }

        h->items[m] = e;
        h->size++;
        return 0;
}

size_t heap_size(const struct heap* h)
{
        return h->size;
}

void heap_destroy(struct heap* h)
{
        free(h->items);
        free(h);
}

