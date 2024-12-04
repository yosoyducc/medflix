// Attached: Lab #13
// File: ScreenObjects.h
// ========================================================================
//
// Programmers: Aiden Asar, Filip Vracevic
// Class: CS 1D
// Instructor: Med Mogasemi
//
// ========================================================================
// Program: MedFlix (Movie tracking system) (lab #13)
// ========================================================================
// Screen objects definitions for the graphical frontend. Provides
// initialization/retrieval/rendering functions for all elements possible.
// We categorize all onscreen objects into their own structs that provide
// this functionality. Only one type of ScreenObjects should exist in main.
// ========================================================================
// ========================================================================


#pragma once

// Include raygui/raylib as C library
extern "C" {
    //#include "raylib.h"
    // Make sure the implementation isn't defined twice
    #undef RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

class ScreenObjects {
public:
    // === Sidebar object =================================================
    //
    // ====================================================================
    struct {
        // === init =======================================================
        // Initialize the program sidebar.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void init()
        {
            // Estabish variables defaults
            headerPressed = false;
            listScrollIdx = 0;
            listActive = 0;

            // Sidebar header
            layout[0] = { 8, 8, 144, 40 };
            // Sidebar list
            layout[1] = { 8, 48, 144, 152 };
        }

        // === draw =======================================================
        // Draw the program sidebar to the screen.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void draw()
        {
            char const *head = "MedFlix";
            char const *list = "#185#Home;#186#Favorites;#043#Search;#151#Account;#159#Quit";

            // Update sidebar values
            layout[1].height = GetScreenHeight() - layout[1].y - layout[0].y - 24;

            // Draw the sidebar and retrieve current values
            headerPressed = GuiButton(layout[0], head);
            GuiListView(layout[1], list, &listScrollIdx, &listActive);
        }

        // === sidebar variables ==========================================
        // Internal variables used by sidebar. Public for convenience.
        // Do not edit these unless you know what you are doing.
        // ================================================================
        // In case the window is too small to fit all elements in list,
        // this will change based on how far user scrolls.
        int listScrollIdx;
        // Identifies the currently selected element in the list.
        int listActive;
        // Tells whether the header is pressed or not. (unused)
        bool headerPressed;
        // Rectangle definitions for list and header
        Rectangle layout[2];
    } sidebar;
};
