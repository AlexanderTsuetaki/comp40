#!/bin/sh
/usr/bin/time ./um progs/midmark.um >/dev/null 2> midmark-time
/usr/bin/time ./um progs/sandmark.umz >/dev/null 2> sandmark-time
/usr/bin/time ./um progs/advent.umz < progs/advent_test >/dev/null 2> advent-time
valgrind --tool=callgrind --dump-instr=yes ./um progs/midmark.um
#valgrind --tool=callgrind --dump-instr=yes ./um progs/sandmark.umz
#valgrind --tool=callgrind --dump-instr=yes ./um progs/advent.umz < progs/advent_test
