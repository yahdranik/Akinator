#include "akinator.h"

void sleep_print(const char *string)
{
    size_t length_of_string = strlen(string);
    for (size_t i = 0; i < length_of_string; i++)
    {
        printf("%c", string[i]);
        fflush(stdout);
        usleep(TIME_TO_SLEEP * 1000);
    }
}

void hello_and_menu()
{
    sleep_print(MY_CYAN_AND_CURSIVE "Hello! I am Akinator and I can guess any character you have in mind\n"
                "Please, select game mode:\n\n" MY_END_CUSTOM);

    sleep_print("===== MODES: =====\n"
                MY_CYAN_AND_CURSIVE "GUESS" MY_END_CUSTOM MY_CURSIVE" - The main game mode, in which I try to guess your character by asking questions.\n"
                "If I fail to guess correctly, you can add that character.\n" MY_END_CUSTOM
                MY_CYAN_AND_CURSIVE "DEF" MY_END_CUSTOM MY_CURSIVE" - Identification mode. Type a character's name, and I'll tell you who it is.\n" MY_END_CUSTOM
                MY_CYAN_AND_CURSIVE "DIF" MY_END_CUSTOM MY_CURSIVE " - Comparison mode. Name two characters,"
                "and I will tell you how they are similar and how they differ.\n" MY_END_CUSTOM
                MY_CYAN_AND_CURSIVE "LEAVE" MY_END_CUSTOM MY_CURSIVE " - Leave the game\n");
}

game_error_t choose_mode(tree_t* tree)
{
    char chosen_mode[MAX_LEN_STR] = {};
    bool is_mode_select = 0;
    
    while (is_mode_select == 0)
    {
        if (scanf("%63s", chosen_mode) != 1)  
        {
            #ifdef DEBUG
            tree->error |= GAME_INPUT_END;
            #endif

            return GAME_ERROR;
        }

        if (strcmp(chosen_mode, "GUESS") == 0)
        {
            game_error_t res_guess = guess_mode(tree);
            if (res_guess == GAME_ERROR) return GAME_ERROR;

            is_mode_select = 1;
        }

        // else if (strcmp(chosen_mode, "DEF") == 0)
        // {
        //     define_mode();
        //     is_mode_select = 1;
        // }

        // else if (strcmp(chosen_mode, "DIF") == 0)
        // {
        //     difference_mode();
        //     is_mode_select = 1;
        // }

        else if(strcmp(chosen_mode, "LEAVE") == 0)
        {
            game_error_t res_leave = leave_game(tree);
            if (res_leave == GAME_ERROR) return GAME_ERROR;

            is_mode_select = 1;
        }

        else
        {
            sleep_print(MY_RED_AND_CURSIVE "Sorry, unknown mode\n" MY_END_CUSTOM);
        }
    }

    return GAME_SUCCESS;
}

game_error_t leave_game(tree_t* tree)
{
    if (tree == NULL)
    {
        return GAME_ERROR;
    }
    
    sleep_print(MY_CURSIVE "Nu vse idi otsuda. Game closed\n" MY_CURSIVE);
    tree_error_t res_dest = tree_destroy(tree, destroy_string_value);

    #ifdef DEBUG
    if (res_dest == TREE_ERROR)
    {
        return GAME_ERROR;
    }
    #endif

    return GAME_SUCCESS;
}

// game_error_t define_mode(tree_t* tree)
// {
//     char user_input[MAX_LEN_STR] = {};
//     bool is_user_chosen_character = 0;

//     while (is_user_chosen_character == 0)
//     {
//         if (scanf("%s", user_input) == 1)
//         {
//             is_user_chosen_character = 1;
//             game_error_t res_find = find_node(tree, user_input);
//         }
//     }
//     return GAME_SUCCESS;
// }

// game_error_t find_node()
// {

// }

// void difference_mode()
// {

// }

game_error_t print_questions(tree_t* tree, node_t* node)
{
    #ifdef DEBUG
    if (tree == NULL) return GAME_ERROR;

    if (node == NULL)
    {
        tree->error |= TREE_NODE_NULL_ADRESS;
        return GAME_ERROR;
    }
    #endif

    bool user_answer = false;

    if (node->left == NULL && node->right == NULL)
    {
        sleep_print("Is it " MY_UNDERLINE);
        sleep_print((char*) node->value);
        sleep_print(MY_END_CUSTOM "?\n");

        game_error_t res_reading = reading_user_answer(tree, &user_answer);

        #ifdef DEBUG 
        if (res_reading == GAME_ERROR) return GAME_ERROR;
        #endif

        if (user_answer)
        {
            sleep_print(MY_GREEN_AND_CURSIVE "I knew it!\n" MY_END_CUSTOM);
            return GAME_SUCCESS;
        }

        return add_new_character(tree, node);
    }

    sleep_print((char*) node->value);
    sleep_print("?\n");

    game_error_t res_reading = reading_user_answer(tree, &user_answer);

    #ifdef DEBUG 
    if (res_reading == GAME_ERROR) return GAME_ERROR;
    #endif

    if (user_answer)
    {
        return print_questions(tree, node->left);                   // NOTE: Это ветки yes 
    }

    return print_questions(tree, node->right);                      // NOTE: Ветки no
}

game_error_t guess_mode(tree_t* tree)
{
    #ifdef DEBUG
    if (tree == NULL) return GAME_ERROR;
    #endif

    node_t* current_node = tree->root;
    return print_questions(tree, current_node);
}

game_error_t add_character_name(tree_t* tree, char* new_character)
{
    sleep_print(MY_CURSIVE "What character did you think of? for example: cat\n" MY_END_CUSTOM);

    if (scanf(" %63[^\n]", new_character) != 1)
    {
        #ifdef DEBUG
        tree->error |= GAME_INPUT_END;
        #endif

        return GAME_ERROR;
    }

    #ifdef DEBUG
    if (strchr(new_character, '"') != NULL)
    {
        sleep_print(MY_RED_AND_CURSIVE "Name must be without quotes\n" MY_END_CUSTOM);
        return GAME_ERROR;
    }
    #endif

    return GAME_SUCCESS;
}

game_error_t add_text_question(tree_t* tree, char* new_question, node_t* node, char* new_character)
{
    sleep_print(MY_CURSIVE "Write a question, which is true for " MY_END_CUSTOM MY_UNDERLINE);
    sleep_print(new_character);
    sleep_print(MY_END_CUSTOM MY_CURSIVE" and false for " MY_END_CUSTOM MY_UNDERLINE);
    sleep_print((char*) node->value);
    sleep_print(MY_END_CUSTOM MY_CURSIVE", for example: an animal\n" MY_END_CUSTOM);

    if (scanf(" %63[^\n]", new_question) != 1)
    {
        #ifdef DEBUG
        tree->error |= GAME_INPUT_END;
        #endif

        return GAME_ERROR;
    }

    #ifdef DEBUG
    if (strchr(new_question, '"') != NULL)
    {
        sleep_print(MY_RED_AND_CURSIVE "Question must be without quotes\n" MY_END_CUSTOM);
        return GAME_ERROR;
    }
    #endif

    return GAME_SUCCESS;
}

game_error_t add_new_character(tree_t* tree, node_t* node)
{
    #ifdef DEBUG
    if (tree == NULL) return GAME_ERROR;

    if (node == NULL)
    {
        tree->error |= TREE_NODE_NULL_ADRESS;
        return GAME_ERROR;
    }
    #endif

    char new_character[MAX_LEN_STR] = {};
    char new_question[MAX_LEN_STR]  = {};

    game_error_t res_add_character = add_character_name(tree, new_character);
    game_error_t res_add_quest = add_text_question(tree, new_question, node, new_character);

    #ifdef DEBUG
    if (res_add_character == GAME_ERROR || res_add_quest == GAME_ERROR) return GAME_ERROR;
    #endif

    char* character_copy = strdup(new_character);
    char* question_copy  = strdup(new_question);

    #ifdef DEBUG
    if (character_copy == NULL || question_copy == NULL)
    {
        tree->error |= TREE_NOT_MEMORY;

        free(character_copy);
        free(question_copy);
        return GAME_ERROR;
    }
    #endif

    node_t* left_node  = {};
    node_t* right_node = {};

    tree_error_t res_create_left  = node_create(tree, &left_node,  character_copy);
    tree_error_t res_create_right = node_create(tree, &right_node, node->value);

    #ifdef DEBUG
    if (res_create_left == TREE_ERROR || res_create_right == TREE_ERROR)
    {
        free(character_copy);
        free(question_copy);
        free(left_node);
        free(right_node);
        return GAME_ERROR;
    }
    #endif

    node->value = question_copy;
    node->left  = left_node;
    node->right = right_node;

    sleep_print(MY_GREEN_AND_CURSIVE "Got it, I will remember\n" MY_END_CUSTOM);

    return GAME_SUCCESS;
}

game_error_t reading_user_answer(tree_t* tree, bool* answer)
{
    char user_answer[MAX_LEN_STR] = {};

    while (true)
    {
        if (scanf("%63s", user_answer) != 1)
        {
            #ifdef DEBUG
            tree->error |= GAME_INPUT_END;
            #endif

            return GAME_ERROR;
        }

        if (strcmp(user_answer, "yes") == 0 || strcmp(user_answer, "YES") == 0 || strcmp(user_answer, "y") == 0)
        {
            *answer = true;
            return GAME_SUCCESS;
        }

        else if (strcmp(user_answer, "no") == 0 || strcmp(user_answer, "NO") == 0 || strcmp(user_answer, "n") == 0)
        {
            *answer = false;
            return GAME_SUCCESS;
        }

        else
        {
            sleep_print(MY_RED_AND_CURSIVE "Please, write yes or no\n" MY_END_CUSTOM);
        }
    }
}

