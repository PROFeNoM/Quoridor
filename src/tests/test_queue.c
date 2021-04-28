#include <stdlib.h>

#include "queue.h"
#include "test.h"

/**
 * Get an allocated copy of an int 'a'
 * @param a Move to copy
 * @return An allocated copy of the int
 */
void* _int_copy(const void* a)
{
	int* p = malloc(sizeof(int));
	*p = *((int*)a);
	return p;
}


/**
 * Release an int memory
 * @param a Int to free
 */
void _free_int(void* a)
{
	free(a);
}

int test__queue__is_empty_with_an_empty_queue()
{
	struct queue* q = queue__empty(_int_copy, _free_int);
	ASSERT_TRUE(queue__is_empty(q));
	queue__free(q);

	return 1;
}

int test__queue__is_empty_after_enqueue()
{
	struct queue* q = queue__empty(_int_copy, _free_int);
	int v1 = 3, v2 = 6;
	queue__enqueue(q, &v1);
	queue__enqueue(q, &v2);
	ASSERT_FALSE(queue__is_empty(q));
	queue__free(q);

	return 1;
}

int test__queue__is_empty_after_enqueue_dequeue()
{
	struct queue* q = queue__empty(_int_copy, _free_int);
	int v1 = 3, v2 = 6;
	queue__enqueue(q, &v1);
	queue__enqueue(q, &v2);
	queue__dequeue(q);
	queue__dequeue(q);
	ASSERT_TRUE(queue__is_empty(q));
	queue__free(q);

	return 1;
}

int test__queue__dequeue_with_an_empty_queue()
{
	struct queue* q = queue__empty(_int_copy, _free_int);
	ASSERT_NULL(queue__dequeue(q));
	queue__free(q);

	return 1;
}

int test__queue__dequeue_after_enqueue()
{
	struct queue* q = queue__empty(_int_copy, _free_int);
	int v1 = 3, v2 = 6;
	queue__enqueue(q, &v1);
	queue__enqueue(q, &v2);
	ASSERT_EQUAL(v1, *((int*)queue__dequeue(q)));
	ASSERT_EQUAL(v2, *((int*)queue__dequeue(q)));
	ASSERT_NULL(queue__dequeue(q));
	queue__free(q);

	return 1;

}

void tests__queue_functions()
{
	TEST_FUNCTION(test__queue__is_empty_with_an_empty_queue);
	TEST_FUNCTION(test__queue__is_empty_after_enqueue);
	TEST_FUNCTION(test__queue__is_empty_after_enqueue_dequeue);
	TEST_FUNCTION(test__queue__dequeue_with_an_empty_queue);
	TEST_FUNCTION(test__queue__dequeue_after_enqueue);
}