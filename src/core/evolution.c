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


#include "../libs.h"

#include "../db_tables/db.h"

#include "evolution.h"
#include "genetic.h"


void Population_seed(Population *population,
                     const Meta *meta,
                     const Allocation allocs[]) {
	population->n_schedules = meta->n_population;
	population->schedules = g_ptr_array_sized_new(population->n_schedules);
	g_ptr_array_set_free_func(population->schedules,
	                          (GDestroyNotify)Schedule_free);
	gint individual;
	for(individual = 0; individual < population->n_schedules; individual++) {
		g_ptr_array_add(population->schedules, Schedule_init(meta));
		Schedule_seed_random(g_ptr_array_index(population->schedules,
		                                       individual),
		                                       meta, allocs);
	}
}


void Population_evolve(Population *population,
                       const gint generations,
                       const gfloat(fitness),
                       const Meta *meta,
                       const Allocation allocs[]) {
	gfloat max_fitness;
	g_ptr_array_sort(population->schedules,
					 (GCompareFunc)Schedule_compare_wrapper);
	gint elapsed_generations;
	for(max_fitness = FLT_MIN, elapsed_generations = 0;
	    max_fitness < fitness && elapsed_generations < generations;
	    elapsed_generations++) {


		gint son      = population->n_schedules - 1;
		gint daughter = population->n_schedules - 2;
		gint mother   = 0;
		gint father   = 1;
		g_ptr_array_remove_range(population->schedules, daughter, 2);
		Schedule *son_schedule;
		Schedule *daughter_schedule;

		Schedule_crossover(g_ptr_array_index(population->schedules, mother),
		                   g_ptr_array_index(population->schedules, father),
		                   &son_schedule, &daughter_schedule, meta, allocs);
		g_ptr_array_add(population->schedules, son_schedule);
		g_ptr_array_add(population->schedules, daughter_schedule);
		Schedule_mutate(g_ptr_array_index(population->schedules, son),
		                meta, allocs);
		Schedule_mutate(g_ptr_array_index(population->schedules, daughter),
		                meta, allocs);
		g_ptr_array_sort(population->schedules,
						 (GCompareFunc)Schedule_compare_wrapper);
		max_fitness = (*(Schedule *)
		               g_ptr_array_index(population->schedules, 0)).fitness;
	}
}


void Population_free(Population *population) {
	g_ptr_array_free(population->schedules, TRUE);
}
