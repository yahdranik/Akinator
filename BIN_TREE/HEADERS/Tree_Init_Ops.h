#ifndef TREE_INIT_OPS
#define TREE_INIT_OPS

#include "Tree_Common.h"

#define TREE_INIT_VALUE NULL


// PROTOTYPES -------------------------------------------------

tree_error_t tree_init(tree_t* tree);
tree_error_t node_create(tree_t* tree, node_t** node, void* value);
tree_error_t node_init(tree_t* tree, node_t* node, void* value);

#endif