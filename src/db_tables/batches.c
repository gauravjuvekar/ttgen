#include "../sqlite/sqlite3.h"
#include "batches.h"


Batch Batch_from_stmt(sqlite3_stmt *stmt) {
	return (Batch) {
		.pk    = sqlite3_column_int(stmt, 0),
		.name  = (const gchar*) sqlite3_column_text(stmt, 1),
		.heads = sqlite3_column_int(stmt, 2)
	};
}
