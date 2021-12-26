
#ifndef queue_h
#define queue_h

#include <stdio.h>
#include "node.h"

void enqueue(node_t **head, point val);

void node_enqueue(node_t **head, node_t * node);

point dequeue(node_t **head);

node_t* node_dequeue(node_t **head);

#endif /* queue_h */
