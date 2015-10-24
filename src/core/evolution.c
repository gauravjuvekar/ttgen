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
	for(max_fitness = FLT_MIN;
	    max_fitness < fitness && population->generations < generations;
	    population->generations++) {

		g_ptr_array_sort(population->schedules,
		                 (GCompareFunc)Schedule_compare);

		gint son      = meta->n_population - 1;
		gint daughter = meta->n_population - 2;
		gint mother   = 0;
		gint father   = 1;
		g_ptr_array_remove_index(population->schedules, son);
		g_ptr_array_remove_index(population->schedules, daughter);
		Schedule_crossover(g_ptr_array_index(population->schedules, mother),
		                   g_ptr_array_index(population->schedules, father),
		                   &g_ptr_array_index(population->schedules, son),
		                   &g_ptr_array_index(population->schedules, daughter),
		                   meta);

		Schedule_mutate(g_ptr_array_index(population->schedules, son), meta);
		Schedule_mutate(g_ptr_array_index(population->schedules, daughter),
		                meta);
	}
}
