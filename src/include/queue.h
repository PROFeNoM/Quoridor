#ifndef _QUEUE_H
#define _QUEUE_H


typedef void* (*Copy)(const void*);
typedef void (*Release)(void*);

struct queue;

// Return a struct queue representing the empty queue
struct queue* queue__empty(Copy copy, Release free_element);

// Returns non-zero if queue is empty, 0 otherwise
int queue__is_empty(const struct queue* q);

// Adds c to the end of queue
void queue__enqueue(struct queue* q, void* c);

// Returns the element at the top of the queue
void* queue__dequeue(struct queue* q);

// Frees the memory allocated for struct queue q
void queue__free(struct queue* q);


#endif //_QUEUE_H
