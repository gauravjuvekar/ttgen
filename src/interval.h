#ifndef INTERVAL_H
#define INTERVAL_H 1

#include <glib.h>

/*! \brief Stores a contiguous time interval */
typedef struct {
	guint32 start;
	guint32 end;
} Interval;

/*! \brief Stores a collection of time intervals */
typedef GList *Intervals;

Intervals unite_Intervals(Intervals intervals, Interval interval);
Intervals difference_Intervals(Intervals intervals, Interval interval);


#endif
