#include <stdlib.h>
#include <check.h>
#include "record.h"

START_TEST (should_create_valid_record)
{
    Record record = record_create("hello", strlen("hello"));
    
    ck_assert(record != NULL);
    
    record_free(&record);
}
END_TEST

START_TEST (should_free_record)
{
    Record record = record_create("hello", strlen("hello"));
    
    record_free(&record);
    
    ck_assert(record == NULL);
}
END_TEST

START_TEST (should_not_free_invalid_record)
{
    record_free(NULL);
    
    ck_assert(1);
}
END_TEST

START_TEST (should_have_null_record_be_invalid)
{
    ck_assert(record_is_valid(NULL) == 0);
}
END_TEST

START_TEST (should_have_null_record_pointer_be_invalid)
{
    Record record = record_create(NULL, 5);
    
    ck_assert(record_is_valid(record) == 0);
    
    record_free(&record);
}
END_TEST

START_TEST (should_have_zero_size_record_be_invalid)
{
    Record record = record_create("hello", 0);
    ck_assert(record_is_valid(record) == 0);
    record_free(&record);
}
END_TEST

Suite* page_suite(void)
{
    Suite* s = suite_create("Record");
    
    TCase* tc_core = tcase_create("Core");
    tcase_add_test(tc_core, should_create_valid_record);
    tcase_add_test(tc_core, should_free_record);
    tcase_add_test(tc_core, should_not_free_invalid_record);
    suite_add_tcase(s, tc_core);
    
    TCase* tc_validity = tcase_create("Validity");
    tcase_add_test(tc_validity, should_have_null_record_be_invalid);
    tcase_add_test(tc_validity, should_have_null_record_pointer_be_invalid);
    tcase_add_test(tc_validity, should_have_zero_size_record_be_invalid);
    suite_add_tcase(s, tc_validity);
    
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