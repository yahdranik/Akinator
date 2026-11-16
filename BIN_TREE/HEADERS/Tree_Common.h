#ifndef TREE_COMMON
#define TREE_COMMON

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define DEBUG

const size_t MAX_FILE_SIZE = 1024;

typedef uint32_t error_flags_t;
typedef void* elem_t;

struct node_t
{
    elem_t value;
    node_t* left;
    node_t* right;
};

struct tree_t
{
    node_t* root;

    #ifdef DEBUG
    error_flags_t error;
    #endif
};

enum tree_error_t
{
    TREE_SUCCESS = 0,
    TREE_ERROR   = 1
};

enum runtime_tree_error_codes
{
    TREE_OK                  = 0,
    TREE_NOT_MEMORY          = 1<<1,
    TREE_NODE_NULL_ADRESS    = 1<<2,
    TREE_INCORRECT_DEEP      = 1<<3,
    TREE_NULL_FUNC_DEST      = 1<<4,
    NULL_READ_FILE           = 1<<5,
    TREE_NULL_VALUE_ADRESS   = 1<<6,
    TREE_FILE_NOT_OPEN       = 1<<7,
    TREE_UNVALIBLE_FILE_SIZE = 1<<8,
    TREE_FILE_READ_FAILING   = 1<<9
};

enum syntax_tree_error_codes
{
    TREE_INCORRECT_NODE    = 1<<10,
    FILE_TREE_SYNTAX_ERROR = 1<<11
};

#endif