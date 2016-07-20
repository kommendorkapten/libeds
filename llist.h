#ifndef __LIST_H__
#define __LIST_H__

#include <stddef.h>

struct llist;
struct lnode;
struct lnode 
{
        void* data;
        struct lnode* next;
};

/**
 * Create a new empty list
 * @param void.
 * @return the new list.
 */
struct llist* llist_create(void);

/**
 * Push an item to the back of the list.
 * @param the list.
 * @param the item to add to the list.
 * @return 0 if the element was added to the list.
 */
int llist_pushb(struct llist*, void*);

/**
 * Push an item to the front of the list.
 * @param the list.
 * @param the item to add to the list.
 * @return 0 if the element was added to the list.
 */
int llist_pushf(struct llist*, void*);

/**
 * Remove the first element of the list and return the data stored in it.
 * @param the list.
 * @return the data stored at the list's head.
 */
void* llist_pop(struct llist*);

/**
 * Return the list's head.
 * @param the list.
 * @return the list's head node.
 */
struct lnode* llist_head(struct llist*);

/**
 * Removes all elements from the list.
 * @param the list.
 * @return void.
 */
void llist_clear(struct llist*);

/**
 * Return the number of elements stored in the list.
 * @param the list.
 * @return the number of elements in the list.
 */
size_t llist_size(const struct llist*);

/**
 * Destroy the list and free all it's memory.
 * @param the list to destroy.
 * @return void.
 */
void llist_destroy(struct llist*);

#endif /* __LIST_H__ */
