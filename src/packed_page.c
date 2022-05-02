#include <stdlib.h>
#include <string.h>
#include "page.h"

struct page
{
    size_t  size;
    int     n_tuples;
    char    data[1];
};

static int
page_min_size()
{
    return sizeof(struct page);
}

Page
page_create(size_t size)
{
    if (size < page_min_size()) {
        return NULL;
    }

    Page page = malloc(size);
    if (page == NULL) {
        return NULL;
    }
    
    page->size = size;
    page-> n_tuples = 0;

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

/*
 * For a packed page, size is assumed to be constant.
 */
int
page_add_record(Page page, void* record, size_t size)
{
    if (record == NULL) {
        return 0;
    }
    
    if (size > page->size) {
        return 0;
    }

    memcpy(page->data + (page->n_tuples * size), record, size);

    return ++page->n_tuples;
}
