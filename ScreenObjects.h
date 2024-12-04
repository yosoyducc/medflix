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
    #include "raylib.h"
    // Make sure the implementation isn't defined twice
    #undef RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

class ScreenObjects {
public:
    // === Statusbar object ===============================================
    //
    // ====================================================================
    struct {
        // === init =======================================================
        // Initialize the statusbar.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void init()
        {
            layout = { 0, 0, 0, 24 };
        }

        // === draw =======================================================
        // Update statusbar internal variables and render the statusbar
        // to the screen.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void draw()
        {
            char const *msg = "MedFlix 0.1.0  |  Copyright © 2024 Aiden Asar & Filip Vracevic. All Rights Reserved.";

            // Update dimensions
            layout.y = GetScreenHeight() - layout.height;
            layout.width = GetScreenWidth();

            // Draw the status bar
            GuiStatusBar(layout, msg);
        }

        // === statusbar variables ========================================
        // Internal variables used by statusbar. Public for convenience.
        // Do not edit these anywhere unless you know what you are doing.
        // ================================================================
        Rectangle layout;
    } status;

    // === Sidebar object =================================================
    //
    // ====================================================================
    struct {
        // Holds reference to the parent.
        // For accessing all objects in this class.
        ScreenObjects &p;

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
            layout[1].height = GetScreenHeight() - layout[1].y - layout[0].y - p.status.layout.height;

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
    } sidebar{*this};

    // === Recommended panel ==============================================
    //
    // ====================================================================
    struct {
        // === init =======================================================
        // Initialize the recommended panel.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void init()
        {
            // Set anchor position
            anchor = { 168, 8 };

            // Panel stuff
            panelScrollView = { 0 };
            panelScrollOffset = { 0 };
            panelBoundsOffset = { 0 };

            // Default boundaries
            // Backdrop
            layout[0] = { anchor.x, anchor.y, 256, 256 };
            // Header
            layout[1] = { anchor.x + 16, anchor.y + 8, 224, 16 };
            // Scroll panel
            layout[2] = { anchor.x + 8, anchor.y + 32, 240, 216 };
        }

        void draw()
        {
            char const *header = "#157#Recommended";

            // Update the backdrop and panel widths/heights
            int w = GetScreenWidth() - anchor.x;
            int h = GetScreenHeight() - anchor.y;

            layout[0] = { anchor.x, anchor.y, w - 8.0f, h - 8.0f - 24.0f };
            layout[1] = { anchor.x + 16, anchor.y + 8, w - (256 - 224.0f), layout[1].height };
            //layout[2] = { anchor.x, anchor.y, w - (256 - 240.0f), h - (256 - 216.0f) };

            // Draw the panel backdrop
            GuiDummyRec(layout[0], nullptr);
            GuiLine(layout[1], header);
            GuiScrollPanel(layout[2], nullptr, layout[2], &panelScrollOffset, &panelScrollView);
        }

        // === recommend variables ========================================
        // Internal variables used by recoomend panel.
        // ================================================================
        // Anchor for the recommended panel
        Vector2 anchor;

        // Movies panels variables
        Rectangle panelScrollView;
        Vector2 panelScrollOffset;
        Vector2 panelBoundsOffset;

        // Rectangle definitions
        Rectangle layout[3];
    } recommend;
};
