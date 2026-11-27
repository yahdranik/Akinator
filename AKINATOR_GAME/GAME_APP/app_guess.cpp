#include "app_guess.h"
#include "app_common.h"
#include "app_def_dif.h"

app_error_t app_init(app_state_t* app)
{
    app->screen = SCREEN_MENU;
    app->is_running = true;

    if (tree_init(&app->tree) == TREE_ERROR)
    {
        printf("Tree init failed\n");
        return APP_ERROR;
    }

    FILE* file_tree = fopen(BASE_FILE, "r");
    if (file_tree == NULL)
    {
        app->error |= APP_FILE_NOT_OPEN;
        return APP_ERROR;
    }

    tree_error_t res_fill = fill_tree(file_tree, &app->tree, BASE_FILE);

    if (res_fill == TREE_ERROR)
    {
        printf("Can't read tree");
        return APP_ERROR;
    }

    app->current_node = app->tree.root;

    fclose(file_tree);
    return APP_SUCCESS;
}

app_error_t app_destroy(app_state_t* app)
{
    if (app == NULL) return APP_ERROR;

    tree_error_t res_dest_tree = tree_destroy(&(app->tree), destroy_string_value);
    if (res_dest_tree == TREE_ERROR)
    {
        app->error |= APP_DEST_TREE_FAIL;
        return APP_ERROR;
    }

    return APP_SUCCESS;
}

app_error_t app_update(app_state_t* app)
{
    if (app == NULL) return APP_ERROR;

    if (IsKeyPressed(KEY_ESCAPE))
    {
        if (app->screen == SCREEN_MENU)
        {
            app->is_running = false;
        }

        else 
        {
            app->screen = SCREEN_MENU;
        }

        return APP_SUCCESS;
    }

    if (app->screen == SCREEN_MENU)
    {
        for (int i = 0; i < MENU_BUTTONS_COUNT; i++)
        {
            Rectangle button = get_menu_button(i);

            if (is_button_clicked(button))
            {
                app->screen         = MENU_SCREENS[i];
                app->current_node   = app->tree.root;
                app->guess_stage    = GUESS_ASKING;
                app->mode_stage     = MODE_INPUT_FIRST;
                app->name_buffer[0]     = '\0';
                app->question_buffer[0] = '\0';
                break;
            }
        }
    }

    else 
    {
        Rectangle back_button = get_back_button();

        if (is_button_clicked(back_button))
        {
            app->screen = SCREEN_MENU;
        }

        if (app->screen == SCREEN_GUESS)
        {
            update_guess(app);
        }

        if (app->screen == SCREEN_DEFINE)
        {
            update_define(app);
        }

        if (app->screen == SCREEN_DIFFERENCE)
        {
            update_difference(app);
        }
    }

    return APP_SUCCESS;
}

app_error_t app_draw(app_state_t* app, Texture2D background, Font font_text, Font font_headers)
{
    if (app == NULL) return APP_ERROR;

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(background, 0, 0, WHITE);
    switch (app->screen)
    {
        case SCREEN_MENU:
            DrawTextEx(font_headers, "DISNEY AKINATOR", (Vector2){60, 50}, 70, 1, DARKPURPLE);

            for (int i = 0; i < MENU_BUTTONS_COUNT; i++)
            {
                draw_button(get_menu_button(i), MENU_LABELS[i], font_text);
            }
            break;

        case SCREEN_GUESS:
            draw_guess(app, font_text, font_headers);
            break;

        case SCREEN_DEFINE:
            draw_define(app, font_text, font_headers);
            break;
        
        case SCREEN_DIFFERENCE:
            draw_difference(app, font_text, font_headers);
            break;
        
        default:
            break;
    }

    EndDrawing();

    return APP_SUCCESS;
}

bool is_leaf_node(node_t* node)
{
    return node->left == NULL && node->right == NULL;
}

app_error_t update_guess(app_state_t* app)
{
    if (app == NULL) return APP_ERROR;

    node_t* node = app->current_node;

    if (app->guess_stage == GUESS_ASKING)
    {
        Rectangle yes_button = get_answer_button(0);
        Rectangle no_button  = get_answer_button(1);

        if (is_button_clicked(yes_button))
        {
            if (is_leaf_node(node))
            {
                app->guess_stage = GUESS_WON;
            }

            else 
            {
                app->current_node = node->left;
            }
        }

        else if(is_button_clicked(no_button))
        {
            if (is_leaf_node(node))
            {
                app->name_buffer[0] = '\0';
                app->guess_stage = GUESS_ENTER_NAME;
            }

            else 
            {
                app->current_node = node->right;
            }
        }
    }

    else if (app->guess_stage == GUESS_ENTER_NAME || app->guess_stage == GUESS_ENTER_QUESTION)
    {
        update_enter_text(app);
    }

    else if (app->guess_stage == GUESS_WON || app->guess_stage == GUESS_ADDED || app->guess_stage == GUESS_FAILED)
    {
        Rectangle again_button = get_single_button();

        if (is_button_clicked(again_button))
        {
            app->current_node = app->tree.root;
            app->guess_stage = GUESS_ASKING;
        }
    }

    return APP_SUCCESS;
}

void update_text_field(char* buffer)
{
    int symbol = GetCharPressed();

    while (symbol > 0)
    {
        int len = strlen(buffer);

        if (symbol != '"' && symbol != '\n' && len < TEXT_BUFFER_SIZE - 1)
        {
            buffer[len] = symbol;
            buffer[len + 1] = '\0';
        }

        symbol = GetCharPressed();
    }

    size_t len_final = strlen(buffer);

    if (IsKeyPressed(KEY_BACKSPACE) && len_final > 0)
    {
        buffer[len_final - 1] = '\0';
    }
}

void update_enter_text(app_state_t* app)
{
    char* buffer = NULL;

    if (app->guess_stage == GUESS_ENTER_NAME)
    {
        buffer = app->name_buffer;
    }

    else if (app->guess_stage == GUESS_ENTER_QUESTION)
    {
        buffer = app->question_buffer;
    }

    update_text_field(buffer);

    if (!(IsKeyPressed(KEY_ENTER) || is_button_clicked(get_single_button())) || buffer[0] == '\0')
    {
        return;
    }

    if (app->guess_stage == GUESS_ENTER_NAME)
    {
        app->question_buffer[0] = '\0';
        app->guess_stage = GUESS_ENTER_QUESTION;
    }

    else 
    {
        if (!add_character(app))
        {
            app->guess_stage = GUESS_FAILED;
        }

        else 
        {
            app->guess_stage = GUESS_ADDED;
        }
    }
}

app_error_t add_character(app_state_t* app)
{
    node_t* node = app->current_node;

    size_t len_question = strlen(app->question_buffer);

    char* character_copy = strdup(app->name_buffer);
    char* question_copy = strdup(app->question_buffer);

    if (character_copy == NULL || question_copy == NULL)
    {
        app->error |= APP_NOT_MEMORY;
        free(character_copy);
        free(question_copy);

        return APP_ERROR;
    }

    node_t* left_node = NULL;
    node_t* right_node = NULL;

    tree_error_t res_create_left = node_create(&app->tree, &left_node, character_copy);
    tree_error_t res_create_right = node_create(&app->tree, &right_node, node->value);

    if (res_create_left == TREE_ERROR || res_create_right == TREE_ERROR)
    {
        free(character_copy);
        free(question_copy);
        free(left_node);
        free(right_node);

        return APP_ERROR;
    }

    node->value = question_copy;
    node->left = left_node;
    node->right = right_node;

    FILE* file_tree = fopen(BASE_FILE, "w");
    print_tree(file_tree, &(app->tree), print_str);
    fclose(file_tree);

    return APP_SUCCESS;
}

app_error_t draw_guess(app_state_t* app, Font font_text, Font font_headers)
{
    if (app == NULL) return APP_ERROR;
    DrawTextEx(font_headers, "GUESS MODE", (Vector2){60, 50}, 70, 1, DARKPURPLE);
    DrawTextEx(font_text, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", (Vector2){60, 120}, 30, 1, DARKPURPLE);
    draw_button(get_back_button(), "back", font_text);

    char* text = (char*) app->current_node->value;
    bool is_leaf = is_leaf_node(app->current_node);

    switch (app->guess_stage)
    {
        case GUESS_ASKING:
            DrawTextEx(font_text, is_leaf ? TextFormat("Is it %s?", text) : TextFormat("%s?", text), 
                     (Vector2) {60, 200}, 35, 1, BLACK);
            draw_button(get_answer_button(0), "Yes", font_text);
            draw_button(get_answer_button(1), "No", font_text);
            break;

        case GUESS_WON:
            DrawTextEx(font_text, "I knew it!", (Vector2) {60, 200}, 35, 1, DARKGREEN);
            draw_button(get_single_button(), "Play again", font_text);
            break;

        case GUESS_ENTER_NAME:
            DrawTextEx(font_text, "What was the character did you think of? \n (for example: Lion King)", 
                      (Vector2) {60, 180}, 30, 1, BLACK);
            draw_text_field(get_text_field(), app->name_buffer, font_text);
            draw_confirm_button(app->name_buffer, font_text); 
            break;

        case GUESS_ENTER_QUESTION:
            DrawTextEx(font_text, TextFormat("Write a question (without ?) that true for %s",
                       app->name_buffer), (Vector2){60, 180}, 30, 1, BLACK);
            DrawTextEx(font_text, TextFormat("and false for %s. Example: an animal", 
                       text), (Vector2){60, 210}, 30, 1, BLACK);
            draw_text_field(get_text_field(), app->question_buffer, font_text);
            draw_confirm_button(app->question_buffer, font_text);
            break;
        
        case GUESS_ADDED:
            DrawTextEx(font_text, "I got it!", (Vector2){60, 200}, 30, 1, DARKGREEN);
            draw_button(get_single_button(), "Play again", font_text);
            break;
        
        case GUESS_FAILED:
            DrawTextEx(font_text, "Something went wrong, the character was not saved.", (Vector2){60, 200}, 30, 1, MAROON);
            draw_button(get_single_button(), "Play again", font_text);
            break;

        default:
            break;
    }

    return APP_SUCCESS;
}

