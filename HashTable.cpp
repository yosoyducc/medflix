// Attached: Lab #13
// File: HashTable.cpp
// =============================================================================
//
// Programmers: Aiden Asar, Filip Vracevic
// Class: CS 1D
// Instructor: Med Mogasemi
//
// =============================================================================
// Program: MedFlix (Movie tracking system) (lab #13)
// =============================================================================
// Hashmap for the movies properties.
// =============================================================================
// =============================================================================


#include "HashTable.h"
#include <algorithm>
#include <fstream>


using std::string;

/**** Node Class ****/

MovieNode::MovieNode(string const &n, string const &y, string const &ra,
                     string const &ru, string const &g, string const &dr,
                     string const &de, string const &i, string const &p)
: name{n}, year{y}, rating{ra}, runtime{ru}, genre{g},
  director{dr}, descript{de}, imdb{i}, poster{p}, next{nullptr}
{ }

// Below code does NOT work yet.

/// **** HashTable Class **** /

// ==== hash ==============================================================
// This number is used as the index for how the teachers and their
// information will be accessed.
//
// Input:
// const string
//
// Output:
// the index
// =============================================================================
int HashTable::hash(string const &key) const
{
    int hash{};
    for (int i = 0; i < key.length(); ++i) {
        int asciiValue = hash + key[i];
        hash = (hash + asciiValue * 23) % SIZE;
    }
    //std::cout<<hash<<std::endl;
    return hash;
}

//this destroys the linked list of the has table
HashTable::~HashTable()
{
    for (int i = 0; i < SIZE; ++i) {
        MovieNode* head = dataMap[i];
        MovieNode* temp = head;

        while (head) {
            head = head->next;
            delete temp;
            temp = head;
        }
    }
}

// ==== set ==============================================================
// this adds a teacher and their information to the linked list, and
// address in the linked list to the datamap array
//
// Input:
// string, prof_data
//
// Output:
//
// =============================================================================
MovieNode* HashTable::set(string const &hashname,  string const &name1,
                          string const &year1,     string const &rating1,
                          string const &runtime1,  string const &genre1,
                          string const &director1, string const &descript1,
                          string const &imdb1,     string const &poster1) {
    int index = hash(hashname);

    MovieNode* newNode = new MovieNode(name1, year1,rating1,runtime1,genre1,director1,descript1,imdb1,poster1);

    if (dataMap[index] == nullptr){
        dataMap[index] = newNode;
    } else {
        MovieNode* temp = dataMap[index];
        while (temp->next != nullptr)
            temp = temp->next;

        temp->next = newNode;
    }
    return newNode;
}

std::vector<MovieNode *> HashTable::search(string const &movieSearch) const {
    //const string& currentName = sects[i];
    //std::cout<<currentName<<std::endl;
    std::vector<char> letters;
    std::vector<string> words;
    std::vector<MovieNode*> results;

    // If string is empty, return nothing
    // removes non alphanumeric characters
    //converts letters to lowercase
    if (movieSearch.empty())
        return results;
    for(int j = 0; j<movieSearch.length(); j++) {
        if(movieSearch[j]==' ' || j==movieSearch.length()-1) {
            if(j==movieSearch.length()-1&&isalnum(movieSearch[j])) {
                letters.push_back(std::tolower(movieSearch[j]));
            }
            string word(letters.begin(), letters.end());
            words.push_back(word);
            letters.clear();
        } else if(isalnum(movieSearch[j])) {
            letters.push_back(std::tolower(movieSearch[j]));
        }
    }
    /*for(int i = 0; i<words.size(); i++) {
        std::cout<<words[i]<<std::endl;
    }*/
    //only the first word is hashed
    //the rest of the letters or anything are used to narrow results
    int index = hash(words[0]);
    //std::transform(movieSearch.begin(), movieSearch.end(), movieSearch.begin(), ::toupper);
    MovieNode* temp = dataMap[index];
    while (temp != nullptr){
        bool good = true;
        std::vector<char> movieNameVec;
        for(auto c: temp->name) {
            if(isalnum(c)) {
                movieNameVec.push_back(tolower(c));
            }
        }
        string movieName(movieNameVec.begin(), movieNameVec.end());
        for(int y = 0; y<words.size(); y++) {
            if (movieName.find(words[y])==string::npos) {
                //std::cout<<temp->name<<std::endl;
                good = false;
            }
        }
        if(good) {
            //std::cout << temp->name << std::endl;
            results.push_back(temp);
        }
        temp = temp->next;
    }
    return results;
}

std::vector<MovieNode *> HashTable::recommend(AccountManager const &accountReader, IniReader const &db) const {
    auto user = accountReader.getUserData();
    //0=Action
    //1=Horror
    //2=Scifi
    //3=Comedy
    //4=Drama
    std::vector<std::vector<MovieNode*>> userGenres = {Action,Horror,Scifi,Comedy,Drama};
    std::vector<MovieNode*> results;
    std::vector<int>recGenres = {0,1,2,3,4};
    std::random_device rd;
    std::default_random_engine rng(rd());
    //shuffles first because it's likely faster than picking a random number each time
    for (auto& genre : userGenres) {
        std::shuffle(genre.begin(), genre.end(), rng);
    }
    for (int i = 1; i < user->getSectionCount(); ++i) {
        //db grab genre[
        //add 1 of each
        std::string genre = db(user->getSectionName(i),"Genre");
      //adds weight to categorys
        if(genre == "Action") {
            if(user->getPropertyValue(i,0)=="2") {
                //std::cout<<user->getSectionName(i)<<std::endl;
                recGenres.push_back(0);
                recGenres.push_back(0);
                recGenres.push_back(0);
            }
            if(user->getPropertyValue(i,2)=="1") {
                recGenres.push_back(0);
                recGenres.push_back(0);
            }
        } else if (genre == "Horror") {
            if(user->getPropertyValue(i,0)=="2") {
                recGenres.push_back(1);
                recGenres.push_back(1);
                recGenres.push_back(1);
            }
            if(user->getPropertyValue(i,2)=="1") {
                recGenres.push_back(1);
                recGenres.push_back(1);
            }
        } else if (genre == "Sci-Fi") {
            if(user->getPropertyValue(i,0)=="2") {
                recGenres.push_back(2);
                recGenres.push_back(2);
                recGenres.push_back(2);
            }
            if(user->getPropertyValue(i,2)=="1") {
                recGenres.push_back(2);
                recGenres.push_back(2);
            }
        } else if (genre == "Comedy") {
            if(user->getPropertyValue(i,0)=="2") {
                recGenres.push_back(3);
                recGenres.push_back(3);
                recGenres.push_back(3);
            }
            if(user->getPropertyValue(i,2)=="1") {
                recGenres.push_back(3);
                recGenres.push_back(3);
            }
        } else {
            if(user->getPropertyValue(i,0)=="2") {
                recGenres.push_back(4);
                recGenres.push_back(4);
                recGenres.push_back(4);
            }
            if(user->getPropertyValue(i,2)=="1") {
                recGenres.push_back(4);
                recGenres.push_back(4);
            }
        }
    }
    srand(time(NULL));
    int size = 50;
    //selects 50 movies based on recGenres
    while(size>=0) {
      //random genre from weighted list
        int movie = recGenres[rand() % recGenres.size()];
        if (!userGenres[movie].empty()) {
            //adds movie to results and deletes it from genre
            results.push_back(userGenres[movie].back());
            userGenres[movie].pop_back();
        }
        --size;
    }
    return results;
}
