#ifndef APP_BUTTONS
#define APP_BUTTONS

#include "raylib.h"
#include "app_common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Rectangle get_menu_button(int index);
Rectangle get_back_button();
bool is_button_hovered(Rectangle button);
bool is_button_clicked(Rectangle button);
void draw_button(Rectangle button, const char* text, Font font);
void draw_disabled_button(Rectangle button, const char *text, Font font);
void draw_confirm_button(const char* text, Font font);
void draw_text_field(Rectangle field, const char* text, Font font);
Rectangle get_answer_button(int index);
Rectangle get_single_button();
Rectangle get_text_field();

#endif