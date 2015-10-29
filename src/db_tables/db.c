#include "../main.h"

#include "db.h"

static const char schema[] = {
#include "../database.sqlite3.hex.inc"
	, '\0'
};

void init_db(sqlite3 *db, const char *file_name) {
	sqlite3_open(file_name, &db);
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, "PRAGMA foreign_keys = ON;", -1, &stmt, NULL);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

void new_db(sqlite3 *db) {
	sqlite3_stmt *stmt;
	sqlite3_prepare(db, schema, -1, &stmt, NULL);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}
