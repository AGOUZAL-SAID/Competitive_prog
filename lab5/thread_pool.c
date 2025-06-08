#define _GNU_SOURCE
#include "thread_pool.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int pt_debug = 0;
int id = 1;
typedef struct {
  thread_pool_t *thread_pool;
  int pool_thread_id;
} pool_thread_arg_t;

task_t *shutdown_task;
bool is_shutdown_task(task_t *task) { return (task->main == NULL); }

pool_thread_arg_t *pool_thread_arg_create(thread_pool_t *thread_pool,
                                          int pool_thread_id) {
  pool_thread_arg_t *arg =
      (pool_thread_arg_t *)malloc(sizeof(pool_thread_arg_t));
  arg->thread_pool = thread_pool;
  arg->pool_thread_id = pool_thread_id;
  return arg;
}

thread_pool_t *thread_pool_create(int core_pool_size, int max_pool_size,
                                  long keep_alive_time,
                                  blocking_queue_t *blocking_queue) {
  thread_pool_t *thread_pool;
  thread_pool = (thread_pool_t *)malloc(sizeof(thread_pool_t));
  thread_pool->core_pool_size = core_pool_size;
  thread_pool->max_pool_size = max_pool_size;
  thread_pool->pool_size = 0;
  thread_pool->idle_threads = 0;
  thread_pool->keep_alive_time = keep_alive_time;
  thread_pool->blocking_queue = blocking_queue;
  thread_pool->shutdown = 0;
  pthread_mutex_init(&thread_pool->mutex, NULL);      
  pthread_cond_init(&thread_pool->condition, NULL);   
  shutdown_task = task_create(NULL, NULL, NULL, 0, 0, 0);
  thread_pool->threads = malloc(sizeof(pthread_t) * max_pool_size);
  return thread_pool;
}

void *pool_thread_main(void *arg) {
  thread_pool_t *thread_pool = ((pool_thread_arg_t *)arg)->thread_pool;
  int pool_thread_id = ((pool_thread_arg_t *)arg)->pool_thread_id;
  blocking_queue_t *blocking_queue = thread_pool->blocking_queue;

  bool is_core = (pool_thread_id <= thread_pool->core_pool_size);
  set_current_thread_id(&pool_thread_id, is_core ? "core" : "temp");

  task_t *task;
  void *result;

  mtxprintf(pt_debug, "thread created\n");
  while (true) {
      pthread_mutex_lock(&thread_pool->mutex);
      thread_pool->idle_threads++;
      pthread_mutex_unlock(&thread_pool->mutex);

      if (!is_core && thread_pool->keep_alive_time != FOREVER) {
          struct timespec abstime;
          clock_gettime(CLOCK_REALTIME, &abstime);
          add_millis_to_timespec(&abstime, thread_pool->keep_alive_time);
          task = blocking_queue_poll(blocking_queue, &abstime);
      } else {

          task = blocking_queue_get(blocking_queue);
      }
      pthread_mutex_lock(&thread_pool->mutex);
      thread_pool->idle_threads--;
      pthread_mutex_unlock(&thread_pool->mutex);

      if (!is_core && thread_pool->keep_alive_time != FOREVER && task == NULL) {
          pthread_mutex_lock(&thread_pool->mutex);
          thread_pool->pool_size--;
          pthread_mutex_unlock(&thread_pool->mutex);
          break;
      }
      if (task == NULL || is_shutdown_task(task)) {
          pthread_mutex_lock(&thread_pool->mutex);
          thread_pool->pool_size--;
          pthread_mutex_unlock(&thread_pool->mutex);
          break;
      }

        result = task->main(task->arg);
        future_complete(task->future, result);
        task_destroy(task);
    }

    mtxprintf(pt_debug, "thread terminated\n");
    pthread_mutex_lock(&thread_pool->mutex);
        
    mtxprintf(pt_debug, "alive threads %d, idle threads %d\n",
            thread_pool->pool_size, thread_pool->idle_threads);
    pthread_mutex_unlock(&thread_pool->mutex);
    return NULL;
}

bool thread_pool_execute(thread_pool_t *thread_pool, task_t *task) {
    pthread_mutex_lock(&thread_pool->mutex);
    if (thread_pool->shutdown) {
        pthread_mutex_unlock(&thread_pool->mutex);
        return false; 
    }
    if (thread_pool->pool_size < thread_pool->core_pool_size) {
        pool_thread_arg_t *arg = pool_thread_arg_create(thread_pool, thread_pool->pool_size + 1); // core id: 1, 2, ...
        pthread_t thread;
        pthread_create(&thread, NULL, pool_thread_main, (void *)arg);
        thread_pool->threads[thread_pool->pool_size] = thread;
        thread_pool->pool_size++;
        pthread_mutex_unlock(&thread_pool->mutex);
        blocking_queue_put(thread_pool->blocking_queue, (void *)task);
        return true;
    } else {
        pthread_mutex_unlock(&thread_pool->mutex);
        if (blocking_queue_add(thread_pool->blocking_queue, (void *)task)) {
            return true;
        } else {
            pthread_mutex_lock(&thread_pool->mutex);
            if (thread_pool->pool_size < thread_pool->max_pool_size) {
                pool_thread_arg_t *arg = pool_thread_arg_create(thread_pool, thread_pool->pool_size + 1); // temp id
                pthread_t thread;
                pthread_create(&thread, NULL, pool_thread_main, (void *)arg);
                thread_pool->threads[thread_pool->pool_size] = thread;
                thread_pool->pool_size++;
                pthread_mutex_unlock(&thread_pool->mutex);
                blocking_queue_put(thread_pool->blocking_queue, (void *)task);
                return true;
            }
            pthread_mutex_unlock(&thread_pool->mutex);
            return false; 
        } 
    }
}

void thread_pool_shutdown(thread_pool_t *thread_pool) {
    pthread_mutex_lock(&thread_pool->mutex);
    thread_pool->shutdown = 1; 
    int n_threads = thread_pool->pool_size;
    pthread_mutex_unlock(&thread_pool->mutex);


    for (int i = 0; i < n_threads; ++i) {
        blocking_queue_put(thread_pool->blocking_queue, (void *)shutdown_task);
    }

    for (int i = 0; i < n_threads; ++i) {
        pthread_join(thread_pool->threads[i], NULL);

    }
    
}
