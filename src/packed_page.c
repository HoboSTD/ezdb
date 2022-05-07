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
    int     n_tuples;
    char    data[1];
};

static int header_size();
static int has_space(Page page, size_t size);
static void* get_offset(Page page, int record_id, size_t size);
static int find_record(Page page, void* record, size_t size);

Page
page_create(size_t size)
{
    if (size <= header_size() || size < MIN_PAGE_SIZE) {
        return NULL;
    }

    Page page = malloc(size);
    if (page == NULL) {
        return NULL;
    }
    
    page->size = size;
    page->n_tuples = 0;

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
page_add_record(Page page, void* record, size_t size)
{
    if (page == NULL || record == NULL) {
        return PAGE_ARG_INVALID;
    }
    
    if (!has_space(page, size)) {
        return PAGE_HAS_NO_SPACE;
    }

    memcpy(get_offset(page, page->n_tuples, size), record, size);

    return page->n_tuples++;
}

int
page_delete_record(Page page, void* record, size_t size)
{
    if (page == NULL || record == NULL) {
        return PAGE_ARG_INVALID;
    }

    int record_id = find_record(page, record, size);
    if (record_id == PAGE_RECORD_NOT_FOUND) {
        return PAGE_RECORD_NOT_FOUND;
    }
    
    /*
     * Instead of moving all the records above down one, just swap the last record with the deleted
     * record.
     * The last record is then zero'd out to delete the record.
     */
    memmove(get_offset(page, record_id, size), get_offset(page, page->n_tuples - 1, size), size);
    memset(get_offset(page, page->n_tuples - 1, size), 0, size);
    page->n_tuples--;
    
    return record_id;
}

int
page_update_record(Page page, void* old, void* new, size_t size)
{
    if (page == NULL || old == NULL || new == NULL) {
        return PAGE_ARG_INVALID;
    }

    int record_id = find_record(page, old, size);
    if (record_id == PAGE_RECORD_NOT_FOUND) {
        return PAGE_RECORD_NOT_FOUND;
    }
    
    memcpy(get_offset(page, record_id, size), new, size);

    return 0;
}

void*
page_read_record(Page page, int record_id, size_t size)
{
    if (page == NULL || !(0 <= record_id && record_id < page->n_tuples)) {
        return NULL;
    }
    
    void* record = malloc(size);
    if (record == NULL) {
        return NULL;
    }
    
    memcpy(record, get_offset(page, record_id, size), size);

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
has_space(Page page, size_t size)
{
    return header_size() + page->n_tuples * size + size < page->size;
}

static void*
get_offset(Page page, int record_id, size_t size)
{
    return page->data + record_id * size;
}

static int
find_record(Page page, void* record, size_t size)
{    
    for (int record_id = 0; record_id < page->n_tuples; record_id++) {
        if (memcmp(get_offset(page, record_id, size), record, size) == 0) {
            return record_id;
        }
    }
    
    return PAGE_RECORD_NOT_FOUND;
}
