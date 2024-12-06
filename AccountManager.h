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
// Declarations for AccountManager class.
// ========================================================================
// ========================================================================


#pragma once

#include "IniReader.h"

// NOTE: this directory MUST exist. Program will always fail
// to create users if it doesn't!
#define USER_DB_PREFIX "./accounts/"

class AccountManager {
public:
    // === AccountManager =================================================
    //
    // ====================================================================
    AccountManager() : user{nullptr} {}
    ~AccountManager();

    // === create =========================================================
    // Given a username and password, creates (registers) a new user.
    // Writes the user account at the file under ./accounts/`username`.ini.
    //
    // Parameters:
    //      desired user name and password
    // Returns:
    //      boolean truth
    // ====================================================================
    static bool create(char const *username, char const *password);

    // === signIn =========================================================
    // Sign in to an account, given a user profile as a file name. Searches
    // for the user with account name under ./accounts/`username`.ini.
    // Returns true if the user profile was signed in successfully.
    //
    // Parameters:
    //      user name and password as a char sequence
    // Returns:
    //      boolean truth
    // ====================================================================
    bool signIn(char const *username, char const *password);

    // === signedIn =======================================================
    // Determine if the user is signed in.
    //
    // Parameters:
    //      none
    // Returns:
    //      boolean truth
    // ====================================================================
    bool signedIn() const;

    // === signOut ========================================================
    // Attempts to sign out the user. Tries first to save user profile to
    // disk, then unloads the user from memory. If anything fails, the user
    // profile will remain in memory.
    //
    // Parameters:
    //      none
    // Returns:
    //      boolean truth
    // ====================================================================
    bool signOut();

    // === getUserData ====================================================
    // Return a pointer to the current user data. This has no regards
    // to whether the user is signed in or not. If user is signed out,
    // this simply returns nullptr.
    //
    // Parameters:
    //      none
    // Returns:
    //      pointer to user data
    // ====================================================================
    IniReader const *getUserData() const;

private:
    IniReader *user;        // Account information (null if signed out)

    enum MovieProperties {
        GLOBL = 0,  // global (user name and password hash)
        FAVES,
        SUPER,      // superliked
        LIKED,
        DISLK,      // disliked
        WTCHD,      // watched
    };
};
