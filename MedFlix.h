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

    // THE STATE OF AFFAIRS (program variables)
    int exitCode;               // Program exit code
    bool programShouldClose;    // Should the program close?
    bool exitPrompt;            // Display exit prompt

    // Login screen variables
    bool login;

    // Cosmetics
    Color background;

    void fillHash(HashTable*& table) {
        //std::cout<<"HELLOHELLOHELLO\n";
        int part = 0;
        auto &props = db.getProperties();
        auto &sects = db.getSections();
        for(int i = 1; i< db.getSectionCount(); i++) {
            std::vector<std::string> words;
            std::vector<char> letters;
            const std::string& currentName = sects[i];
            //std::cout<<currentName<<std::endl;
            for(int j = 0; j<currentName.length(); j++) {
                if(currentName[j]==' ' || j==currentName.length()-1) {
                    //std::cout << currentName[j];
                    if(j==currentName.length()-1&&isalnum(currentName[j])) {
                        letters.push_back(std::tolower(currentName[j]));
                    }
                    std::string currentWord(letters.begin(), letters.end());
                    bool dup = false;
                    for(auto word : words) {
                        if(word==currentWord) {
                            dup = true;
                        }
                    }
                    if(dup==false) {
                        MovieNode* newMovie= table->set(currentWord,currentName,props[part].second.second,props[part+1].second.second,props[part+2].second.second,
                            props[part+3].second.second,props[part+4].second.second,props[part+5].second.second,props[part+6].second.second,
                            props[part+7].second.second);
                        std::string genre = props[part+3].second.second;
                        if(genre=="Action") {
                            table->Action.push_back(newMovie);
                        } else if(genre=="Drama") {
                            table->Drama.push_back(newMovie);
                        } else if(genre=="Sci-Fi") {
                            table->Scifi.push_back(newMovie);
                        } else if(genre=="Horror") {
                            table->Horror.push_back(newMovie);
                        } else {
                            table->Comedy.push_back(newMovie);
                        }
                    }
                    words.push_back(currentWord);
                    letters.clear();
                } else if(isalnum(currentName[j])){
                    letters.push_back(std::tolower(currentName[j]));
                }
            }
            part+=8;
            words.clear();
        }
    }
};
