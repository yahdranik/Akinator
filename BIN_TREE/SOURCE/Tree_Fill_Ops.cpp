#include "../HEADERS/Tree_Fill_Ops.h"

ssize_t get_file_size(const char* NAME)
{
    struct stat file_info = {};
    stat(NAME, &file_info);
    ssize_t size_of_file = file_info.st_size;

    if (size_of_file == -1 || size_of_file == 0)
    {
        perror("Size unvalible\n");
        return UNVALIBLE_FILE_SIZE;
    }

    return size_of_file;
}

tree_error_t read_from_file(FILE* file, tree_t* tree, char** buffer, const char* file_name)
{
    if (tree == NULL) return TREE_ERROR;

    if (file == NULL)
    {
        #ifdef DEBUG 
        tree->error |= TREE_FILE_NOT_OPEN;
        #endif 

        return TREE_ERROR;
    }

    ssize_t file_size = get_file_size(file_name);

    if (file_size == UNVALIBLE_FILE_SIZE)
    {
        #ifdef DEBUG 
        tree->error |= TREE_UNVALIBLE_FILE_SIZE;
        #endif 

        return TREE_ERROR;
    }

    *buffer = (char*) calloc((size_t) file_size + 1, sizeof(char));

    if (buffer == NULL)
    {
        #ifdef DEBUG
        tree->error |= TREE_NOT_MEMORY;
        #endif 

        return TREE_ERROR;
    }

    size_t elem_reads = fread(*buffer, sizeof(char), (size_t) file_size, file);

    if (elem_reads == 0)
    {
        #ifdef DEBUG
        tree->error |= TREE_FILE_READ_FAILING;
        #endif 

        free(buffer);
        return TREE_ERROR;
    }

    return TREE_SUCCESS;
}

tree_error_t fill_tree(FILE* file, tree_t* tree, const char* file_name)
{
    if (tree == NULL) return TREE_ERROR;
    if (file == NULL)
    {
        #ifdef DEBUG
        tree->error |= TREE_FILE_NOT_OPEN;
        #endif

        return TREE_ERROR;
    }

    char* buffer = {};
    tree_error_t res_of_read = read_from_file(file, tree, &buffer, file_name);

    if (res_of_read == TREE_ERROR)
    {
        return TREE_ERROR;
    }

    char* position = (char*) buffer;

    tree_error_t res_of_fill = read_node(tree, &tree->root, &position);

    if (res_of_fill == TREE_SUCCESS)
    {
        skip_spaces(&position);

        if (*position != '\0')
        {
            #ifdef DEBUG
            tree->error |= FILE_TREE_SYNTAX_ERROR;
            #endif

            res_of_fill = TREE_ERROR;
        }
    }

    FREE_MASS(buffer);

    if (res_of_fill == TREE_ERROR)
    {
        tree_destroy(tree, destroy_string_value);
        return TREE_ERROR;
    }

    return TREE_SUCCESS;
}

void skip_spaces(char** position)
{
    while (**position == ' ' || **position == '\n' || **position == '\t' || **position == '\r')
    {
        (*position)++;
    }
}

tree_error_t expect_char(tree_t* tree, char** position, char expected)
{
    skip_spaces(position);

    if (**position != expected)
    {
        #ifdef DEBUG
        tree->error |= FILE_TREE_SYNTAX_ERROR;
        #endif

        return TREE_ERROR;
    }

    (*position)++;
    return TREE_SUCCESS;
}

tree_error_t read_string(tree_t* tree, char** position, char** read_str)
{
    if (expect_char(tree, position, '"') == TREE_ERROR)
        return TREE_ERROR;

    char* end = strchr(*position, '"');
    
    if (end == NULL || end == *position) 
    {
        #ifdef DEBUG
        tree->error |= FILE_TREE_SYNTAX_ERROR;
        #endif 

        return TREE_ERROR;
    }

    size_t len = (size_t)(end - *position);

    char* str = (char*) calloc(len + 1, sizeof(char));

    if (str == NULL)
    {
        #ifdef DEBUG
        tree->error |= TREE_NOT_MEMORY;
         #endif

        return TREE_ERROR;
    }

    memcpy(str, *position, len);
    *position = end + 1;
    *read_str = str;

    return TREE_SUCCESS;
}

tree_error_t read_node(tree_t* tree, node_t** node, char** position)
{
    if (tree == NULL || node == NULL || position == NULL || *position == NULL)
    {
        return TREE_ERROR;
    }

    if (expect_char(tree, position, '(') == TREE_ERROR)
    {
        return TREE_ERROR;
    }

    char* str = NULL;
    if (read_string(tree, position, &str) == TREE_ERROR)
        return TREE_ERROR;

    if (node_create(tree, node, str) == TREE_ERROR)
    {
        free(str);    
        return TREE_ERROR;
    }

    skip_spaces(position);

    if (**position == ')')
    {
        (*position)++;
        return TREE_SUCCESS;
    }

    tree_error_t res_read_left = read_node(tree, &(*node)->left,  position);

    if (res_read_left == TREE_ERROR)
    {
        return TREE_ERROR;
    }

    tree_error_t res_read_right = read_node(tree, &(*node)->right, position);

    if (res_read_right == TREE_ERROR)
    {
        return TREE_ERROR;
    }

    tree_error_t res_exp_last_symbol = expect_char(tree, position, ')');

    return res_exp_last_symbol;
}