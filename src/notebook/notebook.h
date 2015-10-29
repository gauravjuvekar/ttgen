#ifndef NOTEBOOK_NOTEBOOK_H
#define NOTEBOOK_NOTEBOOK_H 1

#include "../main.h"

#include "rooms.h"
#include "batches.h"
#include "subjects.h"
#include "teachers.h"


typedef enum NotebookTabs {
	TAB_ALLOCATION,
	TAB_TEACHERS,
	TAB_ROOMS,
	TAB_BATCHES,
	TAB_SUBJECTS
} NotebookTabs;

void add_button_CB(GtkButton *button, CallBackData *data);
void remove_button_CB(GtkButton *button, CallBackData *data);

void init_notebooks(CallBackData *cb_data);


#endif
