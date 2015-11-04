#ifndef DB_TABLES_ALLOCATIONS_H
#define DB_TABLES_ALLOCATIONS_H 1

#include "../main.h"

typedef struct Allocation {
	gint pk;
	gint batch;
	gint subject;
	gint teacher;
} Allocation;


Allocation Allocation_from_stmt(sqlite3_stmt *stmt);
void insert_Allocation(sqlite3 *db, const Allocation *allocation);
void remove_Allocation(sqlite3 *db, gint pk);

#endif
