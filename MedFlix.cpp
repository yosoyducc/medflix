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

// raygui is responsible for managing onscreen text boxes etc.
extern "C" {
    #define RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

using std::string;

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
    // Load style definition and get properties for this style
    GuiLoadStyle("style_dark.rgs");     // this leaks two blocks of memory
    background = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

// Left bar variables (temp)
static string const medFlix  = "MedFlix";
static string const listText = "#185#Home;#186#Favorites;#043#Search;#151#Account;#159#Quit";
static string const tempText = "Press the \"MedFlix\" button to \"log in\"!";
static string const status   = "MedFlix 0.1.0  |  Copyright © 2024 Aiden Asar & Filip Vracevic. All Rights Reserved.";

static int listScrollIdx = 0;
static int listActive = 0;
static int loggedIn = 0;

static Rectangle layouts[] = {
    { 8, 8, 144, 40 },      // button rectangle
    { 8, 48, 144, 272 },    // list rectangle
    { 192, 16, 336, 72 },   // dummy placeholder text rectangle
    { 0, 0, 0, 24 },        // statusbar rectangle
};

void MedFlix::update()
{
    // Test if conditions are met to draw the exit box
    if (IsKeyPressed(KEY_ESCAPE))
        exitPrompt = !exitPrompt;
    if (WindowShouldClose())
        programShouldClose = true;

    // subtract padding, starting y position, and status bar
    layouts[1].height = GetScreenHeight() - 8 - 48 - layouts[3].height;
    layouts[3].y = GetScreenHeight() - layouts[3].height;
    layouts[3].width = GetScreenWidth();
}

void MedFlix::render()
{
    // Render the game based on current state of affairs
    BeginDrawing();
        ClearBackground(background);

        // Draw left hand menu bar
        if (GuiButton(layouts[0], medFlix.data())) {
            // temporary fake login when pressing the MedFlix button
            loggedIn = !loggedIn;
            listActive = 0;
        }
        // offset the char array to only display account page for sign in
        GuiListView(layouts[1], loggedIn ? listText.data() : listText.data() + 37, &listScrollIdx, &listActive);

        // Draw placeholder rectangle if not "signed in"
        if (!loggedIn)
            GuiDummyRec(layouts[2], tempText.data());

        // Draw status bar at bottom of program
        GuiStatusBar(layouts[3], status.data());

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
            Rectangle bounds = { hwidth - 125, hheight - 75, 250, 150 };
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
