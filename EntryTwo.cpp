// Attached: Lab #13
// File: EntryTwo.cpp
// ========================================================================
//
// Programmers: Aiden Asar, Filip Vracevic
// Class: CS 1D
// Instructor: Med Mogasemi
//
// ========================================================================
// Program: MedFlix (Movie tracking system) (lab #13)
// ========================================================================
// MedFlix driver program. Calls init, update, and render, then exits.
// ========================================================================
// ========================================================================


#include "MedFlix.h"

// === main ===============================================================
//
// ========================================================================
int main()
{
    /* Load the database file and create the window context. */
    MedFlix *app = new MedFlix();

    /* Main program loop: update internal variables, then render */
    while (!app->done())
        app->update(), app->render();

    /* Exit the program once it is finished. */
    int exit = app->exit();
    delete app;
    return exit;
}
