#ifndef APP_GUESS
#define APP_GUESS

#include "app_common.h"
#include "app_buttons.h"
#include "app_def_dif.h"

// PROTOTYPES ----------------------------------------------

app_error_t app_init(app_state_t* app);
app_error_t app_destroy(app_state_t* app);
app_error_t app_update(app_state_t* app);
app_error_t app_draw(app_state_t* app, Texture2D background, Font font_text, Font font_headers);
app_error_t update_guess(app_state_t* app);
app_error_t draw_guess(app_state_t* app, Font font_text, Font font_headers);

bool is_leaf_node(node_t* node);
app_error_t add_character(app_state_t* app);


#endif
