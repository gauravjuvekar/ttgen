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
#include "../callback.h"

#include "../db_tables/db.h"
#include "../notebook/notebook.h"
#include "interface.h"

static void evolve_CB(GtkButton *button, CallBackData *data) {
	(void)button;
	Meta meta = Meta_from_db(data->db);
	if (!meta.db_schedules_valid) {
		delete_db_Population(data->db);
	}

	if (meta.n_time_slots * meta.n_rooms < meta.n_allocs) {
		GtkMessageDialog *dialog = (GtkMessageDialog *)gtk_message_dialog_new(
			(GtkWindow *)gtk_builder_get_object(data->builder, "main_window"),
			GTK_DIALOG_DESTROY_WITH_PARENT,
			GTK_MESSAGE_ERROR,
			GTK_BUTTONS_CLOSE,
			"Number of allocations[%d] is greater than "
			"total number of slots(time slots * rooms)[%d].\n"
			"There is no feasible solution. Reduce allocations or increase "
			"rooms and/or time slots.",
			meta.n_allocs, (meta.n_time_slots * meta.n_rooms));
		gtk_dialog_run(GTK_DIALOG(dialog));
		gtk_widget_destroy((GtkWidget *)dialog);
		return;
	}

	gfloat target_fitness = gtk_spin_button_get_value(
		(GtkSpinButton *)gtk_builder_get_object(data->builder,
		                                        "target_fitness_spin_button"));
	gint target_generations = gtk_spin_button_get_value_as_int(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder, "target_generations_spin_button"));
	reset_pks(data->db);
	const Allocation *allocs  = Allocations_from_db(data->db, &meta);
	const Batch      *batches = Batches_from_db(data->db, &meta);
	const Room       *rooms   = Rooms_from_db(data->db, &meta);

	setup_population(meta.n_population, data->db,
	                 &meta, allocs, batches, rooms);
	reset_pks(data->db);

	Population population = Population_from_db(data->db, &meta);
	Population_evolve(&population, target_generations, target_fitness,
	                  &meta, allocs, batches, rooms);
	g_free((gpointer)allocs);
	replace_db_Population(population, data->db, &meta);
	Population_free(&population);

	meta.db_schedules_valid = 1;
	insert_Meta(data->db, &meta);


	g_free((gpointer)allocs);
	gint i;
	for(i = 0; i < meta.n_batches; i++) {
		g_free((gpointer)batches[i].name);
	}
	g_free((gpointer)batches);
	for(i = 0; i < meta.n_batches; i++) {
		g_free((gpointer)rooms[i].name);
	}
	g_free((gpointer)rooms);


	refresh_notebook_schedules(data);
}


void init_core(CallBackData *data) {
	g_signal_connect(gtk_builder_get_object(data->builder, "evolve_button"),
	                 "clicked", G_CALLBACK(evolve_CB), data);
}
