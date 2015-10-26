#ifndef CORE_EVOLUTION_H
#define CORE_EVOLUTION_H 1

#include <glib.h>
#include "genetic.h"

typedef struct Population {
	GPtrArray *schedules;
	gint generations;
} Population;


void Population_seed(Population *population, const Meta *meta);
void Population_evolve(Population *population,
                       const gint generations,
                       const gfloat(fitness),
                       const Meta *meta);

#endif