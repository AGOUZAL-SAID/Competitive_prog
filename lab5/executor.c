#include "executor.h"
#include "future.h"
#include "task.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

int ex_debug = 0;

executor_t *executor_create(thread_pool_t *thread_pool) {
  executor_t *executor;
  executor = (executor_t *)malloc(sizeof(executor_t));
  executor->thread_pool = thread_pool;
  return executor;
}

/* Submit task to the thread pool. */
future_t *executor_submit_task(executor_t *executor, main_function_t main,
                               void *arg) {
  future_t *future = future_create();
  task_t *task = task_create(main, arg, future, 0, 0, 0);
  if (thread_pool_execute(executor->thread_pool, task)) {
    return future;
  }
  future_destroy(future);
  return NULL;
}
bool executor_schedule_with_fixed_delay(executor_t *executor,
                                        main_function_t main, void *arg,
                                        int ms_offset, int ms_delay) {
  bool success = false;
  return success;
};

typedef struct {
    executor_t *executor;
    main_function_t main;
    void *arg;
    int ms_period;
    struct timespec next_release;
} periodic_task_info_t;

void *periodic_task_wrapper(void *info_ptr) {
    periodic_task_info_t *info = (periodic_task_info_t *)info_ptr;
    delay_until(&info->next_release);
    info->main(info->arg);
    add_millis_to_timespec(&info->next_release, info->ms_period);

    if (!info->executor->thread_pool->shutdown) {
        periodic_task_info_t *next_info = malloc(sizeof(periodic_task_info_t));
        *next_info = *info;
        task_t *task = task_create(periodic_task_wrapper, next_info, NULL, 0, 0, 0);
        thread_pool_execute(info->executor->thread_pool, task);
    }
    free(info);
    return NULL;
}

bool executor_schedule_at_fixed_rate(executor_t *executor, main_function_t main,
                                     void *arg, int ms_offset, int ms_period) {
    if (ms_period <= 0) return false;
    periodic_task_info_t *info = malloc(sizeof(periodic_task_info_t));
    info->executor = executor;
    info->main = main;
    info->arg = arg;
    info->ms_period = ms_period;
    clock_gettime(CLOCK_REALTIME, &info->next_release);
    add_millis_to_timespec(&info->next_release, ms_offset);

    task_t *task = task_create(periodic_task_wrapper, info, NULL, 0, 0, 0);
    return thread_pool_execute(executor->thread_pool, task);
}

void executor_shutdown(executor_t *executor) {
  mtxprintf(ex_debug, "executor shutdown activated\n");
  thread_pool_shutdown(executor->thread_pool);
  mtxprintf(ex_debug, "executor shutdown terminated\n");
}
