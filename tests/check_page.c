#include <stdlib.h>
#include <check.h>
#include "page.h"

START_TEST (should_create_valid_page)
{
    Page page = page_create(1024);
    
    ck_assert(page != NULL);
    
    page_free(&page);
}
END_TEST

START_TEST (should_set_page_to_null_after_free)
{
    Page page = page_create(1024);
    
    page_free(&page);
    
    ck_assert(page == NULL);
}
END_TEST

START_TEST (should_not_throw_error_when_freeing_null_page)
{
    page_free(NULL);

    ck_assert(1);
}
END_TEST

START_TEST (should_not_create_page_smaller_than_header)
{
    Page page = page_create(1);
    ck_assert(page == NULL);
}
END_TEST

Suite* page_suite(void)
{
    Suite* s;
    TCase* tc_core;
    
    s = suite_create("Page");
    
    tc_core = tcase_create("Core");
    
    tcase_add_test(tc_core, should_create_valid_page);
    tcase_add_test(tc_core, should_set_page_to_null_after_free);
    tcase_add_test(tc_core, should_not_throw_error_when_freeing_null_page);
    tcase_add_test(tc_core, should_not_create_page_smaller_than_header);
    
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
