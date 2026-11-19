#ifndef GAME_MAIN
#define GAME_MAIN

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../../BIN_TREE/HEADERS/Tree_Common.h"
#include "../../BIN_TREE/HEADERS/Tree_Init_Ops.h"
#include "../../BIN_TREE/HEADERS/Tree_Fill_Ops.h"
#include "../../BIN_TREE/HEADERS/Tree_Print_Ops.h"

// DEFINES ----------------------------------------------------------

#define TEST_FILE "AKINATOR_GAME/CONSOLE/celebrities_test.txt"

#define MY_END_CUSTOM          "\033[0m"
#define MY_BLUE_AND_CURSIVE    "\033[3;34m"
#define MY_RED_AND_CURSIVE     "\033[3;31m"
#define MY_RED_AND_FADED       "\033[2;31m"
#define MY_GREEN_AND_CURSIVE   "\033[3;32m"
#define MY_PURPLE_AND_CURSIVE  "\033[3;35m"
#define MY_UNDERLINE           "\033[4m"
#define MY_CYAN_AND_CURSIVE    "\033[3;36m"
#define MY_CURSIVE             "\033[3m"
#define MY_RED_AND_BOLD        "\033[1;31m"
#define MY_PINK                "\033[35m"

// CONSTS -------------------------------------------------------------

const int TIME_TO_SLEEP = 10;
const int MAX_LEN_STR   = 64;

enum game_error_t
{
    GAME_SUCCESS = 0,
    GAME_ERROR   = 1
};

enum game_error_codes
{
    GAME_OK         = 0,
    GAME_INPUT_END  = 1<<1
};

// PROTOTYPES ----------------------------------------------------------

void sleep_print(const char *string);
void hello_and_menu();
void define_mode();
void difference_mode();

game_error_t reading_user_answer(tree_t* tree, bool* answer);
game_error_t choose_mode(tree_t* tree);
game_error_t guess_mode(tree_t* tree);
game_error_t leave_game(tree_t* tree);
game_error_t print_questions(tree_t* tree, node_t* node);
game_error_t add_new_character(tree_t* tree, node_t* node);
game_error_t add_text_question(tree_t* tree, char* new_question, node_t* node, char* new_character);
game_error_t add_character_name(tree_t* tree, char* new_character);


game_error_t main_init_and_fill_tree(tree_t* tree);
game_error_t debug_func(tree_t* tree);

#endif
