#ifndef TREE_DEST_OPS
#define TREE_DEST_OPS

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Tree_Common.h"

typedef tree_error_t (*tree_destroy_node_value)(void* value);

tree_error_t tree_destroy(tree_t* tree, tree_destroy_node_value destroyer);
tree_error_t node_destroy(tree_t* tree, node_t** node, tree_destroy_node_value destroyer);

#endif