#ifndef _TEST_H_
#define _TEST_H_

#include <stdio.h>

extern size_t tests_run;
extern size_t tests_passed;


#define ASSERT_FAIL_MESSAGE(message) \
    printf("\t\033[1;31mX\033[0;91m %s() failure line %d, in file %s\033[0m\n", __func__, __LINE__, __FILE__);\
    if ((message)) {\
        printf("\t\t%s\n", (message));\
    }

#define ASSERT_FAIL() \
    ASSERT_FAIL_MESSAGE(NULL)


#define TEST_SUCCESS(function_tested) \
    printf("\t\033[1;32m√\033[0;92m %s()\033[0m\n", #function_tested)


#define TEST_FILE_MESSAGE(testing_file, message)                                      \
    if ((message))\
        printf("\n%s", (char*)(message));\
    printf("\n\033[1;97mTESTING : %s()\n\033[0m", #testing_file);   \
    testing_file();                                                 \
    printf("\n")

#define TEST_FILE(testing_file)\
    TEST_FILE_MESSAGE(testing_file, NULL)

#define TEST_FUNCTION_MESSAGE(testing_function, message) \
    tests_run++;                                         \
    if ((message))                                       \
        printf("\t%s :\n\t", (char*)(message));                       \
    if (testing_function())                              \
    {                                                    \
        tests_passed++;                                  \
        TEST_SUCCESS(testing_function);                  \
    }

#define TEST_FUNCTION(testing_function)\
    TEST_FUNCTION_MESSAGE(testing_function, NULL)

#define ASSERT_TRUE(condition) \
    if (!(condition)) {    \
        ASSERT_FAIL_MESSAGE("Expression : "#condition" evaluated to FALSE");       \
        return 0;     \
    }

#define ASSERT_FALSE(condition)                                 \
    if ((condition))                                          \
    {                                                          \
        ASSERT_FAIL_MESSAGE("Expression : "#condition" evaluated to TRUE"); \
        return 0;                                              \
    }

#define ASSERT_NULL(pointer)                  \
    if (!((pointer) == NULL))                           \
    {                                         \
        ASSERT_FAIL_MESSAGE("Expected NULL"); \
        return 0;                             \
    }

#define ASSERT_NOT_NULL(pointer)                  \
    if (!((pointer) != NULL))                           \
    {                                         \
        ASSERT_FAIL_MESSAGE("Expected non-NULL"); \
        return 0;                             \
    }

#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) {\
        ASSERT_FAIL_MESSAGE("expected : "#expected", received : "#actual);\
        return 0;\
    }

#define ASSERT_NOT_EQUAL(threshold, actual)                                        \
    if ((threshold) == (actual)) {                                                   \
        ASSERT_FAIL_MESSAGE("threshold : " #threshold ", received : " #actual); \
        return 0;                                                             \
    }

#define ASSERT_GREATER_THAN(threshold, actual)                  \
    if (!((actual) > (threshold)))                                \
    {                                             \
        ASSERT_FAIL_MESSAGE("Expected greater than "#threshold", received : "#actual); \
        return 0;                                 \
    }

#define ASSERT_LESS_THAN(threshold, actual)                                              \
    if (!((actual) < (threshold)))                                                          \
    {                                                                                    \
        ASSERT_FAIL_MESSAGE("Expected less than " #threshold ", received : " #actual); \
        return 0;                                                                        \
    }

#define ASSERT_GREATER_OR_EQUAL(threshold, actual)                                           \
    if (!((actual) >= (threshold)))                                                          \
    {                                                                                    \
        ASSERT_FAIL_MESSAGE("Expected greater or equal " #threshold ", received : " #actual); \
        return 0;                                                                        \
    }

#define ASSERT_LESS_OR_EQUAL(threshold, actual)                                            \
    if (!((actual) <= (threshold)))                                                           \
    {                                                                                         \
        ASSERT_FAIL_MESSAGE("Expected less or equal " #threshold ", received : " #actual); \
        return 0;                                                                             \
    }

#define ASSERT_ARRAY_EQUAL(expected, actual, size) \
    for (int i = 0; i < size; i++)                 \
    {                                              \
        if (!((actual[i]) == (expected[i])))       \
        {                                          \
            ASSERT_FAIL_MESSAGE("Expected array to be equal"); \
            return 0;                                                                             \
        }\
    }

#define ASSERT_ARRAY_NOT_EQUAL(expected, actual, size)       \
    for (int i = 0; i < size; i++)                       \
    {                                                    \
        if (!((actual[i]) != (expected[i])))             \
        {                                                \
            ASSERT_FAIL_MESSAGE("Expected array to be non-equal"); \
            return 0;                                    \
        }                                                \
    }
#endif