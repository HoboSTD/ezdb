#include <stdlib.h>
#include <check.h>
#include "table.h"

START_TEST (should_create_table)
{
    Table table = table_create("test");

    ck_assert(table != NULL);
    
    table_free(table);
}
END_TEST

START_TEST (should_not_be_able_to_create_table_with_null_name)
{
    Table table = table_create(NULL);
    
    ck_assert(table == NULL);
}
END_TEST

START_TEST (should_not_be_able_to_create_table_with_empty_name)
{
    Table table = table_create("\0");
    
    ck_assert(table == NULL);
}
END_TEST

/* This is a duplicate, just here in case it is expanded on. */
START_TEST (should_be_able_to_free_table)
{
    Table table = table_create("test");
    
    ck_assert(table != NULL);
    
    table_free(table);
}
END_TEST

Suite* page_suite(void)
{
    Suite* s = suite_create("Table");
    
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, should_create_table);
    tcase_add_test(tc_core, should_not_be_able_to_create_table_with_null_name);
    tcase_add_test(tc_core, should_not_be_able_to_create_table_with_empty_name);
    tcase_add_test(tc_core, should_be_able_to_free_table);
    suite_add_tcase(s, tc_core);
    
    return s;
}

int
main(void)
{
    int number_failed;
    Suite* s;
    SRunner* sr;
    
    s = page_suite();
    sr = srunner_create(s);
    
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
