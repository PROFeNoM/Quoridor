#ifndef _QUEUE_H
#define _QUEUE_H


typedef void* (*Copy)(const void*);
typedef void (*Release)(void*);

struct queue;

/**
 * Get an empty queue
 * @param copy Copy function
 * @param free_element Release function
 * @return A struct queue representing the empty queue
 */
struct queue* queue__empty(Copy copy, Release free_element);

/**
 * Tell if a queue is empty
 * @param q Queue to check
 * @return Non-zero if queue is empty, 0 otherwise
 */
int queue__is_empty(const struct queue* q);

/**
 * Add the element c to the end of the queue q
 * @param q Queue
 * @param c Element to add
 */
void queue__enqueue(struct queue* q, void* c);

/**
 * Dequeue the queue q
 * @param q Queue to dequeue
 * @return The element at the top of the queue
 */
void* queue__dequeue(struct queue* q);

/**
 * Frees the memory allocated for struct queue q
 * @param q Queue to free
 */
void queue__free(struct queue* q);


#endif //_QUEUE_H
