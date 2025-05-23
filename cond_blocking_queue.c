#include "cond_blocking_queue.h"
#include "bounded_buffer.h"
#include "utils.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Initialise the blocking queue structure above.
blocking_queue_t *cond_blocking_queue_init(int length) {
  blocking_queue_t *b;
  b = (blocking_queue_t *)malloc(sizeof(blocking_queue_t));
  b->buffer = bounded_buffer_init(length);
  // Initialize the synchronization components
  pthread_mutex_init(&b->mutex, NULL);
  pthread_cond_init(&b->not_empty, NULL);
  pthread_cond_init(&b->not_full, NULL);
  b->is_sem_impl = false; // condition variable 
  return b;
}

// Extract an element from buffer. If the attempted operation is
// not possible immedidately, the method call blocks until it is.
void *cond_blocking_queue_get(blocking_queue_t *b) {
  void *d;

  // Enter mutual exclusion

  // Wait until there is a full slot to get data from the unprotected
  // bounded buffer (bounded_buffer_get).

  // Signal or broadcast that an empty slot is available in the
  // unprotected bounded buffer (if needed)
  pthread_mutex_lock(&b->mutex);
  while (b->buffer->size == 0) {
    pthread_cond_wait(&b->not_empty, &b->mutex);
  }
  d = bounded_buffer_get(b->buffer);
  pthread_cond_broadcast(&b->not_full);
  if (d == NULL)
  mtxprintf(pb_debug, "get (B) - data=NULL\n");
  else
  mtxprintf(pb_debug, "get (B) - data=%d\n", *(int *)d);
  
  pthread_mutex_unlock(&b->mutex);
  // Leave mutual exclusion

  return d;
}

// Insert an element into buffer. If the attempted operation is
// not possible immedidately, the method call blocks until it is.
void cond_blocking_queue_put(blocking_queue_t *b, void *d) {

  // Enter mutual exclusion

  // Wait until there is an empty slot to put data in the unprotected
  // bounded buffer (bounded_buffer_put).

  // Signal or broadcast that a full slot is available in the
  // unprotected bounded buffer (if needed)
  pthread_mutex_lock(&b->mutex);
  while (b->buffer->size == b->buffer->max_size) {
    pthread_cond_wait(&b->not_full, &b->mutex);
  }
  bounded_buffer_put(b->buffer, d);
  pthread_cond_broadcast(&b->not_empty);
  if (d == NULL)
    mtxprintf(pb_debug, "put (B) - data=NULL\n");
  else
    mtxprintf(pb_debug, "put (B) - data=%d\n", *(int *)d);
  pthread_mutex_unlock(&b->mutex);
  // Leave mutual exclusion
}

// Extract an element from buffer. If the attempted operation is not
// possible immedidately, return NULL. Otherwise, return the element.
void *cond_blocking_queue_remove(blocking_queue_t *b) {
  void *d;

  // Enter mutual exclusion

  // Signal or broadcast that an empty slot is available in the
  // unprotected bounded buffer (if needed)
  pthread_mutex_lock(&b->mutex);
  pthread_cond_broadcast(&b->not_full);
  d = bounded_buffer_get(b->buffer);
  if (d == NULL)
    mtxprintf(pb_debug, "remove (I)) - data=NULL\n");
  else
    mtxprintf(pb_debug, "remove (I)) - data=%d\n", *(int *)d);
  pthread_mutex_unlock(&b->mutex);
  // Leave mutual exclusion
  return d;
}

// Insert an element into buffer. If the attempted operation is
// not possible immedidately, return 0. Otherwise, return 1.
int cond_blocking_queue_add(blocking_queue_t *b, void *d) {
  int done;

  // Enter mutual exclusion

  // Signal or broadcast that a full slot is available in the
  // unprotected bounded buffer (if needed)
  pthread_mutex_lock(&b->mutex);
  pthread_cond_broadcast(&b->not_empty);
  done = bounded_buffer_put(b->buffer, d);
  if (!done)
    d = NULL;

  if (d == NULL)
    mtxprintf(pb_debug, "add (I)) - data=NULL\n");
  else
    mtxprintf(pb_debug, "add (I)) - data=%d\n", *(int *)d);
  pthread_mutex_unlock(&b->mutex);
  // Leave mutual exclusion
  return done;
}

// Extract an element from buffer. If the attempted operation is not
// possible immedidately, the method call blocks until it is, but
// waits no longer than the given timeout. Return the element if
// successful. Otherwise, return NULL.
void *cond_blocking_queue_poll(blocking_queue_t *b, struct timespec *abstime) {
  void *d = NULL;
  int rc = 0;

  // Enter mutual exclusion

  // Wait until there is a full slot to get data from the unprotected
  // bounded buffer  (bounded_buffer_get) but waits no longer than
  // the given timeout.

  // Signal or broadcast that an empty slot is available in the
  // unprotected bounded buffer (if needed)

  d = bounded_buffer_get(b->buffer);
  if (d == NULL)
    mtxprintf(pb_debug, "poll (T) - data=NULL\n");
  else
    mtxprintf(pb_debug, "poll (T) - data=%d\n", *(int *)d);

  // Leave mutual exclusion

  return d;
}

// Insert an element into buffer. If the attempted operation is not
// possible immedidately, the method call blocks until it is, but
// waits no longer than the given timeout. Return 0 if not
// successful. Otherwise, return 1.
int cond_blocking_queue_offer(blocking_queue_t *b, void *d,
                              struct timespec *abstime) {
  int rc = 0;
  int done = 0;

  // Enter mutual exclusion

  // Wait until there is an empty slot to put data in the unprotected
  // bounded buffer (bounded_buffer_put) but waits no longer than
  // the given timeout.

  // Signal or broadcast that a full slot is available in the
  // unprotected bounded buffer (if needed)

  done = bounded_buffer_put(b->buffer, d);
  if (!done)
    d = NULL;

  if (d == NULL)
    mtxprintf(pb_debug, "offer (T) - data=NULL\n");
  else
    mtxprintf(pb_debug, "offer (T) - data=%d\n", *(int *)d);

  // Leave mutual exclusion
  return done;
}
