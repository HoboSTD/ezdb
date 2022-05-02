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
page_header_size()
{
    return sizeof(struct page);
}

Page
page_create(size_t size)
{
    if (size <= page_header_size() || size < MIN_PAGE_SIZE) {
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

static int
page_has_space(Page page, size_t size)
{
    return (page->size - page_header_size() - page->n_tuples * size) > size;
}

/*
 * For a packed page, size is assumed to be constant.
 */
int
page_add_record(Page page, void* record, size_t size)
{
    if (page == NULL || record == NULL) {
        return 0;
    }
    
    if (!page_has_space(page, size)) {
        return 0;
    }

    memcpy(page->data + (page->n_tuples * size), record, size);

    return ++page->n_tuples;
}
