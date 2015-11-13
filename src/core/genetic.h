#ifndef CORE_GENETIC_H
#define CORE_GENETIC_H 1

#include "../libs.h"

#include "../db_tables/meta.h"
#include "../db_tables/allocations.h"


typedef struct Schedule {
	gint   *time_slots;
	gfloat fitness;

	gint *allocations;
} Schedule;

Schedule *Schedule_init(const Meta *meta);
void Schedule_seed_random(Schedule *schedule,
                          const Meta *meta,
                          const Allocation allocs[]);
Schedule *Schedule_clone(const Schedule *schedule, const Meta *meta);
void Schedule_free(Schedule *schedule);
void Schedule_mutate(Schedule *schedule,
                     const Meta *meta,
                     const Allocation allocs[]);
void Schedule_crossover(const Schedule *mother, const Schedule *father,
	    			    Schedule **daughter,    Schedule **son,
                        const Meta *meta, const Allocation allocs[]);

gfloat Schedule_fitness(const Schedule *schedule,
                        const Meta *meta,
                        const Allocation allocs[]);
gint Schedule_compare(const Schedule *a, const Schedule *b);
gint Schedule_compare_wrapper(const Schedule **a, const Schedule **b);

void Schedule_print(const Schedule *schedule,
                    const Meta *meta,
                    const Allocation allocs[]);

static inline gint room_from_slot(gint slot, const Meta *meta) {
	return slot % meta->n_rooms;
}
static inline gint time_slot_from_slot(gint slot, const Meta *meta) {
	return slot / meta->n_rooms;
}

#endif
