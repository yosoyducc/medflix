/* Simple raylib example with IniReader.
 */

#include "IniReader.h"
extern "C" {
    #include "raylib.h"
}

int main()
{
    IniReader ini("database.ini");
    int sec = ini.findSection("the spongebob squarepants movie");
    // raylib only supports printing c-style strings
    const char *txt = ini.getSectionName(sec).c_str();

    // window initialization and load spongebob image
    InitWindow(1280, 720, "MedFlix 0.1.0");
    SetTargetFPS(60);
    // get the poster name and load its texture into VRAM
    Texture2D text = LoadTexture(TextFormat("poster/%s", ini(sec, 7).c_str()));

    // main program loop
    while (!WindowShouldClose()) {
        // tell raylib to start drawing to the screen
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawTexture(text, GetScreenWidth() / 2 - text.width / 2, GetScreenHeight() / 2 - text.height / 2, WHITE);
            DrawText(txt, GetScreenWidth() / 2 - MeasureText(txt, 20) / 2, 150, 20, BLACK);
        EndDrawing();
    }

    // unload texture after done
    UnloadTexture(text);
    CloseWindow();

    return 0;
    // we don't need to unload ini, local scope is done
}
