#include "future.h"
#include <stdlib.h>

future_t *future_create() {
  future_t *future = malloc(sizeof(future_t));
  future->result = NULL;
  future->completed = false;
  pthread_mutex_init(&future->mutex, NULL);
  pthread_cond_init(&future->cond, NULL);
  return future;
}

void future_complete(future_t *future, void *result) {
  pthread_mutex_lock(&future->mutex);
  future->result = result;
  future->completed = true;
  pthread_cond_broadcast(&future->cond);
  pthread_mutex_unlock(&future->mutex);
}

void *future_get(future_t *future) {
  pthread_mutex_lock(&future->mutex);
  while (!future->completed) {
      pthread_cond_wait(&future->cond, &future->mutex);
  }
  void *result = future->result;
  pthread_mutex_unlock(&future->mutex);
  return result;
}

void future_destroy(future_t *future) {
  pthread_mutex_destroy(&future->mutex);
  pthread_cond_destroy(&future->cond);
  free(future);
}
