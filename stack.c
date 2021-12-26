
#include <stdlib.h>
#include "stack.h"

//Sets a point (inside a new created node) at the end of the structure (stack insertion)
void push(node_t ** head, point val) {
    node_t * new_node;
    new_node = (node_t *) malloc(sizeof(node_t));

    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}

//Removes a node and returns its point, who is at the end of the structure (stack remove) (LIFO)
point pop(node_t ** head) {
    point retval;
    retval.x = -1;
    retval.y = -1;
    node_t * next_node = NULL;

    if (*head == NULL) {
        return retval;
    }

    next_node = (*head)->next;
    retval = (*head)->val;
    free(*head);
    *head = next_node;

    return retval;
}
