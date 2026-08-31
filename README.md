*This project has been created as part of the 42 curriculum by maxslvt.*

# Philosophers - I never thought philosophy would be so deadly

## Description
This project is an introduction to concurrent programming, multi-threading, and process synchronization using POSIX threads (`pthreads`) and mutexes in C. It is a classical implementation of Dijkstra's **Dining Philosophers Problem**.

The goal of the project is to simulate a group of philosophers sitting around a circular table with a bowl of spaghetti in the middle and forks placed between each pair of adjacent philosophers. A philosopher must alternate between **eating**, **sleeping**, and **thinking**. To eat, a philosopher must acquire both their left and right forks.

### Objectives & Challenges
- **Thread Management**: Each philosopher is represented by an independent POSIX thread.
- **Mutex Protection**: Shared resources (forks and state variables) are protected using mutexes (`pthread_mutex_t`) to prevent race conditions and duplicate fork usage.
- **Deadlock Avoidance**: Asymmetric fork acquisition strategy to guarantee that circular waits never occur.
- **Precision Timing**: Microsecond-calibrated sleep routines and precise death detection (< 10 ms latency).

---

## Instructions

### Compilation
The project is built using `make` with standard 42 flags (`-Wall -Wextra -Werror`):

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
# Philosopher 1 should die at 800 ms
./philo 1 800 200 200

# No philosopher should die
./philo 5 800 200 200

# Stop after each philosopher eats 7 times
./philo 5 800 200 200 7

# Stress test (no death)
./philo 4 410 200 200
```

---

## Resources

### References & Documentation
- [POSIX Threads (pthreads) Programming Guide](https://man7.org/linux/man-pages/man7/pthreads.7.html)
- [Dining Philosophers Problem - Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [GNU C Library: Date and Time Operations (`gettimeofday`)](https://www.gnu.org/software/libc/manual/html_node/High_Resolution_Calendar.html)
- [ThreadSanitizer (`-fsanitize=thread`) Documentation](https://clang.llvm.org/docs/ThreadSanitizer.html)

### AI Usage Declaration
In accordance with the 42 curriculum guidelines regarding AI assistance:
- **Code Optimization & Profiling**: AI was used to analyze CPU usage bottlenecks, identify high-frequency polling in thread synchronization loops, and optimize `usleep` granularity down to 500 micro-seconds.
- **Race Condition Prevention**: AI assisted in auditing mutex scopes across shared data structures (`t_data` and `t_philo`) to ensure strict ThreadSanitizer compliance.
- **Documentation & README Structure**: AI was used to format and structure this `README.md` to conform strictly to 42 Chapter VII requirements.