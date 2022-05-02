#ifndef PAGE_H
#define PAGE_H

#include <stddef.h>

#define MIN_PAGE_SIZE (128)

typedef struct page* Page;

/*
 * Returns a page of size size (including the header).
 */
Page page_create(size_t size);

/*
 * Frees the memory associated with the page, sets the reference to NULL.
 */
void page_free(Page* page);

/*
 * Returns non-zero if the record was added to the page.
 */
int
page_add_record(Page page, void* record, size_t size);

/*
 * Returns the index of the record that was deleted from the page.
 */
int
page_delete_record(Page page, void* record, size_t size);

#endif
