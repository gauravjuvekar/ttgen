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
		meta.n_generations = 0;
	}
	reset_pks(data->db);
	setup_population(meta.n_population, data->db, &meta);
	reset_pks(data->db);

	Population population = Population_from_db(data->db, &meta);
	Population_evolve(&population, target_generations, target_fitness, &meta);
	replace_db_Population(population, data->db, &meta);

	meta.db_schedules_valid = 1;
	meta.n_generations = population.generations;
	insert_Meta(data->db, &meta);

	GtkLabel *label = (GtkLabel *)gtk_builder_get_object(
		data->builder, "current_generations_label");
	const gchar *generations = g_strdup_printf("%d", meta.n_generations);
	gtk_label_set_text(label, generations);
	g_free((gpointer) generations);
	refresh_notebook_schedules(data);
}


void init_core(CallBackData *data) {
	g_signal_connect(gtk_builder_get_object(data->builder, "evolve_button"),
	                 "clicked", G_CALLBACK(evolve_CB), data);
}
