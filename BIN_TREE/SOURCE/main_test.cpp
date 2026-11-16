#include "C:\Users\MilanaGlav\.vscode\Akinator\Akinator\BIN_TREE\HEADERS\Tree_Init_Ops.h"
#include "C:\Users\MilanaGlav\.vscode\Akinator\Akinator\BIN_TREE\HEADERS\Tree_Fill_Ops.h"
#include "C:\Users\MilanaGlav\.vscode\Akinator\Akinator\BIN_TREE\HEADERS\Tree_Print_Ops.h"
#include "C:\Users\MilanaGlav\.vscode\Akinator\Akinator\BIN_TREE\HEADERS\Tree_Dest_Ops.h"

int main()
{
    FILE* file_tree = fopen(INPUT_TEST_FILE, "r");
    if (file_tree == NULL) return -1;

    FILE* out_file_tree = fopen(OUTPUT_TEST_FILE, "w");
    tree_t tree = {};

    tree_init(&tree);
    fill_tree(file_tree, &tree);

    print_tree(out_file_tree, &tree, print_str);
    
    FILE* html = fopen(HTML_FILE, "w");
    TREE_DUMP(html, &tree);
    fclose(html);

    tree_destroy(&tree, destroy_string_value);
    
    fclose(file_tree);
    fclose(out_file_tree);
}