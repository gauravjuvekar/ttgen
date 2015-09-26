#include <glib.h>

#include "interval.h"


static gint compare_Interval(const Interval *a, const Interval *b) {
	return (gint32)a->start - (gint32)b->start;
}


/*! \brief Takes the union of \p interval with \p intervals
 *
 * \param[in,out] intervals The intervals to add to
 * \param[in]     interval  The interval to add to \p intervals
 *
 * \return The resulting union
 */
Intervals unite_Intervals(Intervals intervals, const Interval interval) {
	g_assert(interval.start <  interval.end);
	g_assert(interval.start <= 7 * 24 * 60 * 60);
	g_assert(interval.end   <= 7 * 24 * 60 * 60);
	g_assert(interval.start >= 0);
	g_assert(interval.end   >= 0);

	intervals = g_list_first(intervals);

	/* Insert new interval into the list sorted */
	Intervals insert_before = intervals;
	while (interval.start >= ((Interval*)insert_before->data)->start) {
		insert_before = g_list_next(insert_before);
	}
	Interval *to_insert = (Interval*)g_memdup(&interval, sizeof(interval));
	intervals = g_list_insert_before(intervals, insert_before,
	                                 (gpointer)to_insert);

	/* Expand other intervals accordingly, deleting intervals that overlap */
	Intervals expand_from = g_list_previous(insert_before);
	if ((g_list_previous(expand_from) != NULL) &&
	    (((Interval*)g_list_previous(expand_from)->data)->end >=
	     ((Interval*)expand_from->data)->start)) {
		expand_from = g_list_previous(expand_from);
	}
	while(g_list_next(expand_from) != NULL) {
		if(((Interval*)expand_from->data)->end >=
		   (((Interval*)g_list_next(expand_from)->data)->start)) {
			((Interval*)expand_from->data)->end =
				MAX(((Interval*)expand_from->data)->end,
				    ((Interval*)g_list_next(expand_from)->data)->end);
			g_free(g_list_next(expand_from)->data);
			intervals = g_list_delete_link(intervals,
			                               g_list_next(expand_from));
		}
		else {
			break;
		}
	}

	return g_list_first(intervals);
}


/*! \brief Takes the difference of \p interval from \p intervals
 *
 * \param[in,out] intervals The intervals to remove from
 * \param[in]     interval  The interval to remove from \p intervals
 *
 * \return The resulting intervals
 */
Intervals difference_Intervals(Intervals intervals, Interval interval) {
	/* Add the interval that is to be subtracted
	 * This ensures that there will be one continuous Interval containing
	 * \p interval completely
	 */
	intervals = unite_Intervals(intervals, interval);
	/* Find that continuous Interval */
	Intervals containing = g_list_first(intervals);
	while ((g_list_next(containing) != NULL) &&
	       (interval.start >=
	        ((Interval*)g_list_next(containing)->data)->start)) {
		containing = g_list_next(containing);
	}
	g_assert(interval.end   <= (((Interval*)containing->data)->end));
	g_assert(interval.start >= (((Interval*)containing->data)->start));

	/* Subtract \p interval from it */
	if ((interval.start != ((Interval*)containing->data)->start) &&
	    (interval.end   != ((Interval*)containing->data)->end)) {
		Interval *to_insert = (Interval*)g_memdup(containing->data,
		                                          sizeof(interval));
		to_insert->start = interval.end;
		((Interval*)containing->data)->end = interval.start;
		intervals = g_list_insert_before(g_list_first(intervals),
		                                 g_list_next(containing),
		                                 to_insert);
	}
	else if ((interval.start == ((Interval*)containing->data)->start) &&
	         (interval.end   == ((Interval*)containing->data)->end)) {
		g_free(containing->data);
		intervals = g_list_delete_link(g_list_first(intervals), containing);
	}
	else {
		if (interval.start == ((Interval*)containing->data)->start) {
			((Interval*)containing->data)->start = interval.end;
		}
		else {
			((Interval*)containing->data)->end = interval.start;
		}
	}

	return g_list_first(intervals);
}
