#include <glib.h>
#include "genetic.h"


static inline gint room_from_slot(gint slot, const Meta *meta) {
	return slot % meta->n_rooms;
}
static inline gint time_slot_from_slot(gint slot, const Meta *meta) {
	return slot / meta->n_rooms;
}
static inline gint slot_from_array(const gint *array, gint slot, gint room,
                                   const Meta *meta) {
	return array[slot * meta->n_rooms + room];
}


Schedule *Schedule_init(const Meta *meta) {
	Schedule *schedule = g_new(Schedule, 1);
	*schedule = (Schedule) {
		.time_slots = g_malloc(sizeof(*(schedule->time_slots)) *
		                       meta->n_slots * meta->n_rooms),
		.fitness = 0,
		.allocations = g_malloc(sizeof(*(schedule->allocations)) *
		                        meta->n_allocs)
	};
	gint slot;
	for(slot = 0; slot < meta->n_slots * meta->n_rooms; slot++) {
		schedule->time_slots[slot] = -1;
	}
	return schedule;
}

Schedule *Schedule_clone(const Schedule *schedule, const Meta *meta) {
	Schedule *clone = g_new(Schedule, 1);
	*clone = (Schedule) {
		.time_slots = g_memdup(schedule->time_slots,
		                        sizeof(*(schedule->time_slots)) * meta->n_rooms * meta->n_slots),
		.fitness = schedule->fitness,
		.allocations = g_memdup(schedule->allocations,
		                        sizeof(*(schedule->allocations)) *
		                        meta->n_allocs)
	};
	return clone;
}

void Schedule_free(Schedule *schedule) {
	g_free(schedule->time_slots);
	g_free(schedule->allocations);
	g_free(schedule);
}

gfloat Schedule_fitness(const Schedule *schedule, const Meta *meta) {
	gint alloc;
	gfloat fitness = 0;

	for(alloc = 0; alloc < meta->n_allocs; alloc++) {
		gint slot = schedule->allocations[alloc];
		gint time_slot = time_slot_from_slot(slot, meta);

		gint time_clash_teacher = -1;
		gint time_clash_batch = -1;
		gint room;
		for (room = 0; room < meta->n_rooms; room++) {
			const gint allocation = slot_from_array(schedule->time_slots,
			                                        time_slot, room, meta);
			if (allocation == -1) continue;
			if (meta->allocs[allocation].teacher ==
			    meta->allocs[alloc].teacher) {
				time_clash_teacher += 1;
			}
			if (meta->allocs[allocation].batch ==
			    meta->allocs[alloc].batch) {
				time_clash_batch += 1;
			}
		}
		fitness += (time_clash_teacher *
		            meta->fitness_penalty_time_clash_teacher);
		fitness += (time_clash_batch *
		            meta->fitness_penalty_time_clash_batch);
	}
	return fitness;
}

void Schedule_mutate(Schedule *schedule, const Meta *meta) {
	gint swaps;
	for(swaps = 0; swaps < meta->mutate_swaps; swaps++) {
		gint first  = g_random_int_range(0, meta->n_allocs);
		gint second = g_random_int_range(0, meta->n_allocs);

		gint tmp;
		tmp = schedule->allocations[first];
		schedule->allocations[first] = schedule->allocations[second];
		schedule->allocations[second] = tmp;

		tmp = schedule->time_slots[schedule->allocations[first]];
		schedule->time_slots[schedule->allocations[first]] =
			schedule->time_slots[schedule->allocations[second]];
		schedule->time_slots[schedule->allocations[second]] = tmp;
	}
	schedule->fitness = Schedule_fitness(schedule, meta);
}

void Schedule_crossover(const Schedule *mother, const Schedule *father,
                        Schedule **daughter,    Schedule **son,
                        const Meta *meta) {
	gint crossover_1 = g_random_int_range(0, meta->n_allocs);
	gint crossover_2 = g_random_int_range(crossover_1, meta->n_allocs + 1);

	*daughter = Schedule_clone(mother, meta);
	*son      = Schedule_clone(father, meta);

	gint i;
	for(i = crossover_1; i < crossover_2; i++) {
		/* Swap son and daughter elements */
		gint tmp;
		tmp = (*son)->allocations[i];
		(*son)->allocations[i] = (*daughter)->allocations[i];
		(*daughter)->allocations[i] = tmp;

		tmp = (*son)->time_slots[(*son)->allocations[i]];
		(*son)->time_slots[(*son)->allocations[i]] =
			(*daughter)->time_slots[(*daughter)->allocations[i]];
		(*daughter)->time_slots[(*daughter)->allocations[i]] = tmp;
	}
	(*son)->fitness      = Schedule_fitness(*son, meta);
	(*daughter)->fitness = Schedule_fitness(*daughter, meta);
}
