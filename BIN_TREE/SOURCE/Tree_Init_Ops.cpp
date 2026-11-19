#include "../HEADERS/Tree_Init_Ops.h"

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

tree_error_t node_destroy(tree_t* tree, node_t** node, tree_destroy_node_value destroyer)
{
    #ifdef DEBUG
    if (tree == NULL) return TREE_ERROR;
    if (node == NULL)
    {
        tree->error |= TREE_NODE_NULL_ADRESS;
        return TREE_ERROR;
    }

    if (destroyer == NULL)
    {
        tree->error |= TREE_NULL_FUNC_DEST;
        return TREE_ERROR;
    }
    #endif

    if (*node ==  NULL)
    {
        return TREE_SUCCESS;
    }

    tree_error_t res_left_destroy = node_destroy(tree, &(*node)->left, destroyer);
    tree_error_t res_right_destroy = node_destroy(tree, &(*node)->right, destroyer);

    destroyer((*node)->value); // обработку
    (*node)->value = NULL;
    free(*node);
    *node = NULL;

    #ifdef DEBUG 
    if (res_left_destroy == TREE_ERROR || res_right_destroy == TREE_ERROR)
    {
        return TREE_ERROR;
    }
    #endif

    return TREE_SUCCESS;
}

tree_error_t destroy_string_value(void* value)
{
    #ifdef DEBUG
    if (value == NULL) 
    {
        return TREE_ERROR;
    }
    #endif

    char* buf = (char*) value;
    memset(buf, 0, strlen(buf));
    free(value);

    return TREE_SUCCESS;
}

tree_error_t tree_destroy(tree_t* tree, tree_destroy_node_value destroyer)
{
    if (tree == NULL)
        return TREE_ERROR;

    tree_error_t res_root_dest = node_destroy(tree, &tree->root, destroyer);

    #ifdef DEBUG
    if (res_root_dest == TREE_ERROR) return TREE_ERROR;
    #endif 

    return TREE_SUCCESS;
}