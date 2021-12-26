
#ifndef node_h
#define node_h

#include <stdio.h>
#include "coordinate.h"

//Node struct declaration
typedef struct node {
    point val;
    struct node *next;
    struct node *parent;
} node_t;

#endif /* node_h */
