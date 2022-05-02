#include <stdlib.h>
#include "page.h"

struct page
{
    int     ntuples;
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
    
    page->ntuples = 0;

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
