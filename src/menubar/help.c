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


#include "help.h"

static const gchar *(authors[]) = {
	"Gaurav Juvekar <gauravjuvekar@gmail.com>",
	NULL
};

static const gchar license[] = {
#include "../LICENSE.hex.inc"
	, '\0'
};

static void about_CB(GtkWidget *widget, CallBackData *data) {
	(void)widget;

	gtk_show_about_dialog(
		(GtkWindow *)gtk_builder_get_object(data->builder, "main_window"),
		"license-type", GTK_LICENSE_GPL_3_0,
		"authors", authors,
		"license", license,
		"program-name", "TTgen",
		"copyright", "\302\251 2015 Gaurav Juvekar",
		"comments", "Genetic algorithm based program to generate timetables",

		NULL);

}


void init_help_menu(CallBackData *data) {
	GtkMenuItem *help_menu =
		(GtkMenuItem *)gtk_builder_get_object(
			data->builder, "help_about_menuitem");
	g_signal_connect(help_menu, "activate", G_CALLBACK(about_CB), data);
}
