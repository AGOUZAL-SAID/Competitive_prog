#ifndef FUTURES_H
#define FUTURES_H
#include <pthread.h>
#include <stdbool.h>

typedef struct {
  void *result;
  bool completed;
} future_t;

future_t *future_create();
void future_complete(future_t *future, void *result);
void *future_get(future_t *future);
void future_destroy(future_t *future);
#endif
