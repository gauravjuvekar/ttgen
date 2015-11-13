#ifndef DB_TABLES_POPULATION_H
#define DB_TABLES_POPULATION_H 1

#include "../libs.h"
#include "allocations.h"
#include "meta.h"
#include "../core/evolution.h"

Population Population_from_db(sqlite3 *db, const Meta *meta);
void delete_db_Population(sqlite3 *db);
void replace_db_Population(Population population,
                           sqlite3 *db, const Meta *meta);
void setup_population(gint schedules, sqlite3 *db,
                      const Meta *meta, const Allocation allocs[]);


#endif