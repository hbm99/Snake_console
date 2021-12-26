
#include "queue.h"
#include "coordinate.h"
#include <stdio.h>
#include <stdlib.h>

//Sets a point (inside a new created node) at the end of the structure (queue insertion)
void enqueue(node_t **head, point val) {
    node_t *new_node = malloc(sizeof(node_t));
    if (!new_node)
        return;
    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}

//Sets a node at the end of the structure (queue insertion)
void node_enqueue(node_t **head, node_t * node) {
    node->next = *head;
    *head = node;
}

//Removes a node and returns its point, who is at the start of the structure (queue remove) (FIFO)
point dequeue(node_t **head) {
    node_t *current, *prev = NULL;
    point retval;
    retval.x = -1;
    retval.y = -1;
    if (*head == NULL) {
        return retval;
    }
    current = *head;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    retval = current->val;
    if (prev)
        prev->next = NULL;
    else
        *head = NULL;
    return retval;
}

//Removes and returns a node, who is at the start of the structure (queue remove) (FIFO)
node_t* node_dequeue(node_t **head) {
    node_t *current;
    node_t *prev = NULL;
    node_t *retnode = malloc(sizeof(node_t));
    if (!retnode) {
        return retnode;
    }
    retnode->val.x = -1;
    retnode->val.y = -1;
    if (*head == NULL) {
        return retnode;
    }
    current = *head;
    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }
    retnode = current;
    if (prev)
        prev->next = NULL;
    else
        *head = NULL;
    return retnode;
}
