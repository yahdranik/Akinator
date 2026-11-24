#ifndef APP_COMMON
#define APP_COMMON

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../BIN_TREE/HEADERS/Tree_Common.h"
#include "../../BIN_TREE/HEADERS/Tree_Init_Ops.h"
#include "../../BIN_TREE/HEADERS/Tree_Fill_Ops.h"
#include "../../BIN_TREE/HEADERS/Tree_Print_Ops.h"

// DEFINES ------------------------------------------------

#define BASE_FILE "disney_test.txt"

// CONSTS -------------------------------------------------

const int WINDOW_WIDTH  = 1080;
const int WINDOW_HEIHGT = 1440;

const int BUTTON_WIDTH  = 320;
const int BUTTON_HEIGHT = 70;
const int BUTTON_GAP    = 30;
const int BUTTON_FONT   = 30;

const int TEXT_BUFFER_SIZE = 64;
const int TEXT_FONT        = 30;

enum screen_t
{
    SCREEN_MENU,
    SCREEN_GUESS,
    SCREEN_DEFINE,
    SCREEN_DIFFERENCE
};

enum app_error_t
{
    APP_ERROR,
    APP_SUCCESS
};

enum app_error_codes_t
{
    APP_OK             = 0,
    APP_FILE_NOT_OPEN  = 1<<1,
    APP_DEST_TREE_FAIL = 1<<2,
    APP_NOT_MEMORY     = 1<<3
};

enum guess_stage_t
{
    GUESS_ASKING,
    GUESS_WON,
    GUESS_ENTER_NAME,
    GUESS_ENTER_QUESTION,
    GUESS_ADDED,
    GUESS_FAILED
};

enum mode_stage_t
{
    MODE_INPUT_FIRST,
    MODE_INPUT_SECOND,
    MODE_RESULT
};

struct app_state_t
{
    screen_t screen;
    bool is_running;

    tree_t tree;
    node_t* current_node;

    guess_stage_t guess_stage;
    mode_stage_t mode_stage;

    char name_buffer[TEXT_BUFFER_SIZE];
    char question_buffer[TEXT_BUFFER_SIZE];

    #ifdef DEBUG
    uint32_t error;
    #endif 
};

const int MENU_BUTTONS_COUNT = 3;
const screen_t MENU_SCREENS[MENU_BUTTONS_COUNT] = {SCREEN_GUESS, SCREEN_DEFINE, SCREEN_DIFFERENCE};
const char* const MENU_LABELS[MENU_BUTTONS_COUNT]  = {"GUESS", "DEF", "DIF"}; 

void update_text_field(char* buffer);
void update_enter_text(app_state_t* app);

#endif