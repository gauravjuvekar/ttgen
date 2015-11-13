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

#include "prefs.h"


static void prefs_menuitem_CB(GtkWidget *widget, CallBackData *data) {
	(void)widget;
	GtkWidget *window = (GtkWidget *)gtk_builder_get_object(
		data->builder, "preferences_window");
	gtk_widget_show_all(window);
}


static void set_prefs_CB(GtkButton *button, CallBackData *data) {
	(void)button;

	data->meta->n_time_slots_per_day = gtk_spin_button_get_value_as_int(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_time_slots_per_day_spinbutton"));
	gint new_n_time_slots = data->meta->n_time_slots_per_day *
		gtk_spin_button_get_value_as_int(
			(GtkSpinButton *)gtk_builder_get_object(
				data->builder,
				"preferences_window_working_days_spinbutton"));
	if (data->meta->n_time_slots != new_n_time_slots) {
		data->meta->db_schedules_valid = 0;
	}
	data->meta->n_time_slots = new_n_time_slots;

	data->meta->n_population = gtk_spin_button_get_value_as_int(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_population_spinbutton"));
	data->meta->mutate_swaps = gtk_spin_button_get_value_as_int(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_mutation_swaps_spinbutton"));
	data->meta->fitness_penalty_time_clash_teacher = gtk_spin_button_get_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_fitness_penalty_time_clash_teacher_spinbutton"));
	data->meta->fitness_penalty_time_clash_batch = gtk_spin_button_get_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_fitness_penalty_time_clash_batch_spinbutton"));

	insert_Meta(data->db, data->meta);

	gtk_widget_hide((GtkWidget *)gtk_builder_get_object(data->builder,
														"preferences_window"));
}


static void cancel_prefs_CB(GtkButton *widget, CallBackData *data) {
	(void)widget;
	gtk_widget_hide((GtkWidget *)gtk_builder_get_object(data->builder,
														"preferences_window"));
}


static gboolean close_prefs_window_CB(GtkWidget *widget,
                                      GdkEvent  *event,
                                      CallBackData *data) {
	(void)widget;
	(void)event;
	cancel_prefs_CB(NULL, data);
	return TRUE;
}


void init_prefs(CallBackData *data) {
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "edit_preferences_menuitem"),
	                 "activate", G_CALLBACK(prefs_menuitem_CB), data);
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "preferences_window_cancel_button"),
	                 "clicked",
	                 G_CALLBACK(cancel_prefs_CB), data);
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "preferences_window"),
	                 "delete-event",
	                 G_CALLBACK(close_prefs_window_CB), data);
	g_signal_connect(gtk_builder_get_object(data->builder,
	                                        "preferences_window_ok_button"),
	                 "clicked",
	                 G_CALLBACK(set_prefs_CB), data);
}


void refresh_prefs(CallBackData *data) {
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_time_slots_per_day_spinbutton"),
		data->meta->n_time_slots_per_day);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_working_days_spinbutton"),
		data->meta->n_time_slots / data->meta->n_time_slots_per_day);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_population_spinbutton"),
		data->meta->n_population);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_mutation_swaps_spinbutton"),
		data->meta->mutate_swaps);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_fitness_penalty_time_clash_teacher_spinbutton"),
		data->meta->fitness_penalty_time_clash_teacher);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_fitness_penalty_time_clash_batch_spinbutton"),
		data->meta->fitness_penalty_time_clash_batch);
}
