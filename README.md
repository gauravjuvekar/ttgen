# TTGEN #

<!---- After fighting with Travis CI for a long time and getting it to install
from trusty repos ---->
[![Build Status](https://travis-ci.org/gauravjuvekar/ttgen.svg?branch=master)](https://travis-ci.org/gauravjuvekar/ttgen)

A miniproject assignment to generate weekly timetables.

## Dependencies ##

- `gtk+-3.0 >= 3.10`, provided by `gnome-platform-devel` in Ubuntu trusty
- `xxd`
- `sqlite` - Provided in source

## Making ##

- make

- To build the dev branch, use autotools with
  `autoreconf --install && ./configure && make`
