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
void fillHash(HashTable*&, IniReader*&);
// === main ===============================================================
//
// ========================================================================
int main()
{
    IniReader* reader = new IniReader("database.ini");
    HashTable* myHash = new HashTable();
    fillHash(myHash, reader);
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

void fillHash(HashTable*& table, IniReader*& reader) {
    //std::cout<<"HELLOHELLOHELLO\n"<<reader->getSectionCount();
    int part = 0;
    auto &props = reader->getProperties();
    auto &sects = reader->getSections();
    for(int i = 0; i< reader->getSectionCount(); i++) {
        //std::cout<<"HELLOHELLOHELLO\n";
        std::vector<std::string> words;
        std::vector<char> letters;
        const std::string& currentName = sects[i];
        for(int j = 0; j<currentName.length(); j++) {
            if(currentName[j]==' ' || j==currentName.length()-1) {
                if(j==currentName.length()-1) {
                    letters.push_back(currentName[j]);
                }
                std::string currentWord(letters.begin(), letters.end());
                table->set(currentWord,currentName,props[part].second.second,props[part+1].second.second,props[part+2].second.second,
                    props[part+3].second.second,props[part+4].second.second,props[part+5].second.second,props[part+6].second.second,
                    props[part+7].second.second);
                //std::cout<<currentWord<<"\n";
                //std::cout << props[part].second.second<< std::endl;
                letters.clear();
            } else {
                letters.push_back(currentName[j]);
            }
        }
        part+=8;
    }
}
