#include "app_common.h"
#include "app_guess.h"

int main() 
{

    app_state_t app = {};

    app_error_t res_init = app_init(&app);
    if (res_init == APP_ERROR) return -1;

    InitWindow(WINDOW_WIDTH, WINDOW_HEIHGT, "Akinator");

    Texture2D background = LoadTexture("DESIGN/background_disney.png");
    Font font_headers = LoadFontEx("DESIGN/Fishface.ttf", 32, NULL, 0);
    Font font_text = LoadFontEx("DESIGN/JannonTextMedSC BoldItalic.otf", 32, NULL, 0);
    SetTextureFilter(font_headers.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(font_text.texture, TEXTURE_FILTER_BILINEAR);

    InitAudioDevice();
    Music music = LoadMusicStream("DESIGN/Akinator_music.mp3");
    music.looping = true;
    SetMusicVolume(music, 0.4f);
    PlayMusicStream(music);

    SetExitKey(KEY_NULL);

    SetTargetFPS(60);

    while (app.is_running && !WindowShouldClose())
    {
        UpdateMusicStream(music);

        app_error_t res_update = app_update(&app);
        app_error_t res_draw = app_draw(&app, background, font_text, font_headers);

        if (res_update == APP_ERROR || res_draw == APP_ERROR)
        {
            break;
        }
    }

    UnloadMusicStream(music);
    CloseAudioDevice();

    UnloadTexture(background);
    CloseWindow();

    FILE* html = fopen(HTML_FILE, "w");

    if (html == NULL) 
    {
        printf("file do not open\n");
        return -1;
    }

    TREE_DUMP(html, &app.tree);
    fclose(html);

    app_error_t res_dest = app_destroy(&app);
    if (res_dest == APP_ERROR) return -1;
}