*This project has been created as part of the 42 curriculum by ldubau.*

# Philosophers

## Description

**Philosophers** is an introduction to concurrent programming with threads and
mutexes, based on Dijkstra's *dining philosophers* problem.

A number of philosophers sit around a round table with a large bowl of spaghetti.
There is one fork between each pair of neighbours, so there are as many forks as
philosophers. A philosopher needs **two forks** (left and right) to eat. Each
philosopher cycles through three states: **eating → sleeping → thinking**.

- Each philosopher is a thread.
- Each fork is protected by a mutex, so it can only be held by one philosopher at a time.
- A philosopher dies if they have not *started* a meal within `time_to_die`
  milliseconds since the start of their last meal (or the start of the simulation).
- The simulation stops as soon as one philosopher dies, or when every philosopher
  has eaten at least `number_of_times_each_philosopher_must_eat` times (if given).

The goal is to keep every philosopher alive as long as possible while avoiding
deadlocks and data races, and to report each state change with accurate timestamps.

## Instructions

### Compilation

```sh
make        # builds ./philo
make clean  # removes object files
make fclean # removes object files and the binary
make re     # full rebuild
```

The project is written in C, compiled with `cc -Wall -Wextra -Werror` and linked
with `-pthread`.

### Execution

```sh
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Meaning |
|---|---|
| `number_of_philosophers` | Number of philosophers, and of forks |
| `time_to_die` (ms) | Max time between the start of two meals before dying |
| `time_to_eat` (ms) | Time spent eating (holding two forks) |
| `time_to_sleep` (ms) | Time spent sleeping |
| `number_of_times_each_philosopher_must_eat` | *Optional.* The simulation stops once every philosopher has eaten this many times |

All arguments must be positive integers; otherwise the program exits with status `1`.

### Output

Each state change is printed as `timestamp_in_ms philosopher_id action`:

```
0 1 has taken a fork
0 1 has taken a fork
0 1 is eating
200 1 is sleeping
400 1 is thinking
...
310 2 died
```

### Examples

```sh
./philo 1 800 200 200       # a single philosopher has one fork: dies at 800
./philo 4 410 200 200       # nobody should die
./philo 4 310 200 100       # a philosopher dies
./philo 5 800 200 200 7     # stops once everyone has eaten 7 times
```

To check for leaks and data races:

```sh
valgrind --leak-check=full ./philo 4 410 200 200 5
valgrind --tool=helgrind ./philo 4 410 200 200 5
```

## Technical choices

- **Fork ordering**: odd-numbered philosophers take their right fork first, even-numbered
  ones their left fork first. This breaks the circular wait and prevents deadlocks.
- **Staggered start**: odd-numbered philosophers wait `time_to_eat / 2` before their
  first meal so neighbours do not all compete for the same forks at once.
- **Monitor thread**: a dedicated thread loops over the philosophers, checks each
  `last_meal` against `time_to_die`, counts the philosophers who are full, and sets
  `end_simulation` when the simulation must stop.
- **Mutexes**:
  - one per fork;
  - one per philosopher (`philo_mutex`), protecting `last_meal`, `nbr_meal` and `full`;
  - `sim_mutex`, protecting `end_simulation`;
  - `print_mutex`, so log lines never interleave and nothing is printed after a death.
- **Precise sleeping**: `ft_usleep` sleeps in small `usleep(500)` steps and checks the
  elapsed time with `gettimeofday`, because a single long `usleep` can oversleep.

## Resources

- [The Dining Philosophers problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- E. W. Dijkstra, *Hierarchical ordering of sequential processes* (1971), the origin of the problem
- [POSIX threads man pages](https://man7.org/linux/man-pages/man7/pthreads.7.html):
  `pthread_create(3)`, `pthread_join(3)`, `pthread_mutex_lock(3)`
- [`gettimeofday(2)`](https://man7.org/linux/man-pages/man2/gettimeofday.2.html) and
  [`usleep(3)`](https://man7.org/linux/man-pages/man3/usleep.3.html)
- [POSIX Threads Programming — LLNL tutorial](https://hpc-llnl.github.io/tutorials/posix/)
- [Valgrind Helgrind manual](https://valgrind.org/docs/manual/hg-manual.html) for detecting data races

### Use of AI

understanding threads, mutexes, data races and
  deadlocks, and reviewing my own code.

writing this README and the `.gitignore`.
