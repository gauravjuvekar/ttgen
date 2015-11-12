#include "prefs.h"

#include "../main.h"

#include "../db_tables/db.h"


static void prefs_menuitem_CB(GtkWidget *widget, CallBackData *data) {
	(void)widget;
	GtkWidget *window = (GtkWidget *)gtk_builder_get_object(
		data->builder, "preferences_window");
	gtk_widget_show_all(window);
}


static void set_prefs_CB(GtkButton *button, CallBackData *data) {
	(void)button;
	Meta meta;
	Meta db_meta = Meta_from_db(data->db);

	meta.n_time_slots_per_day = gtk_spin_button_get_value_as_int(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_time_slots_per_day_spinbutton"));
	meta.n_time_slots = meta.n_time_slots_per_day *
		gtk_spin_button_get_value_as_int(
			(GtkSpinButton *)gtk_builder_get_object(
				data->builder,
				"preferences_window_working_days_spinbutton"));
	meta.n_population = gtk_spin_button_get_value_as_int(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_population_spinbutton"));
	meta.mutate_swaps = gtk_spin_button_get_value_as_int(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_mutation_swaps_spinbutton"));
	meta.fitness_penalty_time_clash_teacher = gtk_spin_button_get_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_fitness_penalty_time_clash_teacher_spinbutton"));
	meta.fitness_penalty_time_clash_batch = gtk_spin_button_get_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_fitness_penalty_time_clash_batch_spinbutton"));


	if (db_meta.n_time_slots != meta.n_time_slots) {
		meta.db_schedules_valid = 0;
	}

	insert_Meta(data->db, &meta);

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
	Meta meta = Meta_from_db(data->db);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_time_slots_per_day_spinbutton"),
		meta.n_time_slots_per_day);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_working_days_spinbutton"),
		meta.n_time_slots / meta.n_time_slots_per_day);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_population_spinbutton"),
		meta.n_population);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_mutation_swaps_spinbutton"),
		meta.mutate_swaps);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_fitness_penalty_time_clash_teacher_spinbutton"),
		meta.fitness_penalty_time_clash_teacher);
	gtk_spin_button_set_value(
		(GtkSpinButton *)gtk_builder_get_object(
			data->builder,
			"preferences_window_fitness_penalty_time_clash_batch_spinbutton"),
		meta.fitness_penalty_time_clash_batch);
}
