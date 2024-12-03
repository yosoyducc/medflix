// Attached: Lab #13
// File: MedFlix.cpp
// ========================================================================
//
// Programmers: Aiden Asar, Filip Vracevic
// Class: CS 1D
// Instructor: Med Mogasemi
//
// ========================================================================
// Program: MedFlix (Movie tracking system) (lab #13)
// ========================================================================
// MedFlix graphcal frontend. Asks user to log in or register. From there,
// it can show the user recommendations of movies to view/watch based on
// what they've double liked in the past.
// ========================================================================
// ========================================================================


// Local includes
#include "MedFlix.h"
#include "Config.h"
#include "IniReader.h"      // Loading database into memory
#include <raylib.h>

// raygui is responsible for managing onscreen text boxes etc.
extern "C" {
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

// Use initializer list to call IniReader's constructor for `db`
// notice that this is called before we enter this constructor.
MedFlix::MedFlix() : db("database.ini")
{
    /*** WINDOW CREATION ***/
    // Create the window with configured values
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MedFlix");
    // Lock window so it can't be resized to smaller than minimum
    SetWindowMinSize(SCREEN_MIN_WIDTH, SCREEN_MIN_HEIGHT);
    // Set the taarget refresh rate of the program (60)
    SetTargetFPS(TARGET_FPS);
    // Tell raylib the window should be resizable
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    // Tell raylib we don't want the ESC key to exit the program,
    // we'll be managing this ourselves
    SetExitKey(KEY_NULL);

    /*** VARIABLES DEFINITIONS ***/
    // Define those program runtime booleans and ints
    exitCode = 0;
    programShouldClose = false;
    exitPrompt = false;

    /* Cosmetics get! */
    // Get background color for this style
    background = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

void MedFlix::update()
{
    // Test if conditions are met to draw the exit box
    if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE))
        exitPrompt = true;
}

void MedFlix::render()
{
    // Render the game based on current state of affairs
    BeginDrawing();
        ClearBackground(background);

        // Draw the exit box if required
        if (exitPrompt) {
            // Establish boundaries for the box
            float hwidth  = GetScreenWidth() / 2.0f,
                  hheight = GetScreenHeight() / 2.0f;
            Rectangle bounds = { hwidth - 125, hheight - 75, 250, 150 };
            Color fadeBg = {background.r, background.g, background.b, 0x80};
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), fadeBg);
            // Draw message box and get response from user
            int choice = GuiMessageBox(bounds, "#015#EXIT PROGRAM", "Really quit MedFlix?", "Yes :(;No");

            // If the user closes prompt or hits no
            if (choice == 0 || choice == 2)
                exitPrompt = false;     // no longer display the exit box
            // If the user wants to exit
            else if (choice == 1)
                programShouldClose = true;
            // If user doesn't choose anything
        }
    EndDrawing();
}

bool MedFlix::done()
{
    return programShouldClose;
}

int MedFlix::exit()
{
    CloseWindow();
    return exitCode;
}
