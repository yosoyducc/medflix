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
#include "IniReader.h"
#include "HashTable.h"

#include <exception>

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
    struct Status {
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
            char const *msg = "MedFlix 0.2.0  |  Copyright © 2024 Aiden Asar & Filip Vracevic. All Rights Reserved.";

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
    struct Sidebar {
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
        //      Account object (for determining sign in status)
        // Returns:
        //      void
        // ================================================================
        void draw(AccountManager const &acct)
        {
            char const *head = "MedFlix";
            char const *list = "#185#Home [^A];#186#Favorites [^E];#043#Search [^F];#169#Movie Info [^V];#151#Account [^M];#159#Quit [ESC]";

            // Calculate offsets for when signed in and out
            int listOffTxt = 0;
            listActiveOff = HOME;
            if (!acct.signedIn()) {
                // Set sign-out offsets
                listOffTxt = 73;
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
    struct Recommend {
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

    // === Favorites panel ================================================
    // Dangerously similar code to Recommend.
    // ====================================================================
    struct Favorites {
        ScreenObjects &p;

        // === init =======================================================
        // Initialize the Favorites panel.
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
        // Draw the favorites panel to the screen.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void draw()
        {
            char const *header = "#186#Favorites";
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

        // === favorites variables ========================================
        // Internal variables used by favorites panel.
        // ================================================================
        // Anchor for the favorites panel
        Vector2 anchor;

        // Movies panels variables
        Rectangle panelScrollView;
        Vector2 panelScrollOffset;
        Vector2 panelBoundsOffset;
        bool refreshPressed;

        // Rectangle definitions
        Rectangle layout[4];
    } favorites{*this};

    // === Search panel (Filmhunter) ======================================
    //
    // ====================================================================
    struct Search {
        ScreenObjects &p;

        // === init =======================================================
        // Initialize the search panel.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void init()
        {
            anchor = { 160, 8 };

            // Initialize controls
            panelScrollOffset = { 0, 0 };
            entryEditMode = false;
            entryText[0] = 0x0;
            buttonPressed = false;

            // Initialize layout rectangles
            // back drop panel
            layout[0] = { anchor.x, anchor.y, 544, 400 };
            // scroll panel
            layout[1] = { anchor.x + 8, anchor.y + 88, 528, 304 };
            // group outline
            layout[2] = { anchor.x + 8, anchor.y + 16, 528, 376 };
            // hint label
            layout[3] = { anchor.x + 40, anchor.y + 40, 216, 32 };
            // entry box
            layout[4] = { anchor.x + 32, anchor.y + 40, 432, 32 };
            // search button
            layout[5] = { anchor.x + 480, anchor.y + 40, 32, 32 };
        }

        // === draw =======================================================
        // Draw the objects to the screen.
        //
        // Parameters:
        //      IniReader movie database, user account
        // Returns:
        //      void
        // ================================================================
        void draw(IniReader const &db, AccountManager const &acct)
        {
            // Check if user hit cool keys
            if (IsKeyPressed(KEY_TAB))
                entryEditMode = true;
            if (IsKeyPressed(KEY_ENTER))
                entryEditMode = false;

            char const *groupText = "#064#Filmhunter";
            char const *menacing  = "Search for your next victim...";
            char const *search    = "#043#";

            // Update controls variables
            int w = GetScreenWidth() - anchor.x;
            int h = GetScreenHeight() - anchor.y;
            // Back panel
            layout[0].width  = w - 8;
            layout[0].height = h - p.status.layout.height - 8;
            // Scroll panel (precalculated, can't be bothered with dynamics)
            layout[1].width  = w - 24;
            layout[1].height = h - p.status.layout.height - 104;
            // Group outline follows scroll panel
            layout[2].width  = layout[1].width;
            layout[2].height = layout[1].height + 72;
            // Hint label is infinitely static.
            // Entry box expands with screen width
            layout[4].width = layout[2].width - layout[5].width - 64;
            // Search button is offset from layout[4] by 16
            layout[5].x     = layout[4].x + layout[4].width + 16;

            // Variables for drawing
            // TEMPORARY: get results from search instead...
            Rectangle view;
            static std::vector<std::string> const names = {
                "the land before time",
                "the last unicorn",
                "the iron giant",
                "knives out",
                "space jam",
                "citizen kane",
                "city lights",
                "oppenheimer",
                "2001: a space odyssey",
                "ghostbusters",
                "the girl who leapt through time",
                "the powerpuff girls movie",
                "the matrix",
                "beverly hills cop",
                "jurassic park",
                "jojo rabbit",
                "pokémon: detective pikachu"
            };
            int contentHeight = names.size() * 48;

            // Render elements to the screen
            GuiDummyRec(layout[0], NULL);
            GuiScrollPanel(layout[1], NULL, (Rectangle){ layout[1].x, layout[1].y, layout[1].width - 15, (float)contentHeight }, &panelScrollOffset, &view);
            GuiLine((Rectangle){layout[1].x, layout[1].y, layout[1].width, 0}, NULL);
            GuiGroupBox(layout[2], groupText);
            // Only display hint if the user hasn't entered any text yet
            if (!entryText[0])
                GuiLabel(layout[3], menacing);
            if (GuiTextBox(layout[4], entryText, 128, entryEditMode))
                entryEditMode = !entryEditMode;
            buttonPressed = GuiButton(layout[5], search);

            // TODO: get data from search results instead.
            BeginScissorMode(view.x, view.y, view.width, view.height);
                for (int i = 0; i < names.size(); ++i) {
                    int s = db.findSection(names[i]);
                    auto &n = db.getSectionName(s);
                    int p = db.findProperty(s, "year");
                    auto &v = db(s, p);
                    GuiLabelButton((Rectangle){ anchor.x + 32, anchor.y + 88 + (i * 48) + panelScrollOffset.y, layout[4].width, 48 }, TextFormat("%s (%s)", n.data(), v.data()));
                    GuiLine((Rectangle){ anchor.x + 24, anchor.y + 128 + (i * 48) + panelScrollOffset.y, layout[4].width + 32, 16 }, NULL);
                }
            EndScissorMode();
        }

        // === Search panel variables =====================================
        // Internal variables used by search panel.
        // ================================================================
        Vector2 anchor;

        // Controls variables
        Vector2 panelScrollOffset;

        bool entryEditMode;
        char entryText[128];
        bool buttonPressed;

        // TODO: add dynamic booleans
        bool *selection;

        // Layout rectangles
        Rectangle layout[6];
    } search{*this};

    // === Movie info panel ===============================================
    //
    // ====================================================================
    struct MovieInfo {
        ScreenObjects &p;

        // === init =======================================================
        // Initialize the movie viewing panel.
        //
        // Parameters:
        //      Nothing
        // Returns:
        //      void
        // ================================================================
        void init()
        {
            // Anchors
            lAnchor = { 160, 8 };
            rAnchor = { 580, 0 };

            // Controls
            for (int i = 0; i < 3; ++i)
                current[i]      = false;
            toggleFaveActive    = false;
            toggleWatchedActive = false;

            for (int i = 0; i < 5; ++i)
                previous[i]     = false;

            // Poster, name, basic info, and divider
            layout[0] = { lAnchor.x, lAnchor.y + 56, 400, 480 };
            layout[1] = { lAnchor.x + 24, lAnchor.y, 168, 48 };
            layout[2] = { lAnchor.x + 216, lAnchor.y + 8, 176, 32 };
            layout[3] = { lAnchor.x + 16, lAnchor.y + 40, 192, 16 };
            // Detail group
            layout[4] = { rAnchor.x, rAnchor.y + 64, 360, 448 };
            // Tags: director, genre, imdb, actions, and description
            layout[5] = { rAnchor.x + 24, rAnchor.y + 80, 112, 24 };
            layout[6] = { rAnchor.x + 232, rAnchor.y + 80, 112, 24 };
            layout[7] = { rAnchor.x + 232, rAnchor.y + 144, 112, 24 };
            layout[8] = { rAnchor.x + 24, rAnchor.y + 216, 120, 24 };
            layout[9] = { rAnchor.x + 24, rAnchor.y + 304, 120, 24 };
            // Dividers for the above, in that order
            layout[10] = { rAnchor.x + 16, rAnchor.y + 96, 120, 16 };
            layout[11] = { rAnchor.x + 224, rAnchor.y + 96, 120, 16 };
            layout[12] = { rAnchor.x + 224, rAnchor.y + 160, 120, 16 };
            layout[13] = { rAnchor.x + 16, rAnchor.y + 232, 120, 16 };
            layout[14] = { rAnchor.x + 16, rAnchor.y + 320, 120, 16 };
            // Director labels (up to four directors)
            layout[15] = { rAnchor.x + 32, rAnchor.y + 112, 160, 24 };
            layout[16] = { rAnchor.x + 32, rAnchor.y + 136, 160, 24 };
            layout[17] = { rAnchor.x + 32, rAnchor.y + 160, 160, 24 };
            layout[18] = { rAnchor.x + 32, rAnchor.y + 184, 160, 24 };
            // Genre and rating labels
            layout[19] = { rAnchor.x + 240, rAnchor.y + 112, 120, 24 };
            layout[20] = { rAnchor.x + 240, rAnchor.y + 176, 72, 24 };
            // Toggles: superlike, like, loathe, fave, and watched
            layout[21] = { rAnchor.x + 40, rAnchor.y + 256, 32, 32 };
            layout[22] = { rAnchor.x + 88, rAnchor.y + 256, 32, 32 };
            layout[23] = { rAnchor.x + 136, rAnchor.y + 256, 32, 32 };
            layout[24] = { rAnchor.x + 208, rAnchor.y + 240, 112, 24 };
            layout[25] = { rAnchor.x + 208, rAnchor.y + 272, 112, 24 };
            // Finally, the film description
            layout[26] = { rAnchor.x + 24, rAnchor.y + 336, 304, 176 };

            // Nullify the internal values at startup
            unload();
        }

        // === load =======================================================
        // For a Movie Hash node and user account, set some internal
        // references to reflect this movie.
        //
        // Parameters:
        //      MovieNode *, AccountManager &acct
        // Returns:
        //      void
        // ================================================================
        void load(MovieNode const *movie, AccountManager const &acct)
        {
            // If null is passed or user isn't signed in
            if (movie == nullptr || !acct.signedIn())
                return;

            // Populate pointer values with addresses of internal
            // strings from movies database ini
            name = movie->name.data();
            year = movie->year.data();
            rating = movie->rating.data();
            runtime = movie->runtime.data();
            genre = movie->genre.data();
            int textCount;
            char const **text = TextSplit(movie->director.data(), ',', &textCount);
            // We cap textCount at four and copy strings
            if (textCount > 4)
                textCount = 4;
            for (int i = 0; i < textCount; ++i) {
                int textSz = TextLength(text[i]);
                director[i] = (char *)MemAlloc(textSz + 1);
                TextCopy(director[i], text[i]);
                director[i][textSz] = 0x0;
            }
            descript = movie->descript.data();
            imdb = movie->imdb.data();
            poster = movie->imdb.data();

            // Calculate movie name dimensions and add offset to name and basic info
            int fontSize = GuiGetStyle(DEFAULT, TEXT_SIZE);
            int spacing = GuiGetStyle(DEFAULT, TEXT_SPACING);
            namePx = MeasureTextEx(GuiGetFont(), name, fontSize * 2.0f, spacing);
            layout[1].width = namePx.x;
            if (layout[1].width > 560)
                layout[1].width = 560;      // cap width of movie name (BORAT!!!)
            // Including left padding for basic info
            layout[2].x = layout[1].x + layout[1].width + 24;

            // Populate toggle variables with preferences from user ini
            try {
                IniReader const &ini = *acct.getUserData();
                int like = std::stoi(ini(name, "Like"));
                int fave = std::stoi(ini(name, "Favorite"));
                int wtch = std::stoi(ini(name, "Watched"));

                switch (like) {
                    case 2:  previous[0] = current[0] = true; break;
                    case 1:  previous[1] = current[1] = true; break;
                    case -1: previous[2] = current[2] = true; break;
                }
                if (fave) previous[3] = toggleFaveActive = true;
                if (wtch) previous[4] = toggleWatchedActive = true;
            } catch (std::exception const &e) {
                TraceLog(LOG_WARNING, "No configuration for this movie for user");
            }
        }

        // === unload =====================================================
        // Nullify/falsify movie-related pointer internal variables.
        //
        // Parameters:
        //      none
        // Returns:
        //      void
        // ================================================================
        void unload()
        {
            // Nullify pointer-to-ini-values variables
            name        =
            year        =
            rating      =
            runtime     =
            genre       =
            descript    =
            imdb        =
            poster      = nullptr;

            // Delete memory allocated for director strings
            for (int i = 0; i < 4; ++i) {
                if (director[i]) {
                    MemFree(director[i]);
                    director[i] = nullptr;
                }
            }

            // Reset state of button toggles to unselected
            for (int i = 0; i < 3; ++i)
                current[i]      = false;
            toggleFaveActive    = false;
            toggleWatchedActive = false;

            for (int i = 0; i < 5; ++i)
                previous[i]     = false;

            // No movie name, so no movie name dimensions either
            namePx = (Vector2){ 0, 0 };
        }

        // === draw =======================================================
        // Render the movie panel to the screen. The reason parameter isn't
        // const is because this draw function modifies the internal ini
        // of the AccountManager object passed in.
        //
        // Parameters:
        //      AccountManager reference
        // Returns:
        //      void
        // ================================================================
        void draw(AccountManager &acct)
        {
            // Keep track of whether any action values change
            bool isDelta = false;

            // Disable rating if movie name doesn't exist (screen unloaded)
            if (name == nullptr)
                GuiDisable();

            // Constant text fields
            char const *posText = "Movie poster area";
            char const *detail  = "#010#Details";       // Paper with text
            char const *tagDir  = "DIRECTOR";
            char const *tagGen  = "GENRE";
            char const *tagImdb = "IMDb";
            char const *tagAct  = "ACTIONS";
            char const *tagDesc = "DESCRIPTION";
            char const *super   = "#157#";              // Star
            char const *suTip   = "SUPER AWESOME!";
            char const *like    = "#148#";              // 1-UP
            char const *liTip   = "I like this";
            char const *loathe  = "#152#";              // Skull
            char const *loTip   = "goofy aah movie";
            char const *fave    = "#186#Favorite";      // Heart
            char const *watch   =                       // Eye
                toggleWatchedActive ? "#044#Watched" : "#045#Unwatched";

            // Get default fontstyle properties
            int const fontSize  = GuiGetStyle(DEFAULT, TEXT_SIZE);
            int const lineSpace = GuiGetStyle(DEFAULT, TEXT_LINE_SPACING);

            // Draw the left side (poster and basic info)
            //GuiDummyRec(layout[0], posText);
            GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize * 2);
            GuiLabel(layout[1], name);
            GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
            GuiLabel(layout[2], info);
            GuiLine(layout[3], NULL);
            // Draw the right side (everything else)
            GuiGroupBox(layout[4], detail);
            GuiLabel(layout[5], tagDir);
            GuiLabel(layout[6], tagGen);
            GuiLabel(layout[7], tagImdb);
            GuiLabel(layout[8], tagAct);
            GuiLabel(layout[9], tagDesc);
            GuiLine(layout[10], NULL);
            GuiLine(layout[11], NULL);
            GuiLine(layout[12], NULL);
            GuiLine(layout[13], NULL);
            GuiLine(layout[14], NULL);
            GuiLabel(layout[15], director[0]);
            GuiLabel(layout[16], director[1]);
            GuiLabel(layout[17], director[2]);
            GuiLabel(layout[18], director[3]);
            GuiLabel(layout[19], genre);
            GuiLabel(layout[20], imdb);
            // We render favorite/watched before the user rating because
            // the tooltip for loathe gets rendered under this which is ugly
            GuiToggle(layout[24], fave, &toggleFaveActive);
            GuiToggle(layout[25], watch, &toggleWatchedActive);
            // Enable tooltips for just these next three toggles
            GuiEnableTooltip();
            GuiSetTooltip(suTip);
            GuiToggle(layout[21], super, &current[0]);
            GuiSetTooltip(liTip);
            GuiToggle(layout[22], like, &current[1]);
            GuiSetTooltip(loTip);
            GuiToggle(layout[23], loathe, &current[2]);
            // Disable tooltips again
            GuiSetTooltip(NULL);
            GuiDisableTooltip();
            // Draw the description with wrapping.
            GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);
            GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);
            GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, lineSpace - 4);
            // Render the text
            GuiLabel(layout[26], descript);
            // Reset GUI styles
            GuiSetStyle(DEFAULT, TEXT_LINE_SPACING, lineSpace);
            GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_NONE);
            GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);

            // Same check at function exit for nonexistent movie name
            // (we don't want to disable the entire program)
            if (name == nullptr)
                GuiEnable();

            // Check for differences and keep exclusivity for first three
            for (int i = 0; i < 3; ++i) {
                if (current[i] != previous[i]) {
                    isDelta = true;     // There HAS been a change!
                    // Reset values for others that aren't selected
                    for (int j = 0; j < 3; ++j) {
                        if (j != i)
                            previous[j] = current[j] = false;
                    }
                    previous[i] = current[i];
                    // We don't need to check after first change, break
                    break;
                }
            }
            // Special case: those last two
            if (toggleFaveActive != previous[3] || toggleWatchedActive != previous[4]) {
                isDelta = true;
                previous[3] = toggleFaveActive;
                previous[4] = toggleWatchedActive;
            }

            // Update the user ini if and only if there was a change
            if (isDelta) {
                // There was a delta change.
                // Update the user ini with new values, casting away constness.
                IniReader *ini = const_cast<IniReader *>(acct.getUserData());

                // Get movie section index in user database, and
                // determine if it exists.
                int si = ini->findSection(name);
                bool siExists = si > 0;     // don't count the global section

                // Determine if there's any truths among the booleans
                // -> we need a section, otherwise we don't.
                bool isTruth = false;
                for (int i = 0; i < 4; ++i) {
                    if (i != 3 && current[i]) {
                        isTruth = true;
                        break;
                    } else if (toggleFaveActive || toggleWatchedActive) {
                        isTruth = true;
                    }
                }

                // Determine action with the section, based on above.
                if (!isTruth) {     // i used to float*, now i just delete;
                    // dropSection already accounts for invalid sections
                    ini->dropSection(si);
                } else {            // TRUTH. LET ME TELL YOU HOW TRULY I'VE...
                    if (!siExists) {
                        // Add section and set section index var to result
                        si = ini->addSection(name);
                        // Add the empty properties
                        ini->addProperty(si, "Like", "");
                        ini->addProperty(si, "Favorite", "");
                        ini->addProperty(si, "Watched", "");
                    }
                    // Edit the values based on current bools.
                    // This could be slightly optimized. Because these
                    // properties are always guaranteed to be sequential
                    // (i.e. in order), we can avoid using the time-complex
                    // setPropertyValue function and edit the IniProperties
                    // vector directly instead.
                    ini->setPropertyValue(si, 0,
                        current[0] ? "2" :          // superlike
                        current[1] ? "1" :          // like
                        current[2] ? "-1" : "0");   // loathe or no rating
                    ini->setPropertyValue(si, 1,
                        toggleFaveActive ? "1" : "0");      // favorite or not
                    ini->setPropertyValue(si, 2,
                        toggleWatchedActive ? "1" : "0");   // watched or not
                }
            }
        }

        // === Movie panel variables ======================================
        // Internal variables used by movie panel.
        // ================================================================
        Vector2 lAnchor;        // Left anchor, for poster
        Vector2 rAnchor;        // Right anchor, for details and actions

        // The first three toggles are for superlike, like, and loathe,
        // and must be mutially exclusive. The other two, favorite and
        // watched, do not.
        bool current[3];
        bool toggleFaveActive;
        bool toggleWatchedActive;
        // Keep track of last boolean values as well
        bool previous[5];

        // Define layout rectangles
        Rectangle layout[27];

        // Variable text fields
        char const *name;
        Vector2     namePx;     // dimensions of movie name (width, height)
        char const *info;
        char const *year;
        char const *rating;
        char const *runtime;
        char       *director[4];    // Not const because we copy
        char const *genre;
        char const *imdb;
        char const *descript;
        char const *poster;
    } movie{*this};

    // === Account panel ==================================================
    //
    // ====================================================================
    struct Account {
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
        //      Account object (determine sign in status)
        // Returns:
        //      void
        // ================================================================
        void draw(AccountManager const &acct)
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

            // Surfing on a keyboard!
            if (IsKeyPressed(KEY_TAB)) {
                if (entryUserEditMode) {
                    entryUserEditMode = false;
                    entryPassEditMode = true;
                } else if (entryPassEditMode) {
                    entryUserEditMode = true;
                    entryPassEditMode = false;
                } else {
                    entryUserEditMode = true;
                }
            }
            if (IsKeyPressed(KEY_ENTER))
                butSigninPressed = true;

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
