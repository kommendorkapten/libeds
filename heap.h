#ifndef __HEAP_H__
#define __HEAP_H__

#include <stddef.h>

struct heap;
/**
 * Compare the order two items.
 * When the heap_cmp method is called during execution, a will always be 
 * the item from the tree, and b will be user provided element.
 * @param the first item, a.
 * @param the second item, b.
 * @return  0 if a and b has the same order.
 *          1 if a has higher order than b.
 *         -1 if b has higher order than a.
 */
typedef int (*heap_cmp)(const void* a, const void* b);

/**
 * Create an empty heap. The heap will store the items with the higest
 * order at the root. Multiple itmes with the same order can be sored in
 * the heap.
 * @param the order method to use when ordering the the heap.
 * @return the newly created heap.
 */
struct heap* heap_create(heap_cmp);

/**
 * Clears all elements in the heap.
 * @param the heap to clear.
 * @return void.
 */
void heap_clear(struct heap*);

/**
 * Extract the higest ordered item from the heap.
 * If multiple values with the same order are present, it's not defined 
 * which one will be returned.
 * @param the heap to extract the item from.
 * @return the higest order item, or NULL if heap is empty.
 */
void* heap_min(struct heap*);

/**
 * Insert a new item into the heap.
 * @param the heap.
 * @param the item to insert.
 * @return 0 if the item was inserted, non-zero otherwise.
 */
int heap_insert(struct heap*, void*);

/**
 * Return the size of the heap.
 * @param the heap.
 * @return the number of items in the heap.
 */
size_t heap_size(const struct heap*);

/**
 * Destroy the heap and free any memory it has allocated.
 * @param the heap.
 * @return void.
 */
void heap_destroy(struct heap*);

#endif /* __HEAP_H__ */
