#include <stdlib.h>
#include <string.h>
#include "table.h"

#define MAX_TABLE_NAME (20)

struct table
{
    char name[MAX_TABLE_NAME];
    
};

Table
table_create(char* name)
{
    Table table = malloc(sizeof(*table));
    if (table == NULL) {
        return NULL;
    }
    
    /* Copy the name but leave space for the NULL terminator. */
    memset(table->name, 0, MAX_TABLE_NAME);
    strncpy(table->name, name, MAX_TABLE_NAME - 1);

    return table;
}

void
table_free(Table table)
{
    return;
}

/*
 * PRIVATE FUNCTIONS
 */
