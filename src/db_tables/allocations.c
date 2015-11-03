#include "../main.h"

#include "allocations.h"

Allocation Allocation_from_stmt(sqlite3_stmt *stmt) {
	return (Allocation) {
		.pk       = sqlite3_column_int(stmt, 0),
		.name     = (const gchar*) sqlite3_column_text(stmt, 1),
		.capacity = sqlite3_column_int(stmt, 2)
	};
}
