#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "C:\Users\MilanaGlav\.vscode\Akinator\Akinator\BIN_TREE\HEADERS\Tree_Init_Ops.h"
#include "C:\Users\MilanaGlav\.vscode\Akinator\Akinator\BIN_TREE\HEADERS\Tree_Fill_Ops.h"
#include "C:\Users\MilanaGlav\.vscode\Akinator\Akinator\BIN_TREE\HEADERS\Tree_Print_Ops.h"
#include "C:\Users\MilanaGlav\.vscode\Akinator\Akinator\BIN_TREE\HEADERS\Tree_Dest_Ops.h"

const int TIME_TO_SLEEP = 10;
const int MAX_LEN_STR   = 64;
enum game_error_t
{
    GAME_SUCCESS,
    GAME_ERROR
};

enum game_error_codes
{

};

void sleep_print(const char *string);
void hello_and_menu();
void choose_mode();