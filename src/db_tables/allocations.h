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


#ifndef DB_TABLES_ALLOCATIONS_H
#define DB_TABLES_ALLOCATIONS_H 1

#include "../libs.h"
#include "meta.h"

typedef struct Allocation {
	gint pk;
	gint batch;
	gint subject;
	gint teacher;
} Allocation;


Allocation Allocation_from_stmt(sqlite3_stmt *stmt);
void insert_Allocation(sqlite3 *db, const Allocation *allocation);
void remove_Allocation(sqlite3 *db, gint pk);
Allocation *Allocations_from_db(sqlite3 *db, const Meta *meta);

#endif
