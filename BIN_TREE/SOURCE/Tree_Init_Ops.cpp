#include "HEADERS/Tree_Init_Ops.h"

tree_error_t tree_init(tree_t* tree)
{
    #ifdef DEBUG
    if (tree == NULL) return TREE_ERROR;
    tree->error = TREE_OK;
    #endif

    tree->root = NULL;

    return TREE_SUCCESS;
}

tree_error_t node_create(tree_t* tree, node_t** node, void* value)
{
    #ifdef DEBUG 
    if (tree == NULL) return TREE_ERROR;

    if (node == NULL )
    {
        tree->error |= TREE_NODE_NULL_ADRESS;
        return TREE_ERROR;
    }
    #endif

    *node  = (node_t*) calloc(1, sizeof(node_t));
    
    #ifdef DEBUG
    if ((*node) == NULL)
    {
        tree->error |= TREE_NOT_MEMORY;
        return TREE_ERROR;
    }
    #endif

    tree_error_t res_init_node = node_init(tree, *node, value);

    #ifdef DEBUG
    if (res_init_node == TREE_ERROR) return TREE_ERROR;
    #endif

    return TREE_SUCCESS;
}

tree_error_t node_init(tree_t* tree, node_t* node, void* value)
{
    #ifdef DEBUG
    if (tree == NULL) return TREE_ERROR;
    if (node == NULL)
    {
        tree->error |= TREE_NODE_NULL_ADRESS;
        return TREE_ERROR;
    }
    #endif

    node->value = value;
    node->left  = NULL;
    node->right = NULL;

    return TREE_SUCCESS;
}