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
    #include "raylib.h"
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

    // Initialize the on-screen objects
    so.status.init();
    so.sidebar.init();
    so.recommend.init();
    so.account.init();

    /* Cosmetics get! */
    // Load style definition and get properties for this style
    GuiLoadStyle("style_dark.rgs");     // this leaks two blocks of memory
    background = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
}

void MedFlix::update()
{
    // Test if conditions are met to draw the exit box
    if (IsKeyPressed(KEY_ESCAPE)) {
        exitPrompt = !exitPrompt;

        // get around a bug allowing press after esc on dropdown
        so.account.dropActionEditMode = false;
    }
    if (WindowShouldClose())
        programShouldClose = true;

    // If sign in button is pressed, determine which mode is
    // selected then take an appropriate action
    if (so.account.butSigninPressed) switch (so.account.dropActionActive) {
    case 0:     // SIGN IN mode
        if (acct.signIn(so.account.entryUserText, so.account.entryPassText)) {
            TraceLog(LOG_INFO, "Signed in as %s",
                acct.getUserData()->getPropertyValue(0, 0).data());
            // "clear" the user/pass entry buffers
            so.account.entryUserText[0] = 0x0;
            so.account.entryPassText[0] = 0x0;
            // switch screen to Home
            so.sidebar.listActive = ScreenObjects::HOME;
        } else {
            // TODO: set label message to account sign in fail
            so.account.loginFailTimeout = 3.5f;
        }
        break;
    case 1:     // REGISTER mode
        if (AccountManager::create(so.account.entryUserText, so.account.entryPassText)) {
            // TODO: set label message to account registration success
        } else {
            // TODO: TEMPORARY: sign out user if failed account creation (only way to sign out right now)
            if (acct.signOut())
                TraceLog(LOG_INFO, "Successfully signed out.");
            // set to Account page (remember the offsets)
            so.sidebar.listActive = ScreenObjects::HOME;
            so.account.loginFailTimeout = 3.0f;
        }
        break;
    }
    // In either case, we want to reset the button press state
    // otherwise the program will infinitely attempt to sign us in
    so.account.butSigninPressed = false;

    // If exit prompt is active, disable onscreen elements
    if (exitPrompt)
        GuiLock();
}

void MedFlix::render()
{
    // Render the game based on current state of affairs
    BeginDrawing();
        ClearBackground(background);

        // Draw bottom-most status bar
        so.status.draw();

        // Draw left hand menu bar with signout offset,
        // checking whether the user wants to quit first
        int listActiveLast = so.sidebar.listActive;
        so.sidebar.draw(acct);
        int listActiveTrue = so.sidebar.listActive + so.sidebar.listActiveOff;

        if (listActiveTrue == ScreenObjects::QUIT) {
            exitPrompt = true;
            so.sidebar.listActive = listActiveLast;
        }

        // Render the main surface based on what tab is selected
        switch (listActiveTrue) {
        case ScreenObjects::HOME:
            so.recommend.draw();
            break;
        case ScreenObjects::ACCOUNT:
            so.account.draw();
            break;
        default:
            break;
        }

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
            GuiUnlock();
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

            // Gui gets locked again by the check in ::update()
        }
    EndDrawing();
}

bool MedFlix::done()
{
    return programShouldClose;
}

int MedFlix::exit()
{
    // Sign out of account, if signed in
    acct.signOut();

    // uncomment to avoid the two-block leak from loading the style
    //GuiLoadStyleDefault();
    CloseWindow();
    return exitCode;
}
