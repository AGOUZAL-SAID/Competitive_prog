
#ifndef TASKS_H
#define TASKS_H

#include "future.h"

typedef void *(*main_function_t)(void *);

typedef struct _task_t {
  main_function_t main;
  void *arg;
  future_t *future;
  int exec_time;
  int ms_period;
  int ms_delay;
  struct timespec release;
} task_t;
extern task_t *tasks;

task_t *task_create(main_function_t main, void *arg, future_t *future,
                    int ms_offset, int ms_period, int ms_delay);
void task_destroy(task_t *task);
#endif
