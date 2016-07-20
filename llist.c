#include "llist.h"
#include <stdlib.h>

struct llist 
{
        struct lnode* head;
        /* tail pointer can only be used on a list that contains elements.
           If the list becomes empty after a list_pop operation, the tail
           becomes dangling. This is ok as it will be initialized to a
           valid value after the next pushX operation. */
        struct lnode* tail;
};

static struct lnode* alloc_node(void*);
static void free_node(struct lnode*);

struct llist* llist_create(void)
{
        struct llist* new = malloc(sizeof(struct llist));
        
        new->head = NULL;
        new->tail = NULL;

        return new;
}

int llist_pushb(struct llist* l, void* e)
{
        struct lnode* new = alloc_node(e);

        if (new == NULL)
        {
                return -1;
        }

        if (l->head)
        {
                /* Non empty list */
                l->tail->next = new;
                l->tail = new;
        }
        else 
        {
                /* Empty list */
                l->head = new;
                l->tail = new;
        }

        return 0;
}

int llist_pushf(struct llist* l, void* e)
{
        struct lnode* new = alloc_node(e);

        if (new == NULL)
        {
                return -1;
        }

        if (l->head)
        {
                /* Non empty list */
                new->next = l->head;
                l->head = new;
        }
        else 
        {
                /* Empty list */
                l->head = new;
                l->tail = new;
        }

        return 0;
}

void* llist_pop(struct llist* l)
{
        struct lnode* n = l->head;
        void* ret;
        
        if (n == NULL)
        {
                return NULL;
        }
        
        ret = n->data;
        l->head = n->next;

        free_node(n);

        return ret;
}

struct lnode* llist_head(struct llist* l)
{
        return l->head;
}

void llist_clear(struct llist* l)
{
        struct lnode* next = l->head;
        
        while (next)
        {
                struct lnode* tmp = next->next;
                
                free_node(next);
                next = tmp;
        }        

        l->head = NULL;
}

size_t llist_size(const struct llist* l)
{
        size_t len = 0;
        struct lnode* n = l->head;
        
        while (n)
        {
                len++;
                n = n->next;
        }

        return len;
}

void llist_destroy(struct llist* l)
{
        llist_clear(l);
        free(l);
}

static struct lnode* alloc_node(void* e)
{
        struct lnode* new = malloc(sizeof(struct lnode));

        new->data = e;
        new->next = NULL;

        return new;
}

static void free_node(struct lnode* n)
{
        free(n);
}
