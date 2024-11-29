// Attached: Lab #13
// File: IniReader.h
// =============================================================================
//
// Programmers: Filip Vracevic
// Class: CS 1D
// Instructor: Med Mogasemi
//
// =============================================================================
// Program: MedFlix (Movie tracking system) (lab #13)
// =============================================================================
// Definitions for the INI reader/writer.
// =============================================================================
// =============================================================================


#include "IniReader.h"
#include <fstream>
#include <iostream>

using namespace std;

IniReader::IniReader()
{
    // Initialize global section
    sections.push_back("");
}

// Also call the default constructor to create global section
IniReader::IniReader(string const &fileName) : IniReader()
{
    // Open the file
    ifstream f(fileName);

    // Verify that the file is open
    if (!f.is_open())
        return;

    string buf;         // Line buffer variable
    int sect = 0;       // assume global section insertion at first

    // Read each line of the file and load into object
    while (getline(f, buf)) {
        size_t cur = 0;     // cursor: where in the line?
        // Find first non-whitespace character
        while (cur < buf.length() && buf[cur] <= ' ')
            ++cur;

        // Skip line if empty
        if (cur == buf.length())
            continue;

        // Check if this line is a comment
        if (buf[cur] == ';')
            continue;
        // Check if this line is a section header
        else if (buf[cur] == '[') {
            size_t start = ++cur;   // beginning of section name
            while (cur < buf.length() && buf[++cur] != ']');

            // End of section name
            if (buf[cur] == ']') {
                sect = addSection(buf.substr(start, cur - 1));
            }
        } else {    // Otherwise it's a key-value pair (properties)
            size_t keyStart = cur;      // beginnning of property
            while (cur < buf.length() && buf[++cur] != '=');

            if (buf[cur] == '=') {
                size_t keyEnd = cur - 1;
                // Skip equals sign
                ++cur;
                // Find last non-whitespace character in key,
                // hereby defining the key string size
                while (keyEnd > 0 && buf[--keyEnd - 1] <= ' ');

                // Find first non-whitespace character in value
                while (cur < buf.length() && buf[++cur] <= ' ');
                size_t valueStart = cur;
                // Find the end of the value
                while (++cur < buf.length());
                // Do not include trailing whitespace characters
                size_t valueSize = cur;
                while (valueSize >= valueStart && buf[--valueSize] <= ' ');

                if (cur == valueStart)
                    addProperty(sect, buf.substr(keyStart, keyEnd), "");
                else
                    addProperty(sect, buf.substr(keyStart, keyEnd), buf.substr(valueStart, cur - 1));
            }
        }
    }
}

IniReader::~IniReader()
{
    
}

int IniReader::addSection(string const &name)
{
    // Check if the name is empty
    if (!name.empty()) {
        // Add section name and return its index
        int idx = sections.size();
        sections.push_back(name);
        return idx;
    }
    // Otherwise we couldn't add the section
    return -1;
}

void IniReader::addProperty(int section, const std::string &key, const std::string &value)
{
    if (key.empty() || value.empty())
        return;

    // something here
}

IniSections const *IniReader::getSections()
{
    return &sections;
}

IniProperties const *IniReader::getProperties()
{
    return &properties;
}
