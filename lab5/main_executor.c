#include "executor.h"
#include "scenario.h"
#include "task.h"
#include "utils.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

future_t **futures;

void *task_main(void *arg) {
  int exec_time = *(int *)arg;
  struct timespec ts1, ts2;

  ts1.tv_sec = exec_time / 1000;
  ts1.tv_nsec = (exec_time % 1000) * 1000000;

  mtxprintf(ex_debug, "initiate execution=%ld\n", exec_time);
  nanosleep(&ts1, &ts2);
  mtxprintf(ex_debug, "complete execution=%ld\n", exec_time);
  return NULL;
}

int main(int argc, char *argv[]) {
  int i;

  if (argc != 2) {
    printf("Usage : %s <scenario file>\n", argv[0]);
    exit(1);
  }
  ex_debug = 1; // Debug Executor
  pt_debug = 1; // Debug Pool Thread
  pb_debug = 0; // Debug Protected Buffer

  /* Read the configuration parameters of the scenario.
   */
  readFile(argv[1]);
  utils_init(max_pool_size);

  futures = (future_t **)malloc(sizeof(future_t *) * task_table_size);

  set_start_time();

  blocking_queue_t *blocking_queue =
      blocking_queue_init(0, blocking_queue_max_size);
  thread_pool_t *thread_pool = thread_pool_create(
      core_pool_size, max_pool_size, keep_alive_time, blocking_queue);
  executor_t *executor = executor_create(thread_pool);

  for (i = 0; i < task_table_size; i++) {
    tasks[i].arg = (void *)&tasks[i].exec_time;
    tasks[i].main = task_main;
    // mtxprintf(ex_debug, "task %d exec %d, period %d delay %d\n", i,
    //           tasks[i].exec_time, tasks[i].ms_period, tasks[i].ms_delay);

    /* Submit task to executor.
     */
    mtxprintf(ex_debug, "submit task of id %d\n", i);
    futures[i] = NULL;
    if (tasks[i].ms_period) {
      executor_schedule_at_fixed_rate(executor, tasks[i].main, tasks[i].arg, 0,
                                      tasks[i].ms_period);
    } else if (tasks[i].ms_delay) {
      executor_schedule_with_fixed_delay(executor, tasks[i].main, tasks[i].arg,
                                         0, tasks[i].ms_delay);
    } else {
      futures[i] = executor_submit_task(executor, tasks[i].main, tasks[i].arg);
      if (futures[i] == NULL)
        mtxprintf(ex_debug, "report task failure for identifier %d\n", i);
    }
  }
  thread_pool_start_threads(executor->thread_pool);
  for (i = 0; i < task_table_size; i++) {
    if (!tasks[i].ms_period && !tasks[i].ms_delay)
      if (futures[i] != NULL) {
        /* Get result from future associated to task. Suspend until result
         * becomes available.
         */
        future_get(futures[i]);
        mtxprintf(ex_debug, "get task result of id %d\n", i);
      }
  }
  sleep(10);
  executor_shutdown(executor);
}
