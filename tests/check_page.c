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
    Page page = page_create(100);
    ck_assert(page == NULL);
}
END_TEST

START_TEST (should_be_able_to_add_record)
{
    Page page = page_create(1024);
    
    size_t size = 128;
    char* record = malloc(size);
    
    ck_assert(page_add_record(page, record, size) != 0);
    
    free(record);
    
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_add_null_record)
{
    Page page = page_create(1024);
    
    ck_assert(page_add_record(page, NULL, 128) == 0);
    
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_add_records_larger_than_page)
{
    Page page = page_create(1024);
    
    size_t size = 2048;
    char* record = malloc(size);
    
    ck_assert(page_add_record(page, record, size) == 0);
    
    free(record);
    
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_add_record_if_no_space)
{
    Page page = page_create(1024);
    
    size_t size = 512;
    char* record = malloc(size);

    ck_assert(page_add_record(page, record, size) != 0);
    ck_assert(page_add_record(page, record, size) == 0);
    
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_add_record_to_null_page)
{
    size_t size = 512;
    char* record = malloc(size);
    
    ck_assert(page_add_record(NULL, record, size) == 0);
    
    free(record);
}
END_TEST

START_TEST (should_be_able_to_delete_record)
{
    Page page = page_create(1024);
    
    char* record = strdup("hello,my,name,jeff");
    size_t size = strlen(record);
    
    page_add_record(page, record, size);
    ck_assert(page_delete_record(page, record, size) == 0);
    
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_delete_record_from_empty_page)
{
    Page page = page_create(1024);
    
    char* record = strdup("hello,my,name,jeff");
    size_t size = strlen(record);
    
    ck_assert(page_delete_record(page, record, size) == -1);
    
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_delete_from_null_page)
{
    char* record = strdup("hello,my,name,jeff");
    size_t size = strlen(record);
    
    ck_assert(page_delete_record(NULL, record, size) == -1);
    
    free(record);
}
END_TEST

START_TEST (should_not_be_able_to_delete_using_null_record)
{
    Page page = page_create(1024);
    
    char* record = strdup("hello,my,name,jeff");
    size_t size = strlen(record);
    
    page_add_record(page, record, size);
    ck_assert(page_delete_record(page, NULL, size) == -1);
    
    free(record);
    page_free(&page);
}
END_TEST

Suite* page_suite(void)
{
    Suite* s = suite_create("Page");
    
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, should_create_valid_page);
    tcase_add_test(tc_core, should_set_page_to_null_after_free);
    tcase_add_test(tc_core, should_not_throw_error_when_freeing_null_page);
    tcase_add_test(tc_core, should_not_create_page_smaller_than_header);
    suite_add_tcase(s, tc_core);
    
    TCase* tc_add = tcase_create("Add");
    tcase_add_test(tc_add, should_be_able_to_add_record);
    tcase_add_test(tc_add, should_not_be_able_to_add_null_record);
    tcase_add_test(tc_add, should_not_be_able_to_add_records_larger_than_page);
    tcase_add_test(tc_add, should_not_be_able_to_add_record_if_no_space);
    tcase_add_test(tc_add, should_not_be_able_to_add_record_to_null_page);
    suite_add_tcase(s, tc_add);
    
    TCase* tc_remove = tcase_create("Remove");
    tcase_add_test(tc_remove, should_be_able_to_delete_record);
    tcase_add_test(tc_remove, should_not_be_able_to_delete_record_from_empty_page);
    tcase_add_test(tc_remove, should_not_be_able_to_delete_from_null_page);
    tcase_add_test(tc_remove, should_not_be_able_to_delete_using_null_record);
    suite_add_tcase(s, tc_remove);
    
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
