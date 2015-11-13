#ifndef CALLBACK_H
#define CALLBACK_H 1

#include "libs.h"
#include "db_tables/meta.h"

typedef struct CallBackData {
	sqlite3    *db;
	GtkBuilder *builder;
	gchar      *db_name;
	Meta       *meta;
} CallBackData;

#endif
