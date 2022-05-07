#include <stdlib.h>
#include <string.h>
#include "page.h"

/*
 * For a packed page, size is assumed to be constant.
 * When a record is deleted, all records "above" it in the page are moved down. 
 */

struct page
{
    size_t  size;
    size_t  record_size;
    int     n_records;
    char    data[1];
};

static int header_size();
static int has_space(Page page);
static void* get_offset(Page page, int record_id);
static int find_record(Page page, void* record);

Page
page_create(size_t size, size_t record_size)
{
    if (size <= header_size() || size < MIN_PAGE_SIZE) {
        return NULL;
    }

    Page page = malloc(size);
    if (page == NULL) {
        return NULL;
    }
    
    page->size = size;
    page->record_size = record_size;
    page->n_records = 0;
    
    if (!has_space(page)) {
        page_free(&page);
        return NULL;
    }

    return page;
}

void
page_free(Page* page)
{
    if (page == NULL || *page == NULL) {
        return;
    }
    
    free(*page);
    *page = NULL;
}

int
page_add_record(Page page, void* record)
{
    if (page == NULL || record == NULL) {
        return PAGE_ARG_INVALID;
    }
    
    if (!has_space(page)) {
        return PAGE_HAS_NO_SPACE;
    }

    memcpy(get_offset(page, page->n_records), record, page->record_size);

    return page->n_records++;
}

int
page_delete_record(Page page, void* record)
{
    if (page == NULL || record == NULL) {
        return PAGE_ARG_INVALID;
    }

    int record_id = find_record(page, record);
    if (record_id == PAGE_RECORD_NOT_FOUND) {
        return PAGE_RECORD_NOT_FOUND;
    }
    
    /*
     * Instead of moving all the records above down one, just swap the last record with the deleted
     * record.
     * The last record is then zero'd out to delete the record.
     */
    memmove(get_offset(page, record_id), get_offset(page, page->n_records - 1), page->record_size);
    memset(get_offset(page, page->n_records - 1), 0, page->record_size);
    page->n_records--;
    
    return record_id;
}

int
page_update_record(Page page, void* old, void* new)
{
    if (page == NULL || old == NULL || new == NULL) {
        return PAGE_ARG_INVALID;
    }

    int record_id = find_record(page, old);
    if (record_id == PAGE_RECORD_NOT_FOUND) {
        return PAGE_RECORD_NOT_FOUND;
    }
    
    memcpy(get_offset(page, record_id), new, page->record_size);

    return 0;
}

void*
page_read_record(Page page, int record_id)
{
    if (page == NULL || !(0 <= record_id && record_id < page->n_records)) {
        return NULL;
    }
    
    void* record = malloc(page->record_size);
    if (record == NULL) {
        return NULL;
    }
    
    memcpy(record, get_offset(page, record_id), page->record_size);

    return record;
}


/*
 * PRIVATE FUNCTIONS
 */

static int
header_size()
{
    return sizeof(struct page);
}

static int
has_space(Page page)
{
    return header_size() + (page->n_records + 1) * page->record_size < page->size;
}

static void*
get_offset(Page page, int record_id)
{
    return page->data + record_id * page->record_size;
}

static int
find_record(Page page, void* record)
{    
    for (int record_id = 0; record_id < page->n_records; record_id++) {
        if (memcmp(get_offset(page, record_id), record, page->record_size) == 0) {
            return record_id;
        }
    }
    
    return PAGE_RECORD_NOT_FOUND;
}
