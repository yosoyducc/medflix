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

#include "AccountManager.h"     // determine if we're signed in or not

// Include raygui/raylib as C library
extern "C" {
    #include "raylib.h"
    // Make sure the implementation isn't defined twice
    #undef RAYGUI_IMPLEMENTATION
    #include "raygui.h"
}

class ScreenObjects {
public:
    // Let me preface by saying this: Clang is a symptom.
    // Originally this enum was under the sidebar struct, while referencing
    // these values as so.sidebar.MEMBER in the switch in ::render(). Clang
    // was whining about it and said "Case value is not a constant expression".
    // OK you win Clang, you want to be *extra* sure that enum members are
    // indeed compile-time constants. Here you go, you drama queen.
    enum SidebarOptions {
        HOME = 0,
        MY_LISTS,
        SEARCH,
        MOVIE_INFO,
        ACCOUNT,
        QUIT
    };

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
            listActive = HOME;

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
        void draw(AccountManager const &acct)
        {
            char const *head = "MedFlix";
            char const *list = "#185#Home;#186#Favorites;#043#Search;#169#Movie Info;#151#Account;#159#Quit";

            // Calculate offsets for when signed in and out
            int listOffTxt = 0;
            listActiveOff = HOME;
            if (!acct.signedIn()) {
                // Set sign-out offsets
                listOffTxt = 53;
                listActiveOff = ACCOUNT;
            }

            // Update sidebar values
            layout[1].height = GetScreenHeight() - layout[1].y - layout[0].y - p.status.layout.height;

            // Draw the sidebar and retrieve current values
            headerPressed = GuiButton(layout[0], head);
            GuiListView(layout[1], list + listOffTxt, &listScrollIdx, &listActive);
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
        // Determines the current offset for the list. (used for logged out)
        int listActiveOff;
        // Tells whether the header is pressed or not. (unused)
        bool headerPressed;
        // Rectangle definitions for list and header
        Rectangle layout[2];
    } sidebar{*this};

    // === Recommended panel ==============================================
    //
    // ====================================================================
    struct {
        ScreenObjects &p;

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
            anchor = { 160, 8 };

            // Panel stuff
            panelScrollView = { 0 };
            panelScrollOffset = { 0 };
            panelBoundsOffset = { 0 };
            refreshPressed = false;

            // Default boundaries
            // Backdrop
            layout[0] = { anchor.x, anchor.y, 256, 256 };
            // Header
            layout[1] = { anchor.x + 16, anchor.y + 8, 192, 24 };
            // Scroll panel
            layout[2] = { anchor.x + 8, anchor.y + 40, 240, 208 };
            // Button
            layout[3] = { anchor.x + 216, anchor.y + 8, 24, 24 };
        }

        // === draw =======================================================
        // Draw the recommended panel to the screen.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void draw()
        {
            char const *header = "#157#Recommended";
            char const *button = "#211#";

            // Update the backdrop and panel widths/heights
            int w = GetScreenWidth() - anchor.x;
            int h = GetScreenHeight() - anchor.y;

            // Update back panel width/height
            layout[0].width  = w - 8;
            layout[0].height = h - p.status.layout.height - 8;
            // Update header width: subtract button width and combined padding
            layout[1].width  = w - layout[3].width - 48;
            // Update scroll panel dimensions, padding pre-calculated
            layout[2].width  = w - 24;
            layout[2].height = h - layout[1].height - p.status.layout.height - 32;
            // Update button position on the x-axis
            layout[3].x      = anchor.x + layout[1].width + 24;

            // Draw the panel backdrop
            GuiDummyRec(layout[0], nullptr);
            GuiLine(layout[1], header);
            GuiScrollPanel(layout[2], nullptr, layout[2], &panelScrollOffset, &panelScrollView);
            refreshPressed = GuiButton(layout[3], button);
        }

        // === recommend variables ========================================
        // Internal variables used by recommend panel.
        // ================================================================
        // Anchor for the recommended panel
        Vector2 anchor;

        // Movies panels variables
        Rectangle panelScrollView;
        Vector2 panelScrollOffset;
        Vector2 panelBoundsOffset;
        bool refreshPressed;

        // Rectangle definitions
        Rectangle layout[4];
    } recommend{*this};

    // === Account panel ==================================================
    //
    // ====================================================================
    struct {
        ScreenObjects &p;

        // === init =======================================================
        // Initialize the accouts registration/login/logout screen.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void init()
        {
            anchor = { 160, 8 };

            dropActionEditMode = false;
            dropActionActive = 0;           // sign in is the default mode

            entryUserEditMode = false;
            entryUserText[0] = 0x0;

            entryPassEditMode = false;
            entryPassText[0] = 0x0;

            butSigninPressed = false;

            labelTimeout = 0.0f;

            // Account management window dimensions
            layout[0] = { anchor.x, anchor.y, 312, 216 };
            // Sign-in/registration dropdown
            layout[1] = { anchor.x + 216, anchor.y, 96, 24 };
            // Sign in label text
            layout[2] = { anchor.x + 24, anchor.y + 32, 264, 24 };
            // "Account info" group
            layout[3] = { anchor.x + 8, anchor.y + 72, 296, 88 };
            // "Username:" label
            layout[4] = { anchor.x + 24, anchor.y + 88, 80, 24 };
            // username entry box
            layout[5] = { anchor.x + 112, anchor.y + 88, 176, 24 };
            // "Password:" label
            layout[6] = { anchor.x + 24, anchor.y + 120, 80, 24 };
            // password entry
            layout[7] = { anchor.x + 112, anchor.y + 120, 176, 24 };
            // sign me in button!
            layout[8] = { anchor.x + 176, anchor.y + 176, 120, 24 };
            // get hint button label
            layout[9] = { anchor.x + 112, anchor.y + 176, 56, 24 };
        }

        // === draw =======================================================
        // Render the account panel to the screen, based on login status.
        //
        // Parameters:
        //      Account type
        // Returns:
        //      void
        // ================================================================
        void draw(/*Account acct*/)
        {
            if (dropActionEditMode || showHintBox) GuiLock();

            char const *panelTitle  = "#137#Account Manager";
            char const *labelSignin = dropActionActive ? "New account creation" : "Please sign in.";
            char const *acctInfo    = dropActionActive ? "Account registration" : "Account info";
            char const *username    = "Username:";
            char const *password    = "Password:";
            char const *signMeIn    = dropActionActive ? "Register me!" : "Sign me in!";
            char const *dropdown    = "SIGN IN;REGISTER";
            char const *labelHint   = "Get hint";

            // Check if there's an active label message
            if (labelTimeout > 0) {
                switch (labelMsgCode) {
                    case SIGN_IN_FAIL:
                        labelSignin = "Auth failed! Bad user/pass?";
                        break;
                    case SIGN_OUT_PASS:
                        labelSignin = "Successfully signed out.";
                        break;
                    case SIGN_OUT_FAIL:
                        labelSignin = "Failed to save user data!";
                        break;
                    case REGISTRY_PASS:
                        labelSignin = "Registration successful!";
                        break;
                    case REGISTRY_FAIL:
                        labelSignin = "Failed to register account!";
                        break;
                    default:
                        labelSignin = "Unknown path was encountered";
                }
                labelTimeout -= GetFrameTime();
            }

            // Update anchor position and all attached elements
            // We need to add sidebar width and statusbar halved to offset to viewport's center
            anchor.x =
                GetScreenWidth() / 2.0f
                - layout[0].width / 2.0f
                + p.sidebar.layout[0].width / 2.0f;
            anchor.y =
                GetScreenHeight() / 2.0f
                - layout[0].height / 2.0f
                - p.status.layout.height / 2.0f;

            layout[0].x = anchor.x;
            layout[0].y = anchor.y;
            layout[1].x = anchor.x + 216;
            layout[1].y = anchor.y;
            layout[2].x = anchor.x + 24;
            layout[2].y = anchor.y + 32;
            layout[3].x = anchor.x + 8;
            layout[3].y = anchor.y + 72;
            layout[4].x = anchor.x + 24;
            layout[4].y = anchor.y + 88;
            layout[5].x = anchor.x + 112;
            layout[5].y = anchor.y + 88;
            layout[6].x = anchor.x + 24;
            layout[6].y = anchor.y + 120;
            layout[7].x = anchor.x + 112;
            layout[7].y = anchor.y + 120;
            layout[8].x = anchor.x + 176;
            layout[8].y = anchor.y + 176;
            if (showButHint) {
                layout[9].x = anchor.x + 112;
                layout[9].y = anchor.y + 176;
            }

            // Render the gui to the screen.
            GuiPanel(layout[0], panelTitle);
            GuiLabel(layout[2], labelSignin);
            GuiGroupBox(layout[3], acctInfo);
            GuiLabel(layout[4], username);
            if (GuiTextBox(layout[5], entryUserText, 16, entryUserEditMode))
                entryUserEditMode = !entryUserEditMode;
            GuiLabel( layout[6], password);
            if (GuiTextBox(layout[7], entryPassText, 32, entryPassEditMode))
                entryPassEditMode = !entryPassEditMode;
            butSigninPressed = GuiButton(layout[8], signMeIn);
            if (showButHint)
                butHintPressed = GuiLabelButton(layout[9], labelHint);
            if (GuiDropdownBox(layout[1], dropdown, &dropActionActive, dropActionEditMode))
                dropActionEditMode = !dropActionEditMode;

            // Render the hints
            if (butHintPressed)
                showHintBox = true;
            if (showHintBox) {
                GuiUnlock();

                // calculate boundaries of the box
                float hwidth  = GetScreenWidth() / 2.0f,
                      hheight = GetScreenHeight() / 2.0f;
                Rectangle bounds = { hwidth - 200, hheight - 75, 400, 150 };
                int choice = GuiMessageBox(bounds, "#193#Hint", "Username must have at least 3 characters,\npassword must have at least 5 characters.", "I understand");

                if (choice >= 0) {
                    showHintBox = false;
                    showButHint = false;
                }

                GuiLock();
            }

            // Get around some annoying bug where focus is kept on
            // entry box even after hitting enter
            if (IsKeyPressed(KEY_ENTER))
                entryUserEditMode = entryPassEditMode = false;

            GuiUnlock();
        }

        // === setLabel ===================================================
        // Set the label message via code for the account management panel.
        // This message will last for 3.5 seconds.
        //
        // Parameters:
        //      int error code (try the enum!)
        // Returns:
        //      void
        // ================================================================
        void setLabelMessage(int code)
        {
            // Set the error timeout.
            labelTimeout = 3.5f;
            // And the message code.
            labelMsgCode = code;
        }

        // === account panel variables ====================================
        // Internal variables used by account panel.
        // ================================================================
        Vector2 anchor;

        bool dropActionEditMode;
        int dropActionActive;

        bool entryUserEditMode;
        char entryUserText[16];

        bool entryPassEditMode;
        char entryPassText[32];

        bool butSigninPressed;
        bool showButHint;
        bool showHintBox;
        bool butHintPressed;

        float labelTimeout;
        int labelMsgCode;
        enum AuthMessage {
            // no need for success sign in message
            SIGN_IN_FAIL,
            SIGN_OUT_PASS,
            SIGN_OUT_FAIL,
            REGISTRY_PASS,
            REGISTRY_FAIL,
        };

        Rectangle layout[10];
    } account{*this};
};
