#include "akinator.h"

int main()
{
    FILE* file_tree = fopen(TEST_FILE, "r");

    #ifdef DEBUG 
    if (file_tree == NULL) 
    {
        printf("file do not open\n");
        return -1;
    }
    #endif

    tree_t tree = {};

    tree_error_t res_init = tree_init(&tree);

    #ifdef DEBUG
    if (res_init == TREE_ERROR)
    {
        printf("Initialise is failed. Check logs\n");
        return -1;
    }
    #endif

    tree_error_t res_fill = fill_tree(file_tree, &tree, TEST_FILE);

    #ifdef DEBUG
    if (res_fill == TREE_ERROR)
    {
        printf("Filling is failed. Check logs\n");
        return -1;
    }
    #endif

    fclose(file_tree);

    hello_and_menu();
    choose_mode(&tree);

    FILE* out_file = fopen(TEST_FILE, "w");

    #ifdef DEBUG 
    if (out_file == NULL) 
    {
        printf("file do not open\n");
        return -1;
    }
    #endif

    print_tree(out_file, &tree, print_str);
    fclose(out_file);
    
    FILE* html = fopen(HTML_FILE, "w");

    #ifdef DEBUG 
    if (html == NULL) 
    {
        printf("file do not open\n");
        return -1;
    }
    #endif

    TREE_DUMP(html, &tree);
    fclose(html);

    tree_destroy(&tree, destroy_string_value);

    return 0;
}
