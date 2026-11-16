#include "game_main.h"

int main()
{
    FILE* file_tree = fopen(INPUT_TEST_FILE, "r");
    if (file_tree == NULL) return -1;

    tree_t tree = {};

    tree_init(&tree);
    fill_tree(file_tree, &tree);

    hello_and_menu();
    choose_mode(tree);
    
    FILE* html = fopen(HTML_FILE, "w");
    TREE_DUMP(html, &tree);
    fclose(html);

    tree_destroy(&tree, destroy_string_value);
    
    fclose(file_tree);
}

void sleep_print(const char *string)
{
    int length_of_string = strlen(string);
    for (int i = 0; i < length_of_string; i++)
    {
        printf("%c", string[i]);
        Sleep(TIME_TO_SLEEP);
    }
}

void hello_and_menu()
{
    sleep_print("Hello! I am Akinator and I can guess any character you have in mind\n"
                "Please, select game mode:\n");

    sleep_print("===== MODE: =====\n"
                "GUESS - The main game mode, in which I try to guess your character by asking questions.\n"
                "If I fail to guess correctly, you can add that character.\n"
                "DEF - Identification mode. Type a character's name, and I'll tell you who it is.\n"
                "DIF - Comparison mode. Name two characters, and I will tell you how they are similar and how they differ.\n");
    
}

void choose_mode(tree_t tree)
{
    char chosen_mode[MAX_LEN_STR] = {};
    bool is_mode_select = 0;
    
    while (is_mode_select == 0)
    {
        if (scanf("%s", chosen_mode) != 1)
        {
            sleep_print("Sorry, incorrect game mode\n");
        }

        else 
        {
            if (strcmp(chosen_mode, "GUESS") == 1)
            {
                guess_mode(tree);
                is_mode_select = 1;
            }

            else if (strcmp(chosen_mode, "DEF") == 1)
            {
                define_mode();
                is_mode_select = 1;
            }

            else if (strcmp(chosen_mode, "DIF") == 1)
            {
                difference_mode();
                is_mode_select = 1;
            }

            else
            {
                printf("Sorry, unknown mode\n");
            }
        }
    }
}

game_error_t print_question(node_t* node)
{
    if 
}

game_error_t guess_mode(tree_t tree)
{
    node_t* current_node = tree.root;
    print_question(current_node);
}