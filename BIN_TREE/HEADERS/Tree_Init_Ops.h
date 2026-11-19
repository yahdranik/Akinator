#ifndef TREE_INIT_OPS
#define TREE_INIT_OPS

#include "Tree_Common.h"

#define TREE_INIT_VALUE NULL


// PROTOTYPES -------------------------------------------------

tree_error_t tree_init(tree_t* tree);
tree_error_t node_create(tree_t* tree, node_t** node, void* value);
tree_error_t node_init(tree_t* tree, node_t* node, void* value);


typedef tree_error_t (*tree_destroy_node_value)(void* value);
tree_error_t tree_destroy(tree_t* tree, tree_destroy_node_value destroyer);
tree_error_t node_destroy(tree_t* tree, node_t** node, tree_destroy_node_value destroyer);
tree_error_t destroy_string_value(void* value);

#endif