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


int queue_dequeue(queue_t queue, void **data) {

  if(queue == NULL || data == NULL) {
    return -1;
  }

  node_t node = queue->head;

  if(queue->length == 0) {
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

/*
 * queue_delete - Delete data item
 * @queue: Queue in which to delete item
 * @data: Data to delete
 *
 * Find in queue @queue, the first (ie oldest) item equal to @data and delete
 * this item.
 *
 * Return: -1 if @queue or @data are NULL, of if @data was not found in the
 * queue. 0 if @data was found and deleted from @queue.
 */
int queue_delete(queue_t queue, void *data) {
  if(queue == NULL || data == NULL) {
    return -1;
  }
  // start the search with the first element of the queue
  node_t node = queue->head;

  while(node != NULL) {
    // if the data is found
    // check that the next node exists before checking for data equality
    if(node->next != NULL) {
      // check next node's data with provided data
      if(node->next->data == data) {
        // if the data matches, delete next node
        node_t next = node->next->next; // this will be the node replacing next
        node_t freeNode = node->next; //this is the memory that must be freed
        node->next = next;
        free(freeNode);
      } else {
        // go to the next node
        node = node->next;
      }
    } else {
      // the next does not exist, therefore data is not found
      return -1;
    }
    return -1;
  }
  return -1; 
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data) {
	/* TODO Phase 1 */
  return 0;
}

int queue_length(queue_t queue) {
	if(queue == NULL) {
    return -1;
  } else {
    return queue->length;
  }
}

// testing the queue based on integers only
/* TODO: delete after queue is working */
void queue_print(queue_t queue) {

  node_t node = queue->head;
  if (node == NULL) {
    printf("The queue is empty");

  }

  int *val = node->data;
  while(node != NULL) {
    printf("%d", *val);
    node = node->next;
  }

}


int main() {


  return 0;
}
