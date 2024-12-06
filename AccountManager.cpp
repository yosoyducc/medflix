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
// Account management: responsible for login/logout.
// ========================================================================
// ========================================================================


#include "AccountManager.h"
extern "C" {
    #include "raylib.h"     // password hashing functions
}

#include <string>
#include <fstream>

using std::string;

AccountManager::~AccountManager()
{
    // Sign out if necessary
    // perhaps we don't want to save the user.
    //signOut();
}

bool AccountManager::create(char const *u, char const *p)
{
    // Check that the username satisfies basic length conditions
    if (TextLength(u) < 3 || TextLength(p) < 5)
        return false;

    // Get the file name for this user by formatting the username
    // and check that this file doesn't exist by opening it for reading
    char const *uf = TextFormat(USER_DB_PREFIX "%s.ini", u);
    std::ifstream f(uf);
    if (f.is_open())
        return false;

    // Compute the SHA-1 hash of the password
    // Also fix your function signature, raylib!!! grrr >:(
    unsigned int *pSha = ComputeSHA1((unsigned char *)p, TextLength(p));

    // Get the base64 representation of this hashed password
    // notice: pb64 must be MemFree()'d
    int pb64Sz;
    char const *pb64 = EncodeDataBase64((unsigned char const *)pSha, sizeof(unsigned int) * 5, &pb64Sz);

    // Initialize the ini and add the username/password field
    // Adding the username might be somewhat redundant because the
    // origin file is named the same way but I'm adding it anyway.
    // Calling the string constructor for password is necessary because
    // the string returned by EncodeDataBase64() is not 0-terminated
    IniReader user;
    // A brand-new user will have two properties...
    user.addProperty(0, "Username", u);
    user.addProperty(0, "Password", string(pb64, pb64Sz));
    // and five (six including global) sections.
    user.addSection("Favorites");   // Personal favorites list
    user.addSection("Superliked");  // Movies like these will get recommended
    user.addSection("Liked");
    user.addSection("Disliked");
    user.addSection("Watched");     // Watch history

    bool writeStatus = user.write(uf);

    // Free allocated memory and return
    MemFree((void *)pb64);
    return writeStatus;
}

bool AccountManager::signIn(char const *u, char const *p)
{
    // If we are already signed in
    if (signedIn())
        return true;

    // Get the file name for this user and check that it exists
    char const *uf = TextFormat(USER_DB_PREFIX "%s.ini", u);
    std::ifstream f(uf);
    if (!f.is_open())
        return false;
    f.close();

    // Initialize the IniReader type in this class
    user = new IniReader(uf);
    if (user->getSectionCount() < 6 || user->getPropertyCount() < 2) {
        delete user;
        user = nullptr;
        return false;
    }

    // Below: Hands-down the most criminal typecasting ever
    // Compute SHA-1 of this password, decode password in the DB, and compare
    // pSha is the hash of user-entered password, and cSha is comparison hash from user ini
    int cShaSz;
    unsigned int *pSha = ComputeSHA1((unsigned char *)p, TextLength(p));
    unsigned int *cSha = (unsigned int *)DecodeDataBase64((const unsigned char *)(*user)(0, 1).data(), &cShaSz);

    // Check whether the password hashes match
    bool hashMatch = true;
    for (int i = 0; i < 5; ++i) {
        if (pSha[i] != cSha[i]) {
            hashMatch = false;
            delete user;
            user = nullptr;
            break;
        }
    }

    // Free memory of comparison SHA allocated by DecodeDataBase64()
    MemFree(cSha);
    return hashMatch;
}

bool AccountManager::signedIn() const
{
    // User is signed in if this isn't nullptr
    return user != nullptr;
}

bool AccountManager::signOut()
{
    // If we already were signed out
    if (!signedIn())
        return true;

    // Write to disk and check status
    bool writeStatus = user->write();
    if (writeStatus) {
        delete user;
        user = nullptr;
    }   

    return writeStatus;
}

IniReader const *AccountManager::getUserData()
{
    return user;
}
