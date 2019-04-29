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

queue_t queue_create(void) {
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

int queue_destroy(queue_t queue) {
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


/*
 * queue_enqueue - Enqueue data item
 * @queue: Queue in which to enqueue item
 * @data: Address of data item to enqueue
 *
 * Enqueue the address contained in @data in the queue @queue.
 *
 * Return: -1 if @queue or @data are NULL, or in case of memory allocation error
 * when enqueing. 0 if @data was successfully enqueued in @queue.
 */
int queue_enqueue(queue_t queue, void *data)
{
  if(queue == NULL || data == NULL) {
    return -1;
  }

  // create a node holding the data
  node_t node = malloc(sizeof(struct node));

  // check that malloc worked
  if(node == NULL) {
    return -1;
  }
  node->data = data;
  node->next = NULL;

  // if an initialized queue doesnt have any elements
  // add to the front of the queue
  if(queue->length == 0) {
    queue->head = node;
    queue->tail = node;
  } else if (queue->length > 0) {
    // if the queue does have something in it, then
    node_t previous = queue->tail;
    previous->next = node;
    queue->tail = node;
  } else {
    // something went wrong
    return -1;
  }

  // after adding a node, increment the queue length
  (queue->length)++;

  return 0;
}


/*
 * queue_dequeue - Dequeue data item
 * @queue: Queue in which to dequeue item
 * @data: Address of data pointer where item is received
 *
 * Remove the oldest item of queue @queue and assign this item (the value of a
 * pointer) to @data.
 *
 * Return: -1 if @queue or @data are NULL, or if the queue is empty. 0 if @data
 * was set with the oldest item available in @queue.
 */
  // NOTE: dequeue = POP off of queue
int queue_dequeue(queue_t queue, void **data) {

  if(queue == NULL) {
    return -1;
  }

  node_t node = queue->head;

  if(node->data == NULL || queue->length == 0) {
    return -1;
  }
  node_t next = node->next; // the node that will be the new head

  // update the queue head and the length of the queue
  queue->head = next;
  (queue->length)--;

  data = node->data;
  free(data); // free the pointer once the data is set

  return 0;
}

int queue_delete(queue_t queue, void *data) {
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data) {
	/* TODO Phase 1 */
}

int queue_length(queue_t queue) {
	/* TODO Phase 1 */
}
/* TODO: delete after queue is working */
void queue_print(queue_t queue) {

}


int main() {
  return 0;
}
