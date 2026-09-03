*This project has been created as part of the 42 curriculum by <masolet->.*

# Philosophers

## Description
This project is an introduction to concurrent programming, multi-threading, and process synchronization using POSIX threads and mutexes in C. It is a classical implementation of **Dining Philosophers Problem**.

The goal of the project is to simulate a group of philosophers sitting around a circular table with a bowl of spaghetti in the middle and forks placed between each pair of adjacent philosophers. A philosopher must alternate between **eating**, **sleeping**, and **thinking**. To eat, a philosopher must acquire both their left and right forks.

### Objectives & Challenges
- **Thread Management**: Each philosopher is represented by an independent POSIX thread.
- **Mutex Protection**: Shared resources (forks and state variables) are protected using mutexes to prevent race conditions and duplicate fork usage.
- **Precision Timing**: Microsecond-calibrated sleep routines and precise death detection.

---

## Instructions

### Compilation
The project is built using `make` with flags (`-Wall -Wextra -Werror`):

```bash
cd philo
make
```

Available Makefile rules:
- `make` or `make all` : Compiles the `philo` executable.
- `make clean` : Removes compiled object files.
- `make fclean` : Removes object files and the `philo` executable.
- `make re` : Recompiles the entire project.

### Execution
Run the executable with 4 or 5 arguments:

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

#### Command Line Arguments
1. `number_of_philosophers`: The number of philosophers (and forks).
2. `time_to_die` (in ms): Time limit since the start of a philosopher's last meal before dying of starvation.
3. `time_to_eat` (in ms): Duration a philosopher spends eating (holding two forks).
4. `time_to_sleep` (in ms): Duration a philosopher spends sleeping.
5. `[number_of_times_each_philosopher_must_eat]` *(optional)*: If specified, simulation stops when all philosophers have eaten at least this many times.

#### Usage Examples

```bash
# Philosopher 1 should die
./philo 1 800 200 200

# No philosopher should die
./philo 5 800 200 200

# Stop after each philosopher eats 7 times
./philo 5 800 200 200 7
```

---

## Resources

### References & Documentation
- [POSIX Threads (pthreads) Programming Guide](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)

### AI Usage Declaration
- **Code Optimization**: AI was used to create a tester.
- **README Language**: AI was used to translate this `README.md` in english.