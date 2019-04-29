#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node* node_t;

struct node {
  void *data;
  node_t next;
};

struct queue {
	node_t head;
  node_t tail;
  int length;
};

queue_t queue_create(void)
{
  // allocate the queue
  queue_t q = malloc(sizeof(struct queue));

  if(q == NULL) {
    return NULL;
  }

  // initialize the queue values
  q->head = NULL;
  q->tail = NULL;
  q->length = 0;

  // return address of the created queue
  return q;
}

int queue_destroy(queue_t queue)
{
  // first check to see if the pointer is already pointing to NULL, ,
  // or if the queue is not empty
	if(queue == NULL || queue->length > 0) {
    return -1;
  } else {
    free(queue);
    // check that the memory was freed successfully
    if(queue == NULL) {
      return 0;
    }
    return -1;
  }
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}
/* TODO: delete after queue is working */
void queue_print(queue_t queue) {

}


int main() {
  return 0;
}
