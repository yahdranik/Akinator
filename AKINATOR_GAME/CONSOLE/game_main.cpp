#include "akinator.h"

int main()
{
    tree_t tree = {};

    game_error_t res = main_init_and_fill_tree(&tree);

    hello_and_menu();
    game_error_t res_choose = choose_mode(&tree);
    if (res_choose == GAME_ERROR) return -1;

    game_error_t res_debug = debug_func(&tree);
    if (res_debug == GAME_ERROR) return -1;

    tree_destroy(&tree, destroy_string_value);


    return 0;
}

// SUPPORTING FUNCS -----------------------------------------------------

game_error_t main_init_and_fill_tree(tree_t* tree)
{
    FILE* file_tree = fopen(TEST_FILE, "r");

    if (file_tree == NULL) 
    {
        printf("file do not open\n");
        return GAME_ERROR;
    }

    tree_error_t res_init = tree_init(tree);

    if (res_init == TREE_ERROR)
    {
        printf("Initialise is failed. Check logs\n");
        return GAME_ERROR;
    }

    tree_error_t res_fill = fill_tree(file_tree, tree, TEST_FILE);
    if (res_fill == TREE_ERROR)
    {
        printf("Filling is failed. Check logs\n");
        return GAME_ERROR;
    }

    fclose(file_tree);

    return GAME_SUCCESS;
}

game_error_t debug_func(tree_t* tree)
{
    FILE* out_file = fopen(TEST_FILE, "w");

    if (out_file == NULL) 
    {
        printf("file do not open\n");
        return GAME_ERROR;
    }
    
    print_tree(out_file, tree, print_str);
    fclose(out_file);
    
    FILE* html = fopen(HTML_FILE, "w");

    if (html == NULL) 
    {
        printf("file do not open\n");
        return GAME_ERROR;
    }

    TREE_DUMP(html, tree);
    fclose(html);

    return GAME_SUCCESS;
}