#include <glib.h>
#include "../sqlite/sqlite3.h"
#include "genetic.h"
#include "evolution.h"
#include "meta.h"

int main(int argc, char **argv) {
	(void)argc;
	sqlite3 *db;
	sqlite3_open(argv[1], &db);

	Meta meta = Meta_from_db(db);
	Population population;
	Population_seed(&population, &meta);
	Population_evolve(&population, 50, 10, &meta);

	g_print("Generations %d\n", population.generations);
	g_ptr_array_sort(population.schedules, (GCompareFunc)Schedule_compare_wrapper);
	gint i;
	for(i = 0; i < meta.n_population; i++) {
		g_print("Fitness[%d] =  %f\n", i,
		        (*(Schedule *)g_ptr_array_index(population.schedules, i))
		        .fitness);
	}

	return 0;
}
