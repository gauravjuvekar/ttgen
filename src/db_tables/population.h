#ifndef DB_TABLES_POPULATION_H
#define DB_TABLES_POPULATION_H 1

#include "../main.h"
#include "../core/evolution.h"

Population Population_from_db(sqlite3 *db, const Meta *meta);
void replace_db_Population(Population population,
                           sqlite3 *db, const Meta *meta);
void extend_db_Population(Population population,
                          sqlite3 *db, const Meta *meta);


#endif
