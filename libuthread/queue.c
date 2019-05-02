#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

  if (q == NULL) {
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
  if (queue == NULL || queue->length > 0) {
    return -1;
  }

  // delete all nodes in the queue
  for (int i = 0; i < queue->length; i++) {
    node_t temp = queue->head;
    queue->head = queue->head->next;
    free(temp);
  }

  free(queue);
  return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
  if (queue == NULL || data == NULL) {
    return -1;
  }

  // create a node holding the data
  node_t node = malloc(sizeof(struct node));

  // check that malloc worked
  if (node == NULL) {
    return -1;
  }
  node->data = data;
  node->next = NULL;

  // if an initialized queue doesnt have any elements
  // add to the front of the queue
  if (queue->length == 0) {
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
  queue->length++;
  return 0;
}


int queue_dequeue(queue_t queue, void **data) {

  if (queue == NULL || data == NULL) {
    return -1;
  }

  if (queue->length == 0) {
    return -1;
  }

  node_t temp = queue->head;
  queue->head = queue->head->next; // update the queue head and the length of the queue

  *data = temp->data;
  queue->length--;
  free(temp); // free the extra queue data, not the user data

  return 0;
}

int queue_delete(queue_t queue, void *data) {
  if (queue == NULL || data == NULL) {
    return -1;
  }

  // start the search with the first element of the queue
  node_t previous = NULL;
  node_t current = queue->head;

  int result = -1;
  while (current != NULL) {
    if (current->data != data) {
      // move along the queue: previous should always be one behind current
      previous = current;
      current = current->next;
    }

    // found the data we are looking for
    if (queue->head == queue->tail) {
      // only one element in the queue so just delete it all
      queue->head = NULL;
      queue->tail = NULL;
    }
    else if (current == queue->head) {
      // data is at the front of the queue
      queue->head = current->next;
    }
    else if (current == queue->tail) {
      // data is at the end of the queue
      queue->tail = previous;
      queue->tail->next = NULL;
    }
    else {
      // general case: just make previous' next go to current's next
      previous->next = current->next;
      free(current);
    }

    queue->length--;
    free(current);
    result = 0;
    break;
  }

  return result;
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data) {

  node_t current = queue->head;
  // if the queue doesnt have anything in it, dont do anything and return -1
  if (current == NULL || func == NULL) {
    return -1;
  }

  while (current != NULL) {
    // perform the function on the queue element
    int funcVal = func(current->data, arg);

    if (funcVal == 1) {
      if (data != NULL) {
        *data = current->data;
      }
      break;
    }

    current = current->next;
  }

  return 0;
}

int queue_length(queue_t queue) {
  if (queue == NULL) {
    return -1;
  } else {
    return queue->length;
  }
}
