#include "scenario.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long task_table_size;
long core_pool_size;
long max_pool_size;
long blocking_queue_max_size;
long keep_alive_time;
long period;

int getString(FILE *f, char *s, char *file, int line) {
  char b[64];
  char *c;

  while (fgets(b, 64, f) != NULL) {
    c = strchr(b, '\n');
    *c = '\0';
    if (strcmp(s, b) == 0)
      return 1;
  }
  printf("getString failed to catch %s in %s:%d\n", s, file, line);
  exit(1);
  return 0;
}

int getLong(FILE *f, long *l, char *file, int line) {
  char b[64];
  char *c;
  ;

  if (fgets(b, 64, f) != NULL) {
    c = strchr(b, '\n');
    *c = '\0';
    *l = strtol(b, NULL, 10);
    if ((*l != 0) || (errno != EINVAL))
      return 1;
  }
  return 0;
}

void readFile(char *filename) {
  FILE *file;
  ulong i;

  file = fopen(filename, "r");
  if (file == NULL) {
    printf("cannot read file %s\n", filename);
    exit(1);
  }

  getString(file, "#core_pool_size", __FILE__, __LINE__);
  getLong(file, (long *)&core_pool_size, __FILE__, __LINE__);
  printf("core_pool_size = %ld\n", core_pool_size);

  getString(file, "#max_pool_size", __FILE__, __LINE__);
  getLong(file, (long *)&max_pool_size, __FILE__, __LINE__);
  printf("max_pool_size = %ld\n", max_pool_size);

  getString(file, "#blocking_queue_max_size", __FILE__, __LINE__);
  getLong(file, (long *)&blocking_queue_max_size, __FILE__, __LINE__);
  printf("blocking_queue_max_size = %ld\n", blocking_queue_max_size);

  getString(file, "#keep_alive_time", __FILE__, __LINE__);
  getLong(file, (long *)&keep_alive_time, __FILE__, __LINE__);
  printf("keep_alive_time = %ld\n", keep_alive_time);

  getString(file, "#task_table_size", __FILE__, __LINE__);
  getLong(file, (long *)&task_table_size, __FILE__, __LINE__);

  getString(file, "#exec_time", __FILE__, __LINE__);
  tasks = (task_t *)malloc((ulong)(task_table_size) * sizeof(task_t));
  for (i = 0; i < task_table_size; i++) {
    getLong(file, (long *)&tasks[i].exec_time, __FILE__, __LINE__);
    // printf ("exec_time[%lu]=%ld\n", i, jobs[i].exec_time);
  }

  getString(file, "#period", __FILE__, __LINE__);
  for (i = 0; i < task_table_size; i++) {
    getLong(file, (long *)&tasks[i].ms_period, __FILE__, __LINE__);
    // printf ("exec_time[%lu]=%ld\n", i, jobs[i].exec_time);
  }

  getString(file, "#delay", __FILE__, __LINE__);
  for (i = 0; i < task_table_size; i++) {
    getLong(file, (long *)&tasks[i].ms_delay, __FILE__, __LINE__);
    // printf ("exec_time[%lu]=%ld\n", i, jobs[i].exec_time);
  }
}
