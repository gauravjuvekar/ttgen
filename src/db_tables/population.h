/*  TTgen: An application to generate timetables using a genetic algorithm.
 *  Copyright (C) 2015  Gaurav Juvekar
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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
