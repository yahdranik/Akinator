#include "app_def_dif.h"
#include "app_common.h"
#include "C:\raylib-6.0_win64_mingw-w64\include\raylib.h"

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

app_error_t update_define(app_state_t* app)
{
    if (app == NULL) return APP_ERROR;

    if (app->mode_stage == MODE_INPUT_FIRST)
    {
        update_text_field(app->name_buffer);

        if (app->name_buffer[0] != '\0' && (IsKeyPressed(KEY_ENTER) || is_button_clicked(get_single_button())))
        {
            app->mode_stage = MODE_RESULT;
        }
    }

    else if (is_button_clicked(get_single_button()))
    {
        app->mode_stage = MODE_INPUT_FIRST;
        app->name_buffer[0] = '\0';
        app->question_buffer[0] = '\0';
    }

    return APP_SUCCESS;
}

app_error_t draw_define(app_state_t* app, Font font_text, Font font_headers)
{
    DrawTextEx(font_headers, "DEFINE MODE", (Vector2){60, 50}, 70, 1, DARKPURPLE);
    DrawTextEx(font_text, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", (Vector2){60, 120}, 30, 1, DARKPURPLE);
    draw_button(get_back_button(), "back", font_text);

    if (app->mode_stage == MODE_INPUT_FIRST)
    {
        DrawTextEx(font_text, "Enter character name:", (Vector2){60, 200}, 30, 1, BLACK);
        draw_text_field(get_text_field(), app->name_buffer, font_text);
        draw_confirm_button(app->name_buffer, font_text);

        return APP_SUCCESS;
    }

    path_step_t path[MAX_PATH_SIZE] = {};
    size_t path_size = 0;
    node_t* found_node = NULL;

    if (find_node(app->tree.root, app->name_buffer, &found_node,path, &path_size) == GAME_ERROR)
    {
        DrawTextEx(font_text, "Sorry, unknown character", (Vector2){60, 200}, 30, 1, MAROON);
    }

    else
    {
        DrawTextEx(font_text, TextFormat("This is what i know about %s:", app->name_buffer),
                  (Vector2){60, 200}, 40, 1, DARKPURPLE);

        char sentence[MAX_SENTENCE_SIZE] = "";

        for (size_t i = 0; i < path_size; i++)
        {
            strcat(sentence, path[i].answer ? "" : "not");
            strcat(sentence, (const char*) path[i].question_node->value);

            if (i == path_size - 2)
            {
                strcat(sentence, " and ");
            }

            else if (i < path_size - 1)
            {
                strcat(sentence, ", ");
            }

            else
            {
                strcat(sentence, ".");
            }
        }

        size_t line_length = 0;

        for (size_t i = 0; sentence[i] != '\0'; i++)
        {
            line_length++;

            if (line_length >= 40 && sentence[i] == ' ')
            {
                sentence[i] = '\n';
                line_length = 0;
            }
        }

        DrawTextEx(font_text, sentence, (Vector2){60, 260}, 30, 1, BLACK);
    }

    draw_button(get_single_button(), "define again", font_text);
    return APP_SUCCESS;
}

app_error_t update_difference(app_state_t* app)
{
    if (app == NULL) return APP_ERROR;

    if (app->mode_stage == MODE_INPUT_FIRST)
    {
        update_text_field(app->name_buffer);

        if (app->name_buffer[0] != '\0' && (IsKeyPressed(KEY_ENTER) || is_button_clicked(get_single_button())))
        {
            app->mode_stage = MODE_INPUT_SECOND;
        }
    }

    else if(app->mode_stage == MODE_INPUT_SECOND)
    {
        update_text_field(app->question_buffer);

        if (app->question_buffer[0] != '\0' && (IsKeyPressed(KEY_ENTER) || is_button_clicked(get_single_button())))
        {
            app->mode_stage = MODE_RESULT;
        }
    }

    else if (is_button_clicked(get_single_button()))
    {
        app->mode_stage = MODE_INPUT_FIRST;
        app->name_buffer[0] = '\0';
        app->question_buffer[0] = '\0';
    }

    return APP_SUCCESS;
}

app_error_t draw_difference(app_state_t* app, Font font_text, Font font_headers)
{
    DrawTextEx(font_headers, "DIFFERENCE MODE", (Vector2){60, 50}, 70, 1, DARKPURPLE);
    DrawTextEx(font_text, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", (Vector2){60, 120}, 30, 1, DARKPURPLE);
    draw_button(get_back_button(), "back", font_text);

    if (app->mode_stage == MODE_INPUT_FIRST)
    {
        DrawTextEx(font_text, "Enter name first character:", (Vector2){60, 200}, 30, 1, BLACK);
        draw_text_field(get_text_field(), app->name_buffer, font_text);
        draw_confirm_button(app->name_buffer, font_text);

        return APP_SUCCESS;
    }

    path_step_t first_path[MAX_PATH_SIZE] = {};
    size_t first_size = 0;
    node_t* first_node = NULL;

    game_error_t res1 = find_node(app->tree.root, app->name_buffer, &first_node, first_path, &first_size);
    if (res1 == GAME_ERROR)
    {
        DrawTextEx(font_text, "Unknown character", (Vector2){60, 200}, 35, 1, MAROON);
        draw_button(get_single_button(), "again", font_text);
        return APP_SUCCESS;
    }

    if (app->mode_stage == MODE_INPUT_SECOND)
    {
        DrawTextEx(font_text, "Enter name second character:", (Vector2){60, 200}, 35, 1, BLACK);
        draw_text_field(get_text_field(), app->question_buffer, font_text);
        draw_confirm_button(app->question_buffer, font_text);

        return APP_SUCCESS;
    }

    path_step_t second_path[MAX_PATH_SIZE] = {};
    size_t second_size = 0;
    node_t* second_node = NULL;

    game_error_t res2 = find_node(app->tree.root, app->question_buffer, &second_node, second_path, &second_size);
    if (res2 == GAME_ERROR)
    {
        DrawTextEx(font_text, "Unknown character", (Vector2){60, 200}, 35, 1, MAROON);
        draw_button(get_single_button(), "again", font_text);
        return APP_SUCCESS;
    }

    char buffer[1024] = {};
    size_t used = 0;
    size_t ptr = 0;

    while (ptr < first_size && ptr < second_size &&
           first_path[ptr].question_node == second_path[ptr].question_node &&
           first_path[ptr].answer == second_path[ptr].answer)
    {
        int written = snprintf(buffer + used, sizeof(buffer) - used,
                               "%s%s%s",
                               ptr > 0 ? ", " : "",
                               first_path[ptr].answer ? "" : "not ",
                               (char*) first_path[ptr].question_node->value);

        if (written < 0 || (size_t) written >= sizeof(buffer) - used)
            break;

        used += (size_t) written;
        ptr++;
    }

    if (ptr > 0)
    {
        DrawTextEx(font_text, TextFormat("%s and %s are similar: they both %s.",
                              app->name_buffer, app->question_buffer, buffer), (Vector2){60, 200}, 30, 1, BLACK);
    }

    if (ptr >= first_size || ptr >= second_size)
    {
        DrawTextEx(font_text, "They are identical!", (Vector2){60, 320}, 35, 1, DARKPURPLE);
    }
    else
    {
        char* quest = (char*) first_path[ptr].question_node->value;

        DrawTextEx(font_text, "Differences:", (Vector2){60, 270}, 35, 1, DARKPURPLE);

        DrawTextEx(font_text, TextFormat("%s %s %s, but %s %s %s.",
                              app->name_buffer,
                              first_path[ptr].answer ? "is" : "is not", quest,
                              app->question_buffer,
                              second_path[ptr].answer ? "is" : "is not", quest), 
                              (Vector2){60, 310}, 30, 1, BLACK);
    }

    draw_button(get_single_button(), "compare again", font_text);

    return APP_SUCCESS;
}