#ifndef PAGE_H
#define PAGE_H

#include <stddef.h>

#define MIN_PAGE_SIZE (128)

#define PAGE_ARG_INVALID -1
#define PAGE_HAS_NO_SPACE -2
#define PAGE_HAS_NO_RECORDS -3
#define PAGE_RECORD_NOT_FOUND -4

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
 * Returns the index of the record if it was added to the page, -1 if it wasn't.
 */
int
page_add_record(Page page, void* record, size_t size);

/*
 * Returns the index of the record that was deleted from the page.
 */
int
page_delete_record(Page page, void* record, size_t size);

#endif
