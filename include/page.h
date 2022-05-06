#ifndef PAGE_H
#define PAGE_H

#include <stddef.h>

#define MIN_PAGE_SIZE (128)

#define PAGE_ARG_INVALID -1
#define PAGE_HAS_NO_SPACE -2
#define PAGE_RECORD_NOT_FOUND -3

typedef struct page* Page;

/*
 * Returns a page of size, size (including the header).
 * Returns NULL if the page could not be created.
 */
Page page_create(size_t size);

/*
 * Frees the memory associated with the page, sets the reference to NULL.
 */
void page_free(Page* page);

/*
 * Returns the index of the record if it was added to the page.
 * Returns PAGE_ARG_INVALID if the given arguments are invalid.
 * Returns PAGE_HAS_NO_SPACE if there was not enough space for the record.
 */
int
page_add_record(Page page, void* record, size_t size);

/*
 * Returns the index of the record that was deleted from the page.
 * Returns PAGE_ARG_INVALID if the given arguments are invalid.
 * Returns PAGE_RECORD_NOT_FOUND if no "record" existed on the page.
 */
int
page_delete_record(Page page, void* record, size_t size);

/*
 * Returns zero on successful update.
 * Returns PAGE_ARG_INVALID if the given arguments are invalid.
 * Returns PAGE_RECORD_NOT_FOUND if no "old" record existed on the page.
 */
int
page_update_record(Page page, void* old, void* new, size_t size);

/*
 * Returns a pointer to the record.
 * Returns NULL if the record_id is invalid.
 */
void*
page_read_record(Page page, int record_id);

#endif
