#include "../include/queue.h"

#include <stdlib.h>

struct node {
	void* data;
	struct node* next;
};

struct queue {
	struct node* head;
	struct node* tail;
	Copy cp;
	Release del;
};


struct node* node__create(void* data)
{
	struct node* n = malloc(sizeof(struct node));
	n->data = data;
	n->next = NULL;
	return n;
}


// Return a struct queue representing the empty queue
struct queue* queue__empty(Copy copy, Release free_element)
{
	struct queue* q = malloc(sizeof(struct queue));
	q->head = NULL;
	q->tail = NULL;
	q->cp = copy;
	q->del = free_element;
	return q;
}


// Returns non-zero if queue is empty, 0 otherwise
int queue__is_empty(const struct queue* q)
{
	return q->head == NULL;
}

void queue__enqueue(struct queue* q, void* c)
{
	void* data_copy = q->cp(c);
	struct node* n = node__create(data_copy);

	if (queue__is_empty(q))
	{
		q->head = n;
		q->tail = n;
	}
	else
	{
		q->tail->next = n;
		q->tail = q->tail->next;
	}

}


void* queue__dequeue(struct queue* q)
{
	if (queue__is_empty(q))
		return NULL;

	void* data = q->head->data;
	struct node* n_head = q->head;
	q->head = q->head->next;
	free(n_head);

	return data;
}


void queue__free(struct queue* q)
{
	while (!queue__is_empty(q))
	{
		void* data = queue__dequeue(q);
		q->del(data);
	}
	free(q);
}