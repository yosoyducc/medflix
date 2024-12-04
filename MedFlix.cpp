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
#include "ScreenObjects.h"

// raygui is responsible for managing onscreen text boxes etc.
extern "C" {
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

using std::string;

// TODO: move to class
static ScreenObjects so;
static char const *status = "MedFlix 0.1.0  |  Copyright © 2024 Aiden Asar & Filip Vracevic. All Rights Reserved.";
static int loggedIn = 0;
static Rectangle layouts[] = {
    { 192, 16, 336, 72 },   // dummy placeholder text rectangle
    { 0, 0, 0, 24 },        // statusbar rectangle
};

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

    // Initialize the sidebar
    so.sidebar.init();

    /* Cosmetics get! */
    // Load style definition and get properties for this style
    GuiLoadStyle("style_dark.rgs");     // this leaks two blocks of memory
    background = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

void MedFlix::update()
{
    // Test if conditions are met to draw the exit box
    if (IsKeyPressed(KEY_ESCAPE))
        exitPrompt = !exitPrompt;
    if (WindowShouldClose())
        programShouldClose = true;

    // subtract padding, starting y position, and status bar
    layouts[1].y = GetScreenHeight() - layouts[1].height;
    layouts[1].width = GetScreenWidth();
}

void MedFlix::render()
{
    // Render the game based on current state of affairs
    BeginDrawing();
        ClearBackground(background);

        // Draw status bar at bottom of program
        GuiStatusBar(layouts[1], status);

        // Draw left hand menu bar
        so.sidebar.draw();

        // Turn on word wrap and draw the text box with sample description
        /*GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);
        GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);
        // yes it's temporary but still horrible
        // we can copy this string only once and not every game loop lol
        string dat = db("star wars", "descript");
        dat.back() = 0x0;   // remove final quote
        // Draw the text starting after the opening quote
        GuiTextBox((Rectangle){ 40, 40, 300, 175 }, (char *)dat.data() + 1, 0, false);
        GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_NONE);
        GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);*/

        // Draw the exit box if required
        if (exitPrompt) {
            // Establish boundaries for the box
            float hwidth  = GetScreenWidth() / 2.0f,
                  hheight = GetScreenHeight() / 2.0f;
            Rectangle bounds = { hwidth - 125, hheight - 50, 250, 100 };
            // Bring attention to exit box by fading out other elements above
            DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), GuiFade(background, 0.7));
            // Draw message box and get response from user
            int choice = GuiMessageBox(bounds, "#141#EXIT PROGRAM", "Really quit MedFlix?", "Yes :(;No");

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
    // uncomment to avoid the two-block leak from loading the style
    //GuiLoadStyleDefault();
    CloseWindow();
    return exitCode;
}
