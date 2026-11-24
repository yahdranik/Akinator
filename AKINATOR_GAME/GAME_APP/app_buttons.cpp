#include "app_buttons.h"
#include "C:\raylib-6.0_win64_mingw-w64\include\raylib.h"

Rectangle get_menu_button(int index)
{
    float x = (float) (WINDOW_WIDTH - BUTTON_WIDTH) / 2;
    float y = 200 + (float) (index * (BUTTON_HEIGHT + BUTTON_GAP));

    return {x, y, (float) BUTTON_WIDTH, (float) BUTTON_HEIGHT};
}

Rectangle get_back_button()
{
    return {60, (float) (WINDOW_HEIHGT - 110), 160, 50};
}

bool is_button_hovered(Rectangle button)
{
    return CheckCollisionPointRec(GetMousePosition(), button);
}

bool is_button_clicked(Rectangle button)
{
    return is_button_hovered(button) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
}

void draw_button(Rectangle button, const char* text, Font font)
{
    Color color = PURPLE;

    if (is_button_hovered(button))
    {
        color = DARKPURPLE;
    }
    
    DrawRectangleRec(button, color);

    int text_width = MeasureText(text, BUTTON_FONT);
    float text_x = button.x + ( button.width - text_width) / 2;
    float text_y = button.y + (button.height - BUTTON_FONT) / 2;

    DrawTextEx(font, text, (Vector2) {text_x, text_y}, BUTTON_FONT, 1, WHITE);
}

void draw_disabled_button(Rectangle button, const char *text, Font font)
{
    DrawRectangleRec(button, LIGHTGRAY);

    int text_width = MeasureText(text, BUTTON_FONT);
    float text_x = (button.x + (button.width - text_width) / 2);
    float text_y = (button.y + (button.height - BUTTON_FONT) / 2);

    DrawTextEx(font, text, (Vector2){text_x, text_y}, 40, 1, GRAY);
}

void draw_confirm_button(const char* text, Font font)
{
    if (text[0] == '\0')
    {
        draw_disabled_button(get_single_button(),"OK", font);
    }

    else 
    {
        draw_button(get_single_button(), "OK", font);
    }
}

void draw_text_field(Rectangle field, const char* text, Font font)
{
    DrawRectangleRec(field, WHITE);
    DrawRectangleLinesEx(field, 3, PURPLE);

    DrawTextEx(font, text, (Vector2){field.x + 15, field.y + 15}, 40, 1, GRAY);
}

Rectangle get_answer_button(int index)
{
    float x = ((float) WINDOW_WIDTH - (float) (2 * BUTTON_WIDTH + BUTTON_GAP)) / 2 
             + (float) (index * (BUTTON_WIDTH + BUTTON_GAP));

    return {x, 350, (float) BUTTON_WIDTH, (float) BUTTON_HEIGHT};
}

Rectangle get_single_button()
{
    return {(float) (WINDOW_WIDTH - BUTTON_WIDTH) / 2, 380, (float) BUTTON_WIDTH, (float) BUTTON_HEIGHT};
}

Rectangle get_text_field()
{
    return {120, 300, 850, 60};
}
