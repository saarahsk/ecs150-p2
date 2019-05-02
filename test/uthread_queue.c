#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "queue.h"

// the testing of the queue uses some of the code from the
// sample queue unit testing
/* Callback function that increments items by a certain value */
static int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;

    return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;

    if (*a == match)
        return 1;

    return 0;
}


// function that will create a queue that we can use to run other
// tests
queue_t create_queue()
{
  queue_t q = queue_create();
  int data[] = {1, 2, 3, 4};
  for (int i = 0; i < sizeof(data) / sizeof(data[0]); i++)
      queue_enqueue(q, &data[i]);

return q;
}

// this test will create a queue, add some data to it,
// and make sure that the length of the queue is right
void test_enqueue()
{
  queue_t q = create_queue();
  assert(queue_length(q) == 4);
}


void test_dequeue_noqueue()
{
  queue_t q = NULL;
  int *ptr;
  assert(queue_dequeue(q, (void **)&ptr) == -1);
}

void test_iterate() {
  queue_t q;
  int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  int i;
  int *ptr;

  /* Initialize the queue and enqueue items */
  q = queue_create();
  for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
      queue_enqueue(q, &data[i]);

  /* Add value '1' to every item of the queue */
  queue_iterate(q, inc_item, (void*)1, NULL);
  assert(data[0] == 2);

  /* Find and get the item which is equal to value '5' */
  ptr = NULL;
  queue_iterate(q, find_item, (void*)5, (void**)&ptr);
  assert(ptr != NULL);
  assert(*ptr == 5);
  assert(ptr == &data[3]);
}


int main()
{
    test_enqueue();
    test_iterate();
    test_dequeue_noqueue();

    printf("All queue tests passed\n");
    return 0;

}
