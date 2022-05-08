#ifndef TABLE_H
#define TABLE_H

typedef struct table* Table;

/*
 * Returns a table with the given name.
 * Returns NULL if the table couldn't be created.
 */
Table
table_create(char* name);

/*
 * Frees the memory associated with the table.
 * TODO: should the table commit before doing this?
 */
void
table_free(Table table);

#endif
