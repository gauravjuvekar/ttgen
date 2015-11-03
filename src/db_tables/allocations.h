#ifndef DB_TABLES_ALLOCATIONS_H
#define DB_TABLES_ALLOCATIONS_H 1

#include "../main.h"

typedef struct Allocation {
	gint        pk;
	const gchar *name;
	gint        capacity;
	gint        parallel;
} Allocation;


Allocation Allocation_from_stmt(sqlite3_stmt *stmt);

#endif
