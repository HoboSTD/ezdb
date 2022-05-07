#ifndef RECORD_H
#define RECORD_H

#include <stddef.h>

typedef struct record* Record;

Record
record_create(void* record, size_t size);

void
record_free(Record* record);

/*
 * Returns non-zero if the record is valid, zero otherwise.
 */
int
record_is_valid(Record record);

/*
 * Returns non-zero if the records are equal, zero otherwise.
 * NULL records aren't equal to other NULL records.
 */
int
are_records_equal(Record rec1, Record rec2);

#endif
