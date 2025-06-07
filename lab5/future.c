#include "future.h"
#include <stdlib.h>

future_t *future_create() {
  future_t *future = malloc(sizeof(future_t));
  future->result = NULL;
  future->completed = false;
  return future;
}

void future_complete(future_t *future, void *result) {}

void *future_get(future_t *future) {
  void *result = NULL;
  return result;
}

void future_destroy(future_t *future) {}
