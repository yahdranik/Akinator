#include "../HEADERS/Tree_Init_Ops.h"

tree_error_t tree_init(tree_t* tree)
{
    if (tree == NULL) return TREE_ERROR;

    #ifdef DEBUG
    tree->error = TREE_OK;
    #endif

    tree->root = NULL;

    return TREE_SUCCESS;
}

tree_error_t node_create(tree_t* tree, node_t** node, void* value)
{
    if (tree == NULL) return TREE_ERROR;

    if (node == NULL )
    {
        #ifdef DEBUG 
        tree->error |= TREE_NODE_NULL_ADRESS;
        #endif 

        return TREE_ERROR;
    }

    *node  = (node_t*) calloc(1, sizeof(node_t));
    
    if ((*node) == NULL)
    {
        #ifdef DEBUG
        tree->error |= TREE_NOT_MEMORY;
        #endif 

        return TREE_ERROR;
    }

    tree_error_t res_init_node = node_init(tree, *node, value);

    if (res_init_node == TREE_ERROR) return TREE_ERROR;

    return TREE_SUCCESS;
}

tree_error_t node_init(tree_t* tree, node_t* node, void* value)
{
    if (tree == NULL) return TREE_ERROR;
    if (node == NULL)
    {
        #ifdef DEBUG
        tree->error |= TREE_NODE_NULL_ADRESS;
        #endif

        return TREE_ERROR;
    }

    node->value = value;
    node->left  = NULL;
    node->right = NULL;

    return TREE_SUCCESS;
}

tree_error_t node_destroy(tree_t* tree, node_t** node, tree_destroy_node_value destroyer)
{
    if (tree == NULL) return TREE_ERROR;
    if (node == NULL)
    {
        #ifdef DEBUG
        tree->error |= TREE_NODE_NULL_ADRESS;
        #endif

        return TREE_ERROR;
    }

    if (destroyer == NULL)
    {
        #ifdef DEBUG
        tree->error |= TREE_NULL_FUNC_DEST;
        #endif
        return TREE_ERROR;
    }

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

    if (res_left_destroy == TREE_ERROR || res_right_destroy == TREE_ERROR)
    {
        return TREE_ERROR;
    }

    return TREE_SUCCESS;
}

tree_error_t destroy_string_value(void* value)
{
    if (value == NULL) 
    {
        return TREE_ERROR;
    }

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

    if (res_root_dest == TREE_ERROR) return TREE_ERROR;

    return TREE_SUCCESS;
}