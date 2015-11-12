#ifndef DB_TABLES_DB_H
#define DB_TABLES_DB_H 1

#include "../main.h"
#include "allocations.h"
#include "meta.h"
#include "teachers.h"
#include "batches.h"
#include "subjects.h"
#include "rooms.h"
#include "population.h"
#include "schedule.h"

void init_connection(sqlite3 **db, const char *file_name);
void init_db(sqlite3 *db);
void new_db(sqlite3 *db);
void reset_pks(sqlite3 *db);

typedef void (*remove_DB_func)(sqlite3 *db, gint pk);

#endif
