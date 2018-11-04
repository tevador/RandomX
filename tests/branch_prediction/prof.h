/*
 * Prof
 * ====
 *
 * Self-contained C/C++ profiler library for Linux.
 *
 * Prof offers a quick way to measure performance events (CPU clock cycles,
 * cache misses, branch mispredictions, etc.) of C/C++ code snippets. Prof is
 * just a wrapper around the `perf_event_open` system call, its main goal is to
 * be easy to setup and painless to use for targeted optimizations, namely, when
 * the hot spot has already been identified. In no way Prof is a replacement for
 * a fully-fledged profiler like perf, gprof, callgrind, etc.
 *
 * Please be aware that Prof uses `__attribute__((constructor))` to be as more
 * straightforward to setup as possible, so it cannot be included more than
 * once.
 *
 * Examples
 * --------
 *
 * ### Minimal
 *
 * The following snippet prints the rough number of CPU clock cycles spent in
 * executing the code between the two Prof calls:
 *
 * ```c
 * #include "prof.h"
 *
 * int main()
 * {
 *     PROF_START();
 *     // slow code goes here...
 *     PROF_STDOUT();
 * }
 * ```
 *
 * ### Custom options
 *
 * The following snippet instead counts both read and write faults of the level
 * 1 data cache that occur in the userland code between the two Prof calls:
 *
 * ```c
 * #include <stdio.h>
 *
 * #define PROF_USER_EVENTS_ONLY
 * #define PROF_EVENT_LIST \
 *     PROF_EVENT_CACHE(L1D, READ, MISS) \
 *     PROF_EVENT_CACHE(L1D, WRITE, MISS)
 * #include "prof.h"
 *
 * int main()
 * {
 *     uint64_t faults[2] = { 0 };
 *
 *     PROF_START();
 *     // slow code goes here...
 *     PROF_DO(faults[index] += counter);
 *
 *     // fast or uninteresting code goes here...
 *
 *     PROF_START();
 *     // slow code goes here...
 *     PROF_DO(faults[index] += counter);
 *
 *     printf("Total L1 faults: R = %lu; W = %lu\n", faults[0], faults[1]);
 * }
 * ```
 *
 * Installation
 * ------------
 *
 * Just include `prof.h`. Here is a quick way to fetch the latest version:
 *
 *     wget -q https://raw.githubusercontent.com/cyrus-and/prof/master/prof.h
 */
#ifndef PROF_H
#define PROF_H

#include <errno.h>
#include <linux/perf_event.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <unistd.h>

/*
 * API
 * ---
 */

/*
 * Reset the counters and (re)start counting the events.
 *
 * The events to be monitored are specified by setting the `PROF_EVENT_LIST`
 * macro before including this file to a list of `PROF_EVENT_*` invocations;
 * defaults to counting the number CPU clock cycles.
 *
 * If the `PROF_USER_EVENTS_ONLY` macro is defined before including this file
 * then kernel and hypervisor events are excluded from the count.
 */
#define PROF_START()                                                           \
    do {                                                                       \
        PROF_IOCTL_(ENABLE);                                                   \
        PROF_IOCTL_(RESET);                                                    \
    } while (0)

/*
 * Specify an event to be monitored, `type` and `config` are defined in the
 * documentation of the `perf_event_open` system call.
 */
#define PROF_EVENT(type, config)                                               \
    (uint32_t)(type), (uint64_t)(config),

/*
 * Same as `PROF_EVENT` but for hardware events; prefix `PERF_COUNT_HW_` must be
 * omitted from `config`.
 */
#define PROF_EVENT_HW(config)                                                  \
    PROF_EVENT(PERF_TYPE_HARDWARE, PERF_COUNT_HW_ ## config)

/*
 * Same as `PROF_EVENT` but for software events; prefix `PERF_COUNT_SW_` must be
 * omitted from `config`.
 */
#define PROF_EVENT_SW(config)                                                  \
    PROF_EVENT(PERF_TYPE_SOFTWARE, PERF_COUNT_SW_ ## config)

/*
 * Same as `PROF_EVENT` but for cache events; prefixes `PERF_COUNT_HW_CACHE_`,
 * `PERF_COUNT_HW_CACHE_OP_` and `PERF_COUNT_HW_CACHE_RESULT_` must be omitted
 * from `cache`, `op` and `result`, respectively. Again `cache`, `op` and
 * `result` are defined in the documentation of the `perf_event_open` system
 * call.
 */
#define PROF_EVENT_CACHE(cache, op, result)                                    \
    PROF_EVENT(PERF_TYPE_HW_CACHE,                                             \
               (PERF_COUNT_HW_CACHE_ ## cache) |                               \
               (PERF_COUNT_HW_CACHE_OP_ ## op << 8) |                          \
               (PERF_COUNT_HW_CACHE_RESULT_ ## result << 16))

/*
 * Stop counting the events. The counter array can then be accessed with
 * `PROF_COUNTERS`.
 */
#define PROF_STOP()                                                            \
    do {                                                                       \
        PROF_IOCTL_(DISABLE);                                                  \
        PROF_READ_COUNTERS_(prof_event_buf_);                                  \
    } while (0)

/*
 * Access the counter array. The order of counters is the same of the events
 * defined in `PROF_EVENT_LIST`. Elements of this array are 64 bit unsigned
 * integers.
 */
#define PROF_COUNTERS                                                          \
    (prof_event_buf_ + 1)

/*
 * Stop counting the events and execute the code provided by `block` for each
 * event. Within `code`: `index` refers to the event position index in the
 * counter array defined by `PROF_COUNTERS`; `counter` is the actual value of
 * the counter. `index` is a 64 bit unsigned integer.
 */
#define PROF_DO(block)                                                         \
    do {                                                                       \
        uint64_t i_;                                                           \
        PROF_STOP();                                                           \
        for (i_ = 0; i_ < prof_event_cnt_; i_++) {                             \
            uint64_t index = i_;                                               \
            uint64_t counter = prof_event_buf_[i_ + 1];                        \
            (void)index;                                                       \
            (void)counter;                                                     \
            block;                                                             \
        }                                                                      \
    } while (0)

/*
 * Same as `PROF_DO` except that `callback` is the name of a *callable* object
 * (e.g. a function) which, for each event, is be called with the two parameters
 * `index` and `counter`.
 */
#define PROF_CALL(callback)                                                    \
    PROF_DO(callback(index, counter))

/*
 * Stop counting the events and write to `file` (a stdio.h `FILE *`) as many
 * lines as are events in `PROF_EVENT_LIST`. Each line contains `index` and
 * `counter` (as defined by `PROF_DO`) separated by a tabulation character. If
 * there is only one event then `index` is omitted.
 */
#define PROF_FILE(file)                                                        \
    PROF_DO(if (prof_event_cnt_ > 1) {                                         \
            fprintf((file), "%lu\t%lu\n", index, counter);                     \
        } else {                                                               \
            fprintf((file), "%lu\n", counter);                                 \
        }                                                                      \
    )

/*
 * Same as `PROF_LOG_FILE` except that `file` is `stdout`.
 */
#define PROF_STDOUT()                                                          \
    PROF_FILE(stdout)

/*
 * Same as `PROF_LOG_FILE` except that `file` is `stderr`.
 */
#define PROF_STDERR()                                                          \
    PROF_FILE(stderr)

/* DEFAULTS ----------------------------------------------------------------- */

#ifndef PROF_EVENT_LIST
#ifdef PERF_COUNT_HW_REF_CPU_CYCLES /* since Linux 3.3 */
#define PROF_EVENT_LIST PROF_EVENT_HW(REF_CPU_CYCLES)
#else
#define PROF_EVENT_LIST PROF_EVENT_HW(CPU_CYCLES)
#endif
#endif

/* UTILITY ------------------------------------------------------------------ */

#define PROF_ASSERT_(x)                                                        \
    do {                                                                       \
        if (!(x)) {                                                            \
            fprintf(stderr, "# %s:%d: PROF error", __FILE__, __LINE__);        \
            if (errno) {                                                       \
                fprintf(stderr, " (%s)", strerror(errno));                     \
            }                                                                  \
            printf("\n");                                                      \
            abort();                                                           \
        }                                                                      \
    } while (0)

#define PROF_IOCTL_(mode)                                                      \
    do {                                                                       \
        PROF_ASSERT_(ioctl(prof_fd_,                                           \
                           PERF_EVENT_IOC_ ## mode,                            \
                           PERF_IOC_FLAG_GROUP) != -1);                        \
    } while (0)

#define PROF_READ_COUNTERS_(buffer)                                            \
    do {                                                                       \
        const ssize_t to_read = sizeof(uint64_t) * (prof_event_cnt_ + 1);      \
        PROF_ASSERT_(read(prof_fd_, buffer, to_read) == to_read);              \
    } while (0)

/* SETUP -------------------------------------------------------------------- */

static int prof_fd_;
static uint64_t prof_event_cnt_;
static uint64_t *prof_event_buf_;

static void prof_init_(uint64_t dummy, ...) {
    uint32_t type;
    va_list ap;

    prof_fd_ = -1;
    prof_event_cnt_ = 0;
    va_start(ap, dummy);
    while (type = va_arg(ap, uint32_t), type != (uint32_t)-1) {
        struct perf_event_attr pe;
        uint64_t config;
        int fd;

        config = va_arg(ap, uint64_t);

        memset(&pe, 0, sizeof(struct perf_event_attr));
        pe.size = sizeof(struct perf_event_attr);
        pe.read_format = PERF_FORMAT_GROUP;
        pe.type = type;
        pe.config = config;
        #ifdef PROF_USER_EVENTS_ONLY
        pe.exclude_kernel = 1;
        pe.exclude_hv = 1;
        #endif

        fd = syscall(__NR_perf_event_open, &pe, 0, -1, prof_fd_, 0);
        PROF_ASSERT_(fd != -1);
        if (prof_fd_ == -1) {
            prof_fd_ = fd;
        }

        prof_event_cnt_++;
    }
    va_end(ap);

    prof_event_buf_ = (uint64_t *)malloc((prof_event_cnt_ + 1) *
                                         sizeof(uint64_t));
}

void __attribute__((constructor)) prof_init()
{
    prof_init_(0, PROF_EVENT_LIST /*,*/ (uint32_t)-1);
}

void __attribute__((destructor)) prof_fini()
{
    PROF_ASSERT_(close(prof_fd_) != -1);
    free(prof_event_buf_);
}

#endif

/*
 * License
 * -------
 *
 * Copyright (c) 2017 Andrea Cardaci <cyrus.and@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
