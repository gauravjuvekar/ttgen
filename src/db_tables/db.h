#ifndef DB_TABLES_DB_H
#define DB_TABLES_DB_H 1

#include "../sqlite/sqlite3.h"

void init_db(sqlite3 *db, const char *file_name);
void new_db(sqlite3 *db);

#endif
