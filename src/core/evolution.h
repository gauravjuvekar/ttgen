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


#ifndef CORE_EVOLUTION_H
#define CORE_EVOLUTION_H 1

#include "../libs.h"
#include "../db_tables/db.h"
#include "genetic.h"

typedef struct Population {
	GPtrArray *schedules;
	gint n_schedules;
} Population;


void Population_seed(Population *population,
                     const Meta *meta,
                     const Allocation allocs[],
                     const Batch      batches[],
                     const Room       rooms[]);
void Population_evolve(Population *population,
                       const gint generations,
                       const gfloat(fitness),
                       const Meta *meta,
                       const Allocation allocs[],
                       const Batch      batches[],
                       const Room       rooms[]);
void Population_free(Population *population);

#endif
