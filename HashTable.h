// Attached: lab#12
// File: HashTable.h
// =============================================================================
//
// Programmers: Aiden Asar, Filip Vracevic
// Class: CS 1D
// Instructor: Med Mogasemi
//
// =============================================================================
// Program: Student Reigistration System(lab#12)
// =============================================================================
// Description:
// declarations for PrintMenu class
// =============================================================================
// =============================================================================


#include <string>
#include <vector>
#include <iostream>

//                                      (class)      (rating)      (students)
//                                    v.first.first  .second        v.second
typedef std::vector<std::pair<std::pair<std::string, float>, std::vector<int>>> prof_data;
//what the teacher's name will be linked to

struct Node {
    std::string const &name;      // movie name (used as key)
    std::string const &year;
    std::string const &rating;
    std::string const &runtime;
    std::string const &genre;
    std::string const &director;
    std::string const &descript;
    std::string const &imdb;
    std::string const &poster;
    Node* next;

    Node(std::string const &, std::string const &, std::string const &,
         std::string const &, std::string const &, std::string const &,
         std::string const &, std::string const &, std::string const &);
};

// Not declared or implemented yet.
/*
class HashTable {
private:
    static const int SIZE = 7;
    Node* dataMap[SIZE];

    int hash(std::string const &key);

public:
    ~HashTable();
    //adds a teacher and their information to the linked list
    void set(std::string name, prof_data courses);

    // For a given professor name, get courses
    prof_data const *getCourses(std::string &name);

    //creates a list of all teachers
    std::vector<std::string> getNames();

    //adds a student to a course
    bool addStudent(int id, std::string const &teacher, std::string const &course);

    //adds student at the start of the program
    void addStudentText(int id, std::string const &prof, std::string const &course);

    //adds a rating for a teacher
    void changeRating(std::string course, std::string teacher, float newRating, int id);

    //updates the teacher txt file when the program ends
    void updateTable();

    //returns a list of all teachers who teach a specific course
    std::vector<std::pair<std::string, float>> findClass(std::string &course);

    // returns a list of all courses student is registered in
    std::vector<std::pair<std::string, std::string>> getRegistered(int ID);

    // check if id already registered
    bool check(int id, std::string &teacher, std::string &course);
};
*/