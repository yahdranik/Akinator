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

void hello()
{
    sleep_print(MY_CYAN_AND_CURSIVE "Hello! I am Akinator and I can guess any character you have in mind\n"
                "Please, select game mode:\n\n" MY_END_CUSTOM);
}

void menu()
{
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

        else if (strcmp(chosen_mode, "DEF") == 0)
        {
            game_error_t res_def = define_mode(tree);
            if (res_def == GAME_ERROR) return GAME_ERROR;

            is_mode_select = 1;
        }

        else if (strcmp(chosen_mode, "DIF") == 0)
        {
            game_error_t res_dif = difference_mode(tree);
            if (res_dif == GAME_ERROR) return GAME_ERROR;

            is_mode_select = 1;
        }

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

    return GAME_SUCCESS;
}

game_error_t define_mode(tree_t* tree)
{
    if (tree == NULL || tree->root == NULL)
    {
        return GAME_ERROR;
    }

    char character[MAX_LEN_STR] = {};

    sleep_print("Enter character name:\n");

    if (scanf(" %63[^\n]", character) != 1)
    {
        #ifdef DEBUG
        tree->error |= GAME_INPUT_END;
        #endif

        return GAME_ERROR;
    }

    path_step_t path[MAX_PATH_SIZE] = {};
    size_t path_size = 0;

    node_t* found_node = NULL;

    game_error_t res_find = find_node(tree->root, character, &found_node, path, &path_size);

    if (res_find == GAME_ERROR)
    {
        sleep_print("Sorry, I don't know this character.\n");
        return GAME_SUCCESS;
    }

    sleep_print("\nThis is what I know about ");
    sleep_print((const char*) found_node->value);
    sleep_print(":\n");

    for (size_t i = 0; i < path_size; i++)
    {
        printf("%s: %s\n", (char*) path[i].question_node->value, path[i].answer ? "YES" : "NO");
    }

    return GAME_SUCCESS;
}

game_error_t find_node(node_t* node, const char* value, node_t** found_node, path_step_t* path, size_t* path_size)
{
    if (node == NULL || value == NULL || found_node == NULL || path == NULL || path_size == NULL)
    {
        return GAME_ERROR;
    }

    if (node->left == NULL && node->right == NULL)
    {
        if (node->value == NULL) return GAME_ERROR;

        if (strcmp((char*) node->value, value) == 0)
        {
            *found_node = node;
            return GAME_SUCCESS;
        }

        return GAME_ERROR;
    }

    if (*path_size >= MAX_PATH_SIZE) return GAME_ERROR;

    size_t old_size = *path_size;

    path[*path_size].question_node = node;
    path[*path_size].answer = true;
    (*path_size)++;

    if (find_node(node->left, value, found_node, path, path_size) == GAME_SUCCESS)
        return GAME_SUCCESS;

    *path_size = old_size;

    path[*path_size].question_node = node;
    path[*path_size].answer = false;
    (*path_size)++;

    if (find_node(node->right, value, found_node, path, path_size) == GAME_SUCCESS)
        return GAME_SUCCESS;

    *path_size = old_size;

    return GAME_ERROR;
}

game_error_t difference_mode(tree_t* tree)
{
    if (tree == NULL || tree->root == NULL) 
    {
        return GAME_ERROR;
    }

    char first_character[MAX_LEN_STR] = {};
    char second_character[MAX_LEN_STR] = {};

    sleep_print(MY_CYAN_AND_CURSIVE "Enter first character:\n" MY_END_CUSTOM);

    if (scanf(" %63[^\n]", first_character) != 1)
    {
        return GAME_ERROR;
    }

    sleep_print(MY_CYAN_AND_CURSIVE "Enter second character:\n" MY_END_CUSTOM);

    if (scanf(" %63[^\n]", second_character) != 1)
    {
        return GAME_ERROR;
    }

    path_step_t first_path[MAX_PATH_SIZE] = {};
    path_step_t second_path[MAX_PATH_SIZE] = {};

    size_t first_size = 0;
    size_t second_size = 0;

    node_t* first_node = NULL;
    node_t* second_node = NULL;


    if (find_node(tree->root, first_character, &first_node, first_path, &first_size) == GAME_ERROR)
    {
        sleep_print(MY_RED_AND_CURSIVE "First character not found.\n" MY_END_CUSTOM);
        return GAME_SUCCESS;
    }

    if (find_node(tree->root, second_character, &second_node, second_path, &second_size) == GAME_ERROR)
    {
        sleep_print("Second character not found.\n");
        return GAME_SUCCESS;
    }

    size_t common_size = 0;

    while (common_size < first_size && common_size < second_size &&
           first_path[common_size].question_node == second_path[common_size].question_node &&
           first_path[common_size].answer == second_path[common_size].answer)
    {
        common_size++;
    }

    sleep_print(MY_PURPLE_AND_CURSIVE "\nDifferences:\n" MY_END_CUSTOM);

    printf("%s\n", (char*) first_path[common_size].question_node->value);

    sleep_print(first_character);
    sleep_print(": ");
    printf("%s\n", first_path[common_size].answer ? "YES" : "NO");

    sleep_print(second_character);
    sleep_print(": ");
    printf("%s\n",second_path[common_size].answer ? "YES" : "NO");

    return GAME_SUCCESS;
}

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

        if (res_reading == GAME_ERROR) return GAME_ERROR;

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

    if (res_reading == GAME_ERROR) return GAME_ERROR;

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
    if (tree == NULL) return GAME_ERROR;

    if (node == NULL)
    {
        #ifdef DEBUG
        tree->error |= TREE_NODE_NULL_ADRESS;
        #endif

        return GAME_ERROR;
    }

    char new_character[MAX_LEN_STR] = {};
    char new_question[MAX_LEN_STR]  = {};

    game_error_t res_add_character = add_character_name(tree, new_character);
    game_error_t res_add_quest = add_text_question(tree, new_question, node, new_character);

    #ifdef DEBUG
    if (res_add_character == GAME_ERROR || res_add_quest == GAME_ERROR) return GAME_ERROR;
    #endif

    char* character_copy = strdup(new_character);
    char* question_copy  = strdup(new_question);

    if (character_copy == NULL || question_copy == NULL)
    {
        #ifdef DEBUG
        tree->error |= TREE_NOT_MEMORY;
        #endif

        free(character_copy);
        free(question_copy);
        return GAME_ERROR;
    }

    node_t* left_node  = {};
    node_t* right_node = {};

    tree_error_t res_create_left  = node_create(tree, &left_node,  character_copy);
    tree_error_t res_create_right = node_create(tree, &right_node, node->value);

    if (res_create_left == TREE_ERROR || res_create_right == TREE_ERROR)
    {
        free(character_copy);
        free(question_copy);
        free(left_node);
        free(right_node);
        return GAME_ERROR;
    }

    node->value = question_copy;
    node->left  = left_node;
    node->right = right_node;

    sleep_print(MY_GREEN_AND_CURSIVE "Got it, I will remember\n" MY_END_CUSTOM);

    return GAME_SUCCESS;
}

game_error_t play_again(tree_t* tree, bool* answer)
{
    sleep_print(MY_CURSIVE "Do you want to play again?\n" MY_END_CUSTOM);

    game_error_t res_reading = reading_user_answer(tree, answer);

    if (res_reading == GAME_ERROR)
        return GAME_ERROR;

    if (*answer == false)
        return leave_game(tree);

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

        else if (strcmp(user_answer, "LEAVE") == 0)
        {
            *answer = true;
            leave_game(tree);
        }

        else
        {
            sleep_print(MY_RED_AND_CURSIVE "Please, write yes or no\n" MY_END_CUSTOM);
        }
    }
}

