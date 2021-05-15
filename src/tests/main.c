#include "test.h"
#include "test_files.h"

size_t tests_run = 0;
size_t tests_passed = 0;

void launch_test()
{
    TEST_FILE(tests__utils_functions);
    TEST_FILE(tests__graph_pattern_functions);
    TEST_FILE(tests__list_functions);
    TEST_FILE(tests__queue_functions);
    TEST_FILE(tests__alphaBetaPlayer_functions);
    TEST_FILE(tests__forrestGump_functions);
    TEST_FILE(tests__random_functions);
    TEST_FILE(tests__IntelligentAntiBolt_functions);
    TEST_FILE(tests__server_functions);
    TEST_FILE(tests__dijsktra_functions);
    TEST_FILE(tests__correlation_graph_functions);
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    
    printf("Lancement des test\n");
    launch_test();

    printf("Tests run: %zu\n", tests_run);
    printf("Tests passed: %zu\n", tests_passed);
    if (tests_run == tests_passed)
        printf("\033[1;92mPASSED\033[0m\n");
    else
        printf("\033[1;91mFAILED\033[0m\n");
    return tests_run - tests_passed;
}
