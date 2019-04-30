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

  *data = node->data;
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


/*
 * queue_iterate - Iterate through a queue
 * @queue: Queue to iterate through
 * @func: Function to call on each queue item
 * @arg: (Optional) Extra argument to be passed to the callback function
 * @data: (Optional) Address of data pointer where an item can be received
 *
 * This function iterates through the items in the queue @queue and calls the
 * given callback function @func on each item. The callback function receives
 * the current data item and @arg.
 *
 * If @func returns 1 for a specific item, the iteration stops prematurely. In
 * this case only, if @data is different than NULL, then @data receives the data
 * item where the iteration was stopped.
 *
 * We assume that queue_delete() cannot be called inside @func on the current
 * data item. Doing so would result in undefined behavior.
 *
 * Return: -1 if @queue or @func are NULL, 0 otherwise.
 */

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data) {

  node_t current = queue->head;
  // if the queue doesnt have anything in it, dont do anything and return -1
  if(current == NULL || func == NULL) {
    return -1;
  }

  void *nodeData = current->data;
  while(current != NULL) {
    // perform the function on the queue element
    int funcVal = func(nodeData, arg);

    if(funcVal == 1) {
      break;
    }
    current = current->next;
    nodeData = current->data;
  }

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
