#include "list.h"
#include "test.h"

/**
 * Get an allocated copy of an int 'a'
 * @param a Move to copy
 * @return An allocated copy of the int
 */
void* int_copy(const void* a)
{
	int* p = malloc(sizeof(int));
	*p = *((int*)a);
	return p;
}


/**
 * Release an int memory
 * @param a Int to free
 */
void free_int(void* a)
{
	free(a);
}

int test__list__is_empty_with_an_empty_list()
{
	struct list* lst = list__empty(int_copy, free_int);
	ASSERT_TRUE(list__is_empty(lst));
	list__free(lst);

	return 1;
}

int test__list__is_empty_after_adding()
{
	struct list* lst = list__empty(int_copy, free_int);
	ASSERT_TRUE(list__is_empty(lst));
	int v = 3;
	list__add(lst, &v);
	ASSERT_FALSE(list__is_empty(lst));
	list__free(lst);

	return 1;
}

int test__list__size_with_an_empty_list()
{
	struct list* lst = list__empty(int_copy, free_int);
	ASSERT_EQUAL(0, list__size(lst));
	list__free(lst);

	return 1;
}

int test__list__size_with_after_adding()
{
	struct list* lst = list__empty(int_copy, free_int);
	ASSERT_EQUAL(0, list__size(lst));
	int v = 3;
	list__add(lst, &v);
	ASSERT_EQUAL(1, list__size(lst));
	list__free(lst);

	return 1;
}

int test__list__get_after_adding()
{
	struct list* lst = list__empty(int_copy, free_int);
	int v = 3;
	list__add(lst, &v);
	ASSERT_EQUAL(v, *((int*)list__get(lst, 0)));
	list__free(lst);

	return 1;
}

int test__list__change_after_adding()
{
	struct list* lst = list__empty(int_copy, free_int);
	int v1 = 3;
	list__add(lst, &v1);
	int v2 = 1;
	list__change(lst, 0, &v2);
	ASSERT_EQUAL(v2, *((int*)list__get(lst, 0)));
	list__free(lst);

	return 1;

}

void tests__list_functions()
{
	TEST_FUNCTION(test__list__is_empty_with_an_empty_list);
	TEST_FUNCTION(test__list__is_empty_after_adding);
	TEST_FUNCTION(test__list__size_with_an_empty_list);
	TEST_FUNCTION(test__list__size_with_after_adding);
	TEST_FUNCTION(test__list__get_after_adding);
	TEST_FUNCTION(test__list__change_after_adding);
}