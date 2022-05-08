#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "table.h"

struct table
{
    char* name;
    
};

static int is_valid_name(char* name);

Table
table_create(char* name)
{
    if (!is_valid_name(name)) {
        return NULL;
    }

    Table table = malloc(sizeof(*table));
    if (table == NULL) {
        return NULL;
    }
    
    table->name = strdup(name);
    if (table->name == NULL) {
        table_free(table);
        return NULL;
    }

    return table;
}

void
table_free(Table table)
{
    free(table->name);
    free(table);
}

/*
 * PRIVATE FUNCTIONS
 */

static int
is_valid_name(char* name)
{
    if (name == NULL) {
        return 0;
    }

    size_t len = strlen(name);
    return 0 < len && len < NAME_MAX;
}
