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


#ifndef NOTEBOOK_NOTEBOOK_H
#define NOTEBOOK_NOTEBOOK_H 1

#include "../libs.h"
#include "../callback.h"

#include "rooms.h"
#include "batches.h"
#include "subjects.h"
#include "teachers.h"
#include "allocations.h"
#include "schedules.h"


typedef enum NotebookTabs {
	TAB_ALLOCATIONS,
	TAB_SCHEDULES,
	TAB_TEACHERS,
	TAB_ROOMS,
	TAB_BATCHES,
	TAB_SUBJECTS
} NotebookTabs;

void add_button_CB(GtkButton *button, CallBackData *data);
void remove_button_CB(GtkButton *button, CallBackData *data);

void init_notebooks(CallBackData *cb_data);
void refresh_notebooks(CallBackData *cb_data);
typedef void (*refresh_notebook_func)(CallBackData *data);


#endif
