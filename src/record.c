#include <stdlib.h>
#include <string.h>
#include "record.h"

struct record
{
    size_t  size;
    void*   record;
};

Record
record_create(void* record, size_t size)
{
    if (record == NULL) {
        return NULL;
    }

    Record rec = malloc(sizeof(*rec));
    if (rec == NULL) {
        return NULL;
    }
    
    rec->size = size;
    rec->record = malloc(rec->size);
    if (rec->record == NULL) {
        free(rec);
        return NULL;
    }
    memcpy(rec->record, record, size);

    return rec;
}

void
record_free(Record* record)
{
    if (record == NULL || *record == NULL) {
        return ;
    }

    free((*record)->record);
    free(*record);
    *record = NULL;
}

/*
 * Returns non-zero if the record is valid, zero otherwise.
 */
int
record_is_valid(Record record)
{
    return record != NULL && record->record != NULL && record->size != 0;
}
