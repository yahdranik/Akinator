#ifndef TREE_PRINT_OPS
#define TREE_PRINT_OPS

#include "Tree_Common.h"

#define OUTPUT_TEST_FILE "BIN_TREE/out_tree.txt"
#define LOG_FILE "BIN_TREE/tree_logs.log"
#define DOT_FILE "BIN_TREE/tree_graphiz.dot"
#define HTML_FILE "BIN_TREE/graph_tree_dump.html"

#define TREE_DUMP(html, tree)  tree_dump(html, tree, __FILE__, __FUNCTION__, __LINE__)
#define PRINT_TREE_ERROR(code) fprintf(log, "\nCode: %d\n", code)
#define CHECK_TREE_ERROR(error, checked) ((error & checked) == checked)

typedef tree_error_t (*tree_print_node_value)(FILE* file, const void* value);

tree_error_t print_tree(FILE* file, tree_t* tree, tree_print_node_value printer);
tree_error_t print_node(FILE* file, tree_t* tree, const node_t* node, size_t deep, tree_print_node_value printer);
tree_error_t print_str(FILE* file, const void* value);

void tree_dump(FILE* html, const tree_t* tree, const char* file_name, const char* func_name, int line);
void create_tree_dot_file(FILE* dot, const tree_t* tree);
void create_nodes_dot_file(FILE* dot, const node_t* node);
void print_nodes_into_logs(FILE* log, node_t* node);

#endif