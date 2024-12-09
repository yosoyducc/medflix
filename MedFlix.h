// Attached: Lab #13
// File: MedFlix.h
// ========================================================================
//
// Programmers: Aiden Asar, Filip Vracevic
// Class: CS 1D
// Instructor: Med Mogasemi
//
// ========================================================================
// Program: MedFlix (Movie tracking system) (lab #13)
// ========================================================================
// Declarations for the MedFlix graphical frontend.
// ========================================================================
// ========================================================================


#pragma once

// Local includes
#include "AccountManager.h"
#include "Config.h"
#include "IniReader.h"
#include "ScreenObjects.h"

// raylib include (raygui included in MedFlix.cpp)
// We need the extern "C" here because raylib is a C library
// and function names cannot be mangled
extern "C" {
    #include "raylib.h"
}

// C++ stdlib includes
#include <string>
#include <vector>

class MedFlix {
public:
    // === MedFlix ========================================================
    // Initializes the application. Loads the database from the disk and
    // creates the window context.
    // ====================================================================
    MedFlix();

    // === update =========================================================
    // Updates the application variables.
    //
    // Parameters:
    //      none
    // Returns:
    //      void
    // ====================================================================
    void update();

    // === render =========================================================
    // Renders and draws the screen.
    //
    // Parameters:
    //      none
    // Returns:
    //      void
    // ====================================================================
    void render();

    // === done ===========================================================
    // Returns whether the application is ready to exit.
    //
    // Parameters:
    //      none
    // Returns:
    //      boolean truth
    // ====================================================================
    bool done();

    // === exit ===========================================================
    // Uninitializes the application and returns the exit status.
    //
    // Parameters:
    //      none
    // Returns:
    //      int status
    // ====================================================================
    int exit();

private:
    IniReader db;               // Movies database
    AccountManager acct;        // Currently signed in user account
    ScreenObjects so;           // Screen objects (only init objs once per run)
    HashTable ht;               // Hash table of all movies

    // THE STATE OF AFFAIRS (program variables)
    int exitCode;               // Program exit code
    bool programShouldClose;    // Should the program close?
    bool exitPrompt;            // Display exit prompt

    // Login screen variables
    bool login;

    // Cosmetics
    Color background;

    // === fillHash =======================================================
    // Initialize the hashmap with data from the ini database.
    //
    // Parameters:
    //      none
    // Returns:
    //      void
    // ====================================================================
    void fillHash();
};
