#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include "blocking_queue.h"
#include "task.h"
#include <pthread.h>

extern int pt_debug;

/* FOREVER stands for an infinite keep alive time. */
#define FOREVER -1

typedef struct {
  blocking_queue_t *blocking_queue;
  int core_pool_size;
  int max_pool_size;
  int pool_size;
  int idle_threads;
  long keep_alive_time;
  int shutdown;
  pthread_mutex_t mutex;
  pthread_cond_t condition;
  pthread_t *threads;
} thread_pool_t;

bool thread_pool_execute(thread_pool_t *thread_pool, task_t *task);

thread_pool_t *thread_pool_create(int core_pool_size, int max_pool_size,
                                  long keep_alive_time,
                                  blocking_queue_t *blocking_queue);

bool pool_thread_create(thread_pool_t *thread_pool, main_function_t main,
                        void *main_params, bool is_core);

void thread_pool_shutdown(thread_pool_t *thread_pool);
#endif
