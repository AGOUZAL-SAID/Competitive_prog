#include "task.h"

#include <stdlib.h>

#include "utils.h"

task_t *tasks;

task_t *task_create(main_function_t main, void *arg, future_t *future,
                    int ms_offset, int ms_period, int ms_delay) {
  task_t *task = malloc(sizeof(task_t));
  task->main = main;
  task->arg = arg;
  task->future = future;
  task->ms_period = ms_period;
  task->ms_delay = ms_delay;
  clock_gettime(CLOCK_REALTIME, &task->release);
  add_millis_to_timespec(&task->release, ms_offset);
  return task;
}

void task_destroy(task_t *task) { /* free(task) */ ; }
