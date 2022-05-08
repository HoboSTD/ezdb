#include <stdlib.h>
#include <string.h>
#include "table.h"

struct table
{
    char* name;
    
};

Table
table_create(char* name)
{
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
