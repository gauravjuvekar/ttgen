#ifndef DB_TABLES_SCHEDULE_H
#define DB_TABLES_SCHEDULE_H 1

#include "../main.h"
#include "../db.h"
#include "../core/genetic.h"

Schedule *Schedule_from_db(gint pk, sqlite3 *db, const Meta *meta);
void insert_Schedule(Schedule *schedule, sqlite3 *db, const Meta *meta);

#endif
