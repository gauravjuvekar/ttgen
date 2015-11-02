#include <glib.h>

#include "evolution.h"
#include "genetic.h"


void Population_seed(Population *population, const Meta *meta) {
	population->generations = 0;
	population->schedules = g_ptr_array_sized_new(meta->n_population);
	g_ptr_array_set_free_func(population->schedules,
	                          (GDestroyNotify)Schedule_free);
	gint individual;
	for(individual = 0; individual < meta->n_population; individual++) {
		g_ptr_array_add(population->schedules, Schedule_init(meta));
		Schedule_seed_random(g_ptr_array_index(population->schedules,
		                                       individual),
		                                       meta);
	}
}


void Population_evolve(Population *population,
                       const gint generations,
                       const gfloat(fitness),
                       const Meta *meta) {
	gfloat max_fitness;
	g_ptr_array_sort(population->schedules,
					 (GCompareFunc)Schedule_compare_wrapper);
	for(max_fitness = FLT_MIN;
	    max_fitness < fitness && population->generations < generations;
	    population->generations++) {


		gint son      = meta->n_population - 1;
		gint daughter = meta->n_population - 2;
		gint mother   = 0;
		gint father   = 1;
		g_ptr_array_remove_range(population->schedules, daughter, 2);
		Schedule *son_schedule;
		Schedule *daughter_schedule;

		Schedule_crossover(g_ptr_array_index(population->schedules, mother),
		                   g_ptr_array_index(population->schedules, father),
		                   &son_schedule, &daughter_schedule, meta);
		g_ptr_array_add(population->schedules, son_schedule);
		g_ptr_array_add(population->schedules, daughter_schedule);
		Schedule_mutate(g_ptr_array_index(population->schedules, son), meta);
		Schedule_mutate(g_ptr_array_index(population->schedules, daughter),
		                meta);
		g_ptr_array_sort(population->schedules,
						 (GCompareFunc)Schedule_compare_wrapper);
		max_fitness = (*(Schedule *)
		               g_ptr_array_index(population->schedules, 0)).fitness;
	}
}
