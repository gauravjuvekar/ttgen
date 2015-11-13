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


#ifndef DB_TABLES_DB_H
#define DB_TABLES_DB_H 1

#include "../libs.h"
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
