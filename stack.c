#include "stack.h"
#include <stdlib.h>

struct stack
{
        void** stack;
        size_t cap;
        size_t pos;
        unsigned int flags;
};

struct stack* stack_create(size_t cap, unsigned int flags)
{
        struct stack* s = malloc(sizeof(struct stack));

        s->stack = malloc(cap * sizeof(void*));
        s->cap = cap;
        s->pos = 0;
        s->flags = flags;

        return s;
}

int stack_push(struct stack* s, void* d)
{
        if (s->pos == s->cap)
        {
                /* Stack is full */
                if (s->flags & STACK_AUTO_EXPAND)
                {
                        void** new = realloc(s->stack, 
                                             s->cap * 2 * sizeof(void*));

                        if (new == NULL)
                        {
                                return 1;
                        }
                        s->stack = new;
                        s->cap = 2 * s->cap;
                }
                else 
                {
                        return 1;
                }
        }

        s->stack[s->pos++] = d;

        return 0;
}

void* stack_pop(struct stack* s)
{
        void* ret;

        if (s->pos)
        {
                ret = s->stack[--s->pos];
        }
        else 
        {
                ret = NULL;
        }

        return ret;
}

size_t stack_size(const struct stack* s)
{
        return s->pos;
}

void stack_clear(struct stack* s)
{
        s->pos = 0;
}

void stack_destroy(struct stack* s)
{
        free(s->stack);
        free(s);
}
