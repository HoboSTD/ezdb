#ifndef PAGE_H
#define PAGE_H

#include <stddef.h>

typedef struct page* Page;

Page page_create(size_t size);

void page_free(Page* page);

#endif
