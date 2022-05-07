#include <stdlib.h>
#include <string.h>
#include <check.h>
#include "page.h"

START_TEST (should_create_valid_page)
{
    Page page = page_create(1024, 128);
    
    ck_assert(page != NULL);
    
    page_free(&page);
}
END_TEST

START_TEST (should_set_page_to_null_after_free)
{
    Page page = page_create(1024, 128);
    
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
    Page page = page_create(50, 4);
    ck_assert(page == NULL);
}
END_TEST

START_TEST (should_not_be_able_to_create_pages_that_cant_hold_records)
{
    Page page = page_create(1024, 2048);
    
    ck_assert(page == NULL);
}
END_TEST

START_TEST (should_be_able_to_add_record)
{
    Page page = page_create(1024, 128);
    
    char* record = malloc(128);
    
    ck_assert(page_add_record(page, record) == 0);
    
    free(record);
    
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_add_null_record)
{
    Page page = page_create(1024, 128);
    
    ck_assert(page_add_record(page, NULL) == PAGE_ARG_INVALID);
    
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_add_record_if_no_space)
{
    Page page = page_create(1024, 512);
    char* record = malloc(512);

    ck_assert(page_add_record(page, record) == 0);
    ck_assert(page_add_record(page, record) == PAGE_HAS_NO_SPACE);
    
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_add_record_to_null_page)
{
    size_t size = 512;
    char* record = malloc(size);
    
    ck_assert(page_add_record(NULL, record) == PAGE_ARG_INVALID);
    
    free(record);
}
END_TEST

START_TEST (should_be_able_to_add_many_records)
{
    Page page = page_create(1024, 18);
    
    char* record1 = strdup("hello,my,name,jeff");
    char* record2 = strdup("hello,my,name,john");
    
    ck_assert(page_add_record(page, record1) == 0);
    ck_assert(page_add_record(page, record2) == 1);
    
    free(record1);
    free(record2);
    page_free(&page);
}
END_TEST

START_TEST (should_be_able_to_delete_record)
{
    Page page = page_create(1024, 18);
    
    char* record = strdup("hello,my,name,jeff");
    
    page_add_record(page, record);
    ck_assert(page_delete_record(page, record) == 0);
    
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_delete_record_from_empty_page)
{
    Page page = page_create(1024, 18);
    
    char* record = strdup("hello,my,name,jeff");
    
    ck_assert(page_delete_record(page, record) == PAGE_RECORD_NOT_FOUND);
    
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_delete_from_null_page)
{
    char* record = strdup("hello,my,name,jeff");
    
    ck_assert(page_delete_record(NULL, record) == PAGE_ARG_INVALID);
    
    free(record);
}
END_TEST

START_TEST (should_not_be_able_to_delete_using_null_record)
{
    Page page = page_create(1024, 18);
    
    char* record = strdup("hello,my,name,jeff");
    
    page_add_record(page, record);
    ck_assert(page_delete_record(page, NULL) == PAGE_ARG_INVALID);
    
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_delete_non_existant_record)
{
    Page page = page_create(1024, 18);
    
    char* record1 = strdup("hello,my,name,jeff");
    char* record2 = strdup("hello,my,name,john");
    
    page_add_record(page, record1);
    ck_assert(page_delete_record(page, record2) == PAGE_RECORD_NOT_FOUND);
    
    free(record1);
    free(record2);
    page_free(&page);
}
END_TEST

START_TEST (should_be_able_to_delete_many_records)
{
    Page page = page_create(1024, 18);
    
    char* record1 = strdup("hello,my,name,jeff");
    char* record2 = strdup("hello,my,name,john");
    
    page_add_record(page, record1);
    page_add_record(page, record2);
    ck_assert_int_eq(page_delete_record(page, record1), 0);
    ck_assert_int_eq(page_delete_record(page, record2), 0);
    
    free(record1);
    free(record2);
    page_free(&page);
}
END_TEST

START_TEST (should_be_able_to_update_record)
{
    Page page = page_create(1024, 18);
    
    char* old = strdup("hello,my,name,jeff");
    char* new = strdup("hello,my,name,john");
    size_t size2 = strlen(new);
    
    int record_id = page_add_record(page, old);
    ck_assert(page_update_record(page, old, new) == 0);
    char* record = page_read_record(page, record_id);
    ck_assert(memcmp(new, record, size2) == 0);
    
    free(record);
    free(old);
    free(new);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_update_null_page)
{
    char* old = strdup("hello,my,name,jeff");
    char* new = strdup("hello,my,name,john");

    ck_assert(page_update_record(NULL, old, new) == PAGE_ARG_INVALID);
    
    free(old);
    free(new);
}
END_TEST

START_TEST (should_not_be_able_to_update_null_record)
{
    Page page = page_create(1024, 18);
    
    char* record = strdup("hello,my,name,jeff");
    
    page_add_record(page, record);
    ck_assert(page_update_record(page, NULL, record) == PAGE_ARG_INVALID);
    ck_assert(page_update_record(page, record, NULL) == PAGE_ARG_INVALID);
    
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_update_record_that_doesnt_exist)
{
    Page page = page_create(1024, 18);
    
    char* record = strdup("hello,my,name,jeff");
    char* unknown = strdup("hello,my,name,john");
    
    page_add_record(page, record);
    ck_assert(page_update_record(page, unknown, record) == PAGE_RECORD_NOT_FOUND);
    
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_only_update_one_record)
{
    Page page = page_create(1024, 18);
    
    char* record = strdup("hello,my,name,jeff");
    char* new = strdup("hello,my,name,john");
    size_t size = strlen(record);
    
    page_add_record(page, record);
    int record_id = page_add_record(page, record);
    ck_assert(page_update_record(page, record, new) == 0);
    
    char* read = page_read_record(page, record_id);
    ck_assert(memcmp(record, read, size) == 0);
    
    free(read);
    free(new);
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_read_record_that_doesnt_exist)
{
    Page page = page_create(1024, 18);
    
    char* record = strdup("hello,my,name,jeff");
    
    page_add_record(page, record);
    ck_assert(page_read_record(page, -1) == NULL);
    ck_assert(page_read_record(page, 1) == NULL);
    
    free(record);
    
    page_free(&page);
}
END_TEST

START_TEST (should_be_able_to_read_record_that_exists)
{
    Page page = page_create(1024, 18);
    
    char* record = strdup("hello,my,name,jeff");
    size_t size = strlen(record);
    
    int record_id = page_add_record(page, record);
    
    char* read = page_read_record(page, record_id);
    ck_assert(read != NULL);
    
    ck_assert(memcmp(record, read, size) == 0);
    
    free(read);
    free(record);
    page_free(&page);
}
END_TEST

START_TEST (should_not_be_able_to_read_from_null_page)
{
    ck_assert(page_read_record(NULL, 0) == NULL);
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
    tcase_add_test(tc_core, should_not_be_able_to_create_pages_that_cant_hold_records);
    suite_add_tcase(s, tc_core);
    
    TCase* tc_add = tcase_create("Add");
    tcase_add_test(tc_add, should_be_able_to_add_record);
    tcase_add_test(tc_add, should_not_be_able_to_add_null_record);
    tcase_add_test(tc_add, should_not_be_able_to_add_record_if_no_space);
    tcase_add_test(tc_add, should_not_be_able_to_add_record_to_null_page);
    tcase_add_test(tc_add, should_be_able_to_add_many_records);
    suite_add_tcase(s, tc_add);
    
    TCase* tc_remove = tcase_create("Remove");
    tcase_add_test(tc_remove, should_be_able_to_delete_record);
    tcase_add_test(tc_remove, should_not_be_able_to_delete_record_from_empty_page);
    tcase_add_test(tc_remove, should_not_be_able_to_delete_from_null_page);
    tcase_add_test(tc_remove, should_not_be_able_to_delete_using_null_record);
    tcase_add_test(tc_remove, should_not_be_able_to_delete_non_existant_record);
    tcase_add_test(tc_remove, should_be_able_to_delete_many_records);
    suite_add_tcase(s, tc_remove);
    
    TCase* tc_update = tcase_create("Update");
    tcase_add_test(tc_update, should_be_able_to_update_record);
    tcase_add_test(tc_update, should_not_be_able_to_update_null_page);
    tcase_add_test(tc_update, should_not_be_able_to_update_null_record);
    tcase_add_test(tc_update, should_not_be_able_to_update_record_that_doesnt_exist);
    tcase_add_test(tc_update, should_only_update_one_record);
    suite_add_tcase(s, tc_update);
    
    TCase* tc_read = tcase_create("Read");
    tcase_add_test(tc_read, should_not_be_able_to_read_record_that_doesnt_exist);
    tcase_add_test(tc_read, should_be_able_to_read_record_that_exists);
    tcase_add_test(tc_read, should_not_be_able_to_read_from_null_page);
    suite_add_tcase(s, tc_read);
    
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
