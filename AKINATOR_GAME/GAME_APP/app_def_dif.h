#ifndef APP_DEF_DIF
#define APP_DEF_DIF

#include "app_common.h"
#include "app_buttons.h"
#include "../../AKINATOR_GAME/CONSOLE/akinator.h"
#include "string.h"

const size_t MAX_SENTENCE_SIZE = 2048;
void reset_mode_input(app_state_t* app);

app_error_t update_define(app_state_t* app);
app_error_t draw_define(app_state_t* app, Font font_text, Font font_headers);

app_error_t update_difference(app_state_t* app);
app_error_t draw_difference(app_state_t* app, Font font_text, Font font_headers);

#endif