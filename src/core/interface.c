#include "interface.h"

#include "../main.h"
#include "../db_tables/db.h"
#include "../notebook/notebook.h"

static void evolve_CB(GtkButton *button, CallBackData *data) {
	(void)button;
	gfloat target_fitness = gtk_spin_button_get_value(
		(GtkSpinButton *)gtk_builder_get_object(data->builder,
		                                        "target_fitness_spin_button"));
	gint target_generations = gtk_spin_button_get_value_as_int(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder, "target_generations_spin_button"));

	Meta meta = Meta_from_db(data->db);
	if (!meta.db_schedules_valid) {
		delete_db_Population(data->db);
	}
	setup_population(meta.n_population, data->db, &meta);
	reset_pks(data->db);

	Population population = Population_from_db(data->db, &meta);
	Population_evolve(&population, target_generations, target_fitness, &meta);
	replace_db_Population(population, data->db, &meta);

	meta.db_schedules_valid = 1;
	insert_Meta(data->db, &meta);

	refresh_notebook_schedules(data);

	g_print("Generations %d\n", population.generations);
	g_ptr_array_sort(population.schedules, (GCompareFunc)Schedule_compare_wrapper);
	gint i;
	for(i = 0; i < meta.n_population; i++) {
		g_print("Fitness[%d] = %f\n", i,
		        (*(Schedule *)g_ptr_array_index(population.schedules, i))
		        .fitness);
		Schedule_print((Schedule *)g_ptr_array_index(population.schedules, i),
		               &meta);
	}
}


void init_core(CallBackData *data) {
	g_signal_connect(gtk_builder_get_object(data->builder, "evolve_button"),
	                 "clicked", G_CALLBACK(evolve_CB), data);

}
