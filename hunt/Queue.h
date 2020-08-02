// Interface for a queue of items

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct queue *Queue;

// Create a new queue
Queue QueueNew(void);

// Free all resources allocated for the queue
void QueueDrop(Queue q);

// Add an item to the end of the queue
void QueueEnqueue(Queue q, int item);

// Remove an element from the front of the queue and return it
int QueueDequeue(Queue q);

// Get the element at the front of the queue (without removing it)
int QueuePeek(Queue q);

// Get the number of elements in the queue
int QueueSize(Queue q);

// Check if the queue is empty
bool QueueIsEmpty(Queue q);

// Print the queue to an open file (for debugging)
void QueueDump(Queue q, FILE *fp);

#endif
