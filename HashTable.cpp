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

MovieNode::MovieNode(string const &n, string const &y, string const &ra, string const &ru, string const &g, string const &dr, string const &de, string const &i, string const &p)
: name{n}, year{y}, rating{ra}, runtime{ru}, genre{g}, director{dr}, descript{de}, imdb{i}, poster{p}, next{nullptr}
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
int HashTable::hash(std::string const &key)
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
MovieNode* HashTable::set(std::string hashname, const std::string &name1,const std::string &year1,const std::string &rating1,const std::string &runtime1,
const std::string &genre1,const std::string &director1,const std::string &descript1,const std::string &imdb1,const std::string &poster1) {
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

std::vector<MovieNode*> HashTable::search(std::string movieSearch) {
    //const std::string& currentName = sects[i];
    //std::cout<<currentName<<std::endl;
    std::vector<char> letters;
    std::vector<string> words;
    std::vector<MovieNode*> results;
    for(int j = 0; j<movieSearch.length(); j++) {
        if(movieSearch[j]==' ' || j==movieSearch.length()-1) {
            if(j==movieSearch.length()-1&&isalnum(movieSearch[j])) {
                letters.push_back(std::tolower(movieSearch[j]));
            }
            std::string word(letters.begin(), letters.end());
            words.push_back(word);
            letters.clear();
        } else if(isalnum(movieSearch[j])) {
            letters.push_back(std::tolower(movieSearch[j]));
        }
    }
    /*for(int i = 0; i<words.size(); i++) {
        std::cout<<words[i]<<std::endl;
    }*/
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
        std::string movieName(movieNameVec.begin(), movieNameVec.end());
        for(int y = 0; y<words.size(); y++) {
            if (movieName.find(words[y])==std::string::npos) {
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

std::vector<MovieNode*> HashTable::recommend(AccountManager& accountReader, IniReader const &db) {
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
    /*for(int e = 0; e<recGenres.size(); e++) {
        std::cout<<recGenres[e];
    }*/
    //std::cout<<std::endl;
    srand(time(NULL) );
    int size = 50;//db.getSectionCount();
    while(size>=0) {
        int movie  = recGenres[rand() % (recGenres.size()-1)];
        results.push_back(userGenres[movie].back());
        userGenres[movie].pop_back();
        //std::cout<<movie;
        if(userGenres[movie].empty()) {
            for(int k = 0; k<recGenres.size(); ++k) {
                if(recGenres[k] == movie) {
                    userGenres[movie].erase(userGenres[movie].begin() + k);
                }
            }
        }
        size--;
        /*if(movie==0) {
            results.push_back(userGenres[0].back());
            userGenres[0].pop_back();
        } else if(movie==1) {

        } else if(movie==2) {

        } else if(movie==3) {

        } else if(movie==4) {

        }*/
    }
    return results;
    //randomNumber = rand() % 4 + 0;
    //move the fill has to medflix.cpp in private
    //called by medflix constructor
    //add alphanum filter
    //lowercase each character in fill hash
}



/*
// ==== getCourses ==============================================================
// this returns the professor data for a given teacher, and updates teacher
// name if case isn't quite the same as in hash map
//
// Input:
// string
//
// Output:
// prof_data
// =============================================================================
prof_data const *HashTable::getCourses(std::string &name)
{
    // first get hash then uppercase the string for case insensitive compare
    int index = hash(name);
    std::transform(name.begin(), name.end(), name.begin(), ::toupper);

    Node* temp = dataMap[index];

    while (temp != nullptr) {
        std::string aux = temp->name;
        std::transform(aux.begin(), aux.end(), aux.begin(), ::toupper);
        if (aux == name) {
            name = temp->name;
            return &temp->courses;
        }
        temp = temp->next;
    }
    return nullptr;
}

// ==== getNames ==============================================================
// gets the complete list of all professors in the hash table
//
// Input:
//
// Output:
// vector of names
// =============================================================================
std::vector<std::string> HashTable::getNames()
{
    // contrary to what it seems the compiler will optimize this
    // so that the return value isn't copied again unnecessarily.
    std::vector<std::string> allKeys;

    for (int i = 0; i < SIZE; ++i) {
        Node* temp = dataMap[i];
        while (temp != nullptr) {
            allKeys.push_back(temp->name);
            temp = temp->next;
        }
    }

    return allKeys;
}

// ==== addStudent ==============================================================
// adds a student to the professors courses and a text file with other student
// data. returns true if the student is already enrolled, false if they are not.
//
// Input:
// student id, teacher name, course name
//
// Output:
// bool
// =============================================================================
bool HashTable::addStudent(int id, std::string const &prof, std::string const &course)
{
    Node *temp = dataMap[hash(prof)];
    std::ofstream studentFile("students.txt", std::ios::app);
    //studentFile.open("students.txt");

    for (; temp != nullptr; temp = temp->next) {
        if (temp->name == prof) {
            for (auto& courseData : temp->courses) {
                if (courseData.first.first == course) {
                    for (auto const &studentId : courseData.second) {
                        if (studentId == id) {
                            return false;
                        }
                    }
                    courseData.second.push_back(id);
                    studentFile << prof << "," << course << "," << id << ",0;\n";
                    return true;
                }
            }
        }
    }

    return false;
}

// ==== addStudentText ==============================================================
// Adds the student to the course but not the text file. Only used at the beginning
// of the program to fill the hash and is always taking in data from student.txt.
//
// Input:
// student id, teacher name, course name
//
// Output:
//
// =============================================================================
void HashTable::addStudentText(int id, std::string const &prof, std::string const &course)
{
    Node *temp = dataMap[hash(prof)];

    //this lacks a check for duplicates as the addStudent function is always ran first
    for (; temp != nullptr; temp = temp->next) {
        if (temp->name == prof) {
            for (auto& courseData : temp->courses) {
                if (courseData.first.first == course){
                    courseData.second.push_back(id);
                }
            }
        }
    }
}

// ==== findClass ==============================================================
// Given a class, it finds all the teachers who teach the class
//
// Input:
// course name
//
// Output:
// a vector of the matching teachers
// =============================================================================
std::vector<std::pair<std::string, float>> HashTable::findClass(std::string &course)
{
    // uppercase the course string for caseless comparison
    std::transform(course.begin(), course.end(), course.begin(), ::toupper);

    std::vector <std::pair<std::string, float>> matchingTeachers;
    std::vector<std::string> teachers = getNames();

    for (int i = 0; i < teachers.size(); i++) {
        prof_data const *courses = getCourses(teachers[i]);
        for (auto &courseData : *courses) {
            std::string aux = courseData.first.first;
            std::transform(aux.begin(), aux.end(), aux.begin(), ::toupper);
            if (aux == course) {
                // set course string to the one in database (for correct capitalization)
                course = courseData.first.first;
                std::pair<std::string, float> target = { teachers[i],courseData.first.second };
                matchingTeachers.push_back(target);
                break;
            }
        }
    }

    return matchingTeachers;
}


// ==== getRegistered =====================================================
// Given a student ID, return all courses where the ID exists.
//
// Input:
// student ID
//
// Output:
// a vector of course name and professor name
// ========================================================================
std::vector<std::pair<std::string, std::string>> HashTable::getRegistered(int ID)
{
    std::vector<std::pair<std::string, std::string>> reg;  // return value
    std::vector<std::string> teachers(getNames());

    for (int i = 0 ; i < teachers.size(); ++i) {
        prof_data const *courses(getCourses(teachers.at(i)));
        for (auto &e : *courses) {
            for (auto id : e.second) {
                if (ID == id)
                    reg.push_back({e.first.first, teachers.at(i)});
            }
        }
    }

    return reg;

}

// ==== check ==============================================================
// Given a class, it finds all the teachers who teach the class
//
// Input:
// course name
//
// Output:
// a vector of the matching teachers
// =============================================================================
bool HashTable::check(int id, std::string &teacher, std::string &course)
{
    // transform course string to uppercase
    std::transform(course.begin(), course.end(), course.begin(), ::toupper);
    // get course data (this uppercases passed string)
    prof_data const *courses = getCourses(teacher);
    if (courses) for (auto& courseData : *courses) {
        std::string aux = courseData.first.first;
        std::transform(aux.begin(), aux.end(), aux.begin(), ::toupper);
        if (aux == course) {
            // set passed string to original from database
            course = courseData.first.first;
            for (auto& studentId : courseData.second) {
                if (studentId == id) {
                    return true;
                }
            }
        }
    }

    return false;
}

// ==== changeRating ==============================================================
// this adds a rating from the user and recalculates the average rating.
//
// Input:
// course, teacher, new rating
//
// Output:
//
// =============================================================================
void HashTable::changeRating(std::string course, std::string teacher, float newRating, int id) {
    float total = 0;

    std::string filedata;
    std::ifstream input("students.txt");
    std::vector<std::string> totalfiledata;
    int line = 0;

    if (!input.is_open()) {
        std:: cout << "Error: Could not open file 'students.txt'\n";
        return;
    }

    //searches txt file for everyone who left a review
    while (getline(input, filedata)) {
       std::vector<char> sectionData;
        int tempID;
       int section = 0;

       for (int i = 0; i < filedata.length(); i++) {
           //the check for last character is because things are only processed if they have commas
           if (filedata[i] == ',' || i == filedata.length() - 1) {
               if (section == 0) {
                   //transforms char vector to string
                   std::string getTeacher(sectionData.begin(), sectionData.end());
                   if(teacher != getTeacher) {
                       break;
                   }
               } else if (section == 1) { //class
                   std::string getCSclass(sectionData.begin(), sectionData.end());
                   if(getCSclass != course) {
                       break;
                   }
               } else if (section == 2) { //ID
                   std::string gettempID(sectionData.begin(), sectionData.end());
                   tempID = stoi(gettempID);
               } else if(section == 3) { //Have they made a rating for this class before?
                   std::string getStdRating(sectionData.begin(), sectionData.end());
                   if(getStdRating == "0" && tempID == id) {
                       filedata[i-1] = '1';
                       total = total + 1;
                   } else if(getStdRating == "1" && tempID == id) {
                       std::cout<<"You have already rated this teacher so your previous rating will be replaced\n";
                       total = total + 1;
                   } else if (getStdRating == "1") {
                       total = total + 1;
                   }
               }

               sectionData.clear();
               section++;
           } else {
               sectionData.push_back(filedata[i]);
           }
       }
        totalfiledata.push_back(filedata);
        line++;
    }


    //calculates average rating then changes actual rating
    Node *temp = dataMap[hash(teacher)];
    for (; temp != nullptr; temp = temp->next) {
        if (temp->name == teacher) {
            for (auto& courseData : temp->courses) {
                if (courseData.first.first == course){
                    float rating = courseData.first.second;
                    float totalRating = 0;
                    if(total >= 2) {
                        totalRating = rating*(total-1);
                        totalRating = totalRating + newRating;
                        newRating = totalRating/total;
                    }
                    courseData.first.second = newRating;
                }
            }
        }
    }

    std::ofstream clear;
    clear.open("students.txt", std::ofstream::out | std::ofstream::trunc);
    clear.close();

    std::ofstream output("students.txt", std::ios::app);
    for(int z = 0; z < totalfiledata.size(); z++) {
        output<<totalfiledata[z]<<"\n";
    }
}

// ==== updateTable ==============================================================
// this re enters all the data into the teachers.txt file. this was used
// to make permanent changes to a teachers rating
//
// Input:
//
// Output:
//
// =============================================================================
void HashTable::updateTable() {
    Node *temp = dataMap[0];

    //deletes contents of teachers.txt
    std::ofstream clear;
    clear.open("teachers.txt", std::ofstream::out | std::ofstream::trunc);
    clear.close();

    std::ofstream teacherFile("teachers.txt", std::ios::app);
    std::vector<std::string> teachers = getNames();

    for (int i = 0; i < teachers.size(); i++) {
        prof_data const *courses = getCourses(teachers[i]);
        teacherFile << teachers[i];
        for (auto &courseData : *courses) {
            teacherFile << "," <<courseData.first.first << "," << courseData.first.second;
        }
        teacherFile << ";\n";
    }

}
*/
