#ifndef TREE_FILL_OPS
#define TREE_FILL_OPS

#include "Tree_Common.h"
#include "Tree_Init_Ops.h"
#include "oper_with_file.h"
#include "oper_with_str.h"

#define INPUT_TEST_FILE "init_tree.txt"

tree_error_t read_from_file(FILE* file, tree_t* tree, char** buffer, const char* file_name);
tree_error_t fill_tree(FILE* file, tree_t* tree, const char* file_name);

void skip_spaces(char** position);
tree_error_t expect_char(tree_t* tree, char** position, char expected);
tree_error_t read_string(tree_t* tree, char** position, char** out);
tree_error_t read_node(tree_t* tree, node_t** node, char** pos);

#endif