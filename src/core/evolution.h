#ifndef CORE_EVOLUTION_H
#define CORE_EVOLUTION_H 1

#include "../main.h"
#include "genetic.h"

typedef struct Population {
	GPtrArray *schedules;
	gint generations;
	gint n_schedules;
} Population;


void Population_seed(Population *population, const Meta *meta);
void Population_evolve(Population *population,
                       const gint generations,
                       const gfloat(fitness),
                       const Meta *meta);
void Population_free(Population *population);

#endif
