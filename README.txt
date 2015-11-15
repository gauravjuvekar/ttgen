# TTgen #

Gaurav Juvekar
MIS: 111408024

A miniproject assignment to generate weekly timetables.

The core logic uses a genetic algorithm.

A *schedule* consists of *allocations* (combination of teacher, batch subject)
associated with a *time-place slot*(combination of a time-slot with a room).
For every time-slot, all rooms are checked to see if teachers or batches clash.

Every schedule is given a *fitness value*, which reduces as the number of
clashes increases. The fitness value also decreases if a batch is assigned a
room that is not large enough.

Thus, in the genetic model, each schedule acts as an "individual" with the
allocation-time-place associations being "chromosomes" or "genes" and each
individual is given a fitness value.

A collection of individuals form a *population*.
In every *generation*, two of the least fit individuals die. Two of the most fit
individuals *mate* to form two *offspring* schedules by a (two point) crossover
operation. These offspring schedules are a clone of the parents with some
genes(allocation-time-place) between them swapped.
After crossover, to maintain diversity, the offspring are *mutated*. A mutation
happens by swapping the time-place slots between a few randomly selected
allocations.
The offspring are then evaluated for their fitness and added to the population.

This evolution process is repeated either an individual reaches the target
fitness or a limit on the number of generations has exceeded.


All this information of teachers, rooms, batches, subjects, allocations,
schedules and preferences such as number of mutations, size of the population,
etc is stored in an sqlite database file. This approach was used as it was
expected that the entire state of the genetic model would need to be preserved
as the evolution process might be very expensive for large inputs
(As scheduling in general is an NP-hard problem). However, it has been observed
that inserting a completely new population into the database is a bottle-neck
because the os buffers need to be flushed to the disk to prevent corruption n
case of power-failures.


## Dependencies ##

- `gtk+-3.0 >= 3.10`, provided by `gnome-platform-devel` in Ubuntu trusty
- `xxd`
- `sqlite` - Provided in source

The build is tested with Ubuntu 12.04, in which gnome-platform-devel is
installed from trusty repos. See the build log at
https://travis-ci.org/gauravjuvekar/ttgen

## Making ##

- make

- To build the dev branch, use autotools with
  `autoreconf --install && ./configure && make`
