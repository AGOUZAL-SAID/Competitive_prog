#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "future.h"
#include "bounded_buffer.h"
#include "task.h"
#include "thread_pool.h"
#include <pthread.h>

extern int ex_debug;

typedef struct _executor_t {
  thread_pool_t *thread_pool;
} executor_t;

/* Allocate and initialize executor. */
executor_t *executor_create(thread_pool_t *thread_pool);

future_t *executor_submit_task(executor_t *executor, main_function_t main,
                               void *arg);

bool executor_schedule_with_fixed_delay(executor_t *executor,
                                        main_function_t main, void *arg,
                                        int ms_offset, int ms_delay);

bool executor_schedule_at_fixed_rate(executor_t *executor, main_function_t main,
                                     void *arg, int ms_offset, int ms_period);

/* Wait for pool threads to be completed. */
void executor_shutdown(executor_t *executor);
#endif
