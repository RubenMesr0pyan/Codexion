*This project has been created as part of the 42 curriculum by rmesropy*

## Description

Codexion is a C concurrency simulation inspired by the dining-philosophers
problem. Several coder threads sit around a circular table and compete for
shared USB dongles. A compile requires both adjacent dongles at the same time.
The simulation adds a mandatory dongle cooldown, FIFO or EDF arbitration, a
custom priority queue, precise burnout monitoring, and serialized logging.

The program stops when one coder burns out or when every coder has completed
at least the required number of compiles.

## Instructions

Build the project with:

```sh
make
```

The resulting executable is `codexion`.

Run it with exactly eight arguments:

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
    time_to_debug time_to_refactor number_of_compiles_required \
    dongle_cooldown scheduler
```

The scheduler must be exactly `fifo` or `edf`. Numeric arguments are
non-negative integers, except that `number_of_coders` must be at least `1`.

Examples:

```sh
./codexion 5 800 200 100 100 5 100 fifo
./codexion 5 800 200 100 100 5 100 edf
```

Other Makefile targets:

```sh
make clean
make fclean
make re
```

## Blocking cases handled

**Deadlock prevention.** A coder is granted both adjacent dongles in one
atomic operation under `state_lock`, or receives neither. A waiting coder
holds no dongle, so the hold-and-wait condition required for the classical
circular-wait deadlock cannot occur.

**Scheduler fairness.** Pending requests are stored in a custom binary heap.
FIFO uses a monotonic request sequence number so requests that occur inside
the same millisecond keep their true arrival order. EDF orders requests by
their burnout deadline and uses coder id as the deterministic tie-breaker.
A lower-priority request cannot take a dongle needed by an earlier pending
request that shares that dongle.

**Cooldown.** Every released dongle records its release time and is unavailable
until `dongle_cooldown` milliseconds have elapsed. A separate `released` flag
avoids relying on artificial negative timestamps and prevents integer
overflow at extreme cooldown values.

**Burnout detection.** Each coder's first compile deadline is measured from
the beginning of the simulation. After a compile starts, its
`last_compile_start_ms` becomes the new reference point. A dedicated monitor
thread checks burnout periodically and logs the first detected burnout before
stopping the simulation.

**Shutdown.** Stopping the simulation clears pending requests and broadcasts
`state_cond`, waking coders that are waiting for a grant. No new request is
granted after the stop flag is set. Threads are joined before mutexes,
condition variables, the priority queue, and heap allocations are destroyed.

**Log serialization.** Every state-change line is protected by `log_lock` and
flushed immediately. This prevents two threads from interleaving their output
lines.

## Thread synchronization mechanisms

| Primitive | Protected state | Purpose |
|---|---|---|
| `pthread_mutex_t log_lock` | stdout | Serializes one complete log line. |
| `pthread_mutex_t state_lock` | dongles, request heap, coder state, stop state | Provides one atomic arbitration point for shared simulation state. |
| `pthread_cond_t state_cond` | waiting coder threads | Wakes coders after grants and wakes all waiting coders during shutdown. |

No separate custom event object is used; the condition variable is the
thread-to-thread wake-up mechanism in this implementation.

The per-dongle `pthread_mutex_t` fields are initialized and destroyed as part
of dongle lifetime management, while `state_lock` is the mutex that protects
shared dongle state and guarantees that a pair can be checked and granted
atomically.

A concrete race prevented by `state_lock` is the monitor reading
`last_compile_start_ms` while a coder updates it. The same lock protects
`compiles_done`, the request heap, `granted`, `held`, `released_at_ms`, and
`stopped`.

## Resources

The project uses the following references while working with POSIX threads and
timing:

- POSIX `pthread_create`, `pthread_join`, mutexes and condition variables:
  <https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html>
- Linux `pthread_mutex_lock` documentation:
  <https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html>
- Linux `pthread_cond_wait` documentation:
  <https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html>
- Linux `gettimeofday` documentation:
  <https://man7.org/linux/man-pages/man2/gettimeofday.2.html>

### AI usage

AI was used as a review and debugging aid during development. It helped
inspect the parser, concurrency design, scheduler, monitor, cleanup paths,
and README requirements; identify race-condition, shutdown, fairness, timing,
and integer-boundary issues; and propose focused code changes.

The final implementation was manually reviewed and tested after those
changes. In particular, the project was rebuilt with `-Wall -Wextra -Werror
-pthread`, exercised with normal and edge-case scenarios, and checked with
AddressSanitizer and ThreadSanitizer. AI-assisted code was kept only after its
logic could be explained and verified against the project requirements.
