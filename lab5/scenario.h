#include "blocking_queue.h"
#include "task.h"

typedef unsigned long ulong;

extern long task_table_size;
extern long core_pool_size;
extern long max_pool_size;
extern long blocking_queue_max_size;
extern long keep_alive_time;

#ifdef DEPS
extern bool **deps;
#endif

void readFile(char *filename);
