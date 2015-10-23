#ifndef CORE_GENETIC_H
#define CORE_GENETIC_H 1

#include <glib.h>

typedef struct Allocation {
	gint teacher;
	gint batch;
	gint subject;
} Allocation;

typedef struct Meta {
	const gint n_teachers;
	const gint n_batches;
	const gint n_slots;
	const gint n_rooms;
	const gint n_allocs;
	const Allocation *const allocs;
	const gint crossover_points;
	const gint mutate_swaps;
	const gfloat fitness_penalty_time_clash_teacher;
	const gfloat fitness_penalty_time_clash_batch;

} Meta;

typedef struct Schedule {
	gint   *time_slots;
	gfloat fitness;

	gint *allocations;
} Schedule;

Schedule *Schedule_init(const Meta *meta);
Schedule *Schedule_clone(const Schedule *schedule, const Meta *meta);
void     Schedule_free(Schedule *schedule);
void     Schedule_mutate(Schedule *schedule, const Meta *meta);
void     Schedule_crossover(const Schedule *mother, const Schedule *father,
						    Schedule **daughter,    Schedule **son,
                            const Meta *meta);


gfloat Schedule_fitness(const Schedule *schedule, const Meta *meta);


#endif
