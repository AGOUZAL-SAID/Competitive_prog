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

bool executor_schedule_at_fixed_rate(executor_t *executor, main_function_t main,
                                     void *arg, int ms_offset, int ms_period) {
  bool success = false;
  return success;
};

void executor_shutdown(executor_t *executor) {
  mtxprintf(ex_debug, "executor shutdown activated\n");
  thread_pool_shutdown(executor->thread_pool);
  mtxprintf(ex_debug, "executor shutdown terminated\n");
}
