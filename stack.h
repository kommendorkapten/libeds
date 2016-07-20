#ifndef __STACK_H__
#define __STACK_H__

#include <stddef.h>

struct stack;

#define STACK_AUTO_EXPAND 0x1

/**
 * Create a stack.
 * @param the initial capacity.
 * @param flags
 * @return the stack
 */
struct stack* stack_create(size_t, unsigned int);

/**
 * Push an element on the stack.
 * @param the stack.
 * @param the element to push.
 * @return 0 if element was added to stack, -1 otherwise.
 */
int stack_push(struct stack*, void*);

/**
 * Pop an element from the stack.
 * @param the stack.
 * @return the element, or NULL if the stack is empty.
 */
void* stack_pop(struct stack*);

/**
 * Returns the number of elements on the stack.
 * @param the stack
 * @return number of elements.
 */
size_t stack_size(const struct stack*);

/**
 * Clears the element on the stack
 * @param the stack
 * @return void.
 */
void stack_clear(struct stack*);

/**
 * Destroy the stack and free any memory.
 * @param the stack to destroy.
 * @return void.
 */
void stack_destroy(struct stack*);
#endif /* __STACK_H__ */
