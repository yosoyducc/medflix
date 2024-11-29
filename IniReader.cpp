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
#include <utility>

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
        int const bufLen = buf.length();
        // Find first non-whitespace character
        while (cur < bufLen && buf[cur] <= ' ')
            ++cur;

        // Skip line if empty
        if (cur == bufLen)
            continue;

        // Check if this line is a comment
        if (buf[cur] == ';')
            continue;
        // Check if this line is a section header
        else if (buf[cur] == '[') {
            size_t start = ++cur;   // beginning of section name
            while (cur < bufLen && buf[++cur] != ']');

            // End of section name
            if (buf[cur] == ']') {
                sect = addSection(buf.substr(start, cur - 1));
            }
        } else {    // Otherwise it's a key-value pair (properties)
            size_t keyStart = cur;      // beginnning of property
            while (cur < bufLen && buf[++cur] != '=');

            if (buf[cur] == '=') {
                size_t keyEnd = cur - 1;
                // Skip equals sign
                ++cur;
                // Find last non-whitespace character in key,
                // hereby defining the key string size
                while (keyEnd > 0 && buf[--keyEnd - 1] <= ' ');

                // Find first non-whitespace character in value
                while (cur < bufLen && buf[++cur] <= ' ');
                size_t valueStart = cur;
                // Find the end of the value
                while (++cur < bufLen);
                // Do not include trailing whitespace characters
                size_t valueSize = cur;
                while (valueSize >= valueStart && buf[--valueSize] <= ' ');

                // If value doesn't exist (length zero)
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
        int idx = getSectionCount();
        sections.push_back(name);
        return idx;
    }
    // Otherwise we couldn't add the section
    return -1;
}

void IniReader::addProperty(int section, const std::string &key, const std::string &value)
{
    // Before constructing the IniProperty pair at the end of `properties`,
    // make sure name isn't empty and section isn't absurd
    if (!key.empty() && section >= 0 && section < getSectionCount())
        properties.emplace_back(section, make_pair(key, value));
}

// Get reference to internal sections
IniSections const *IniReader::getSections()
{
    return &sections;
}

// Get reference to internal properties
IniProperties const *IniReader::getProperties()
{
    return &properties;
}

// Get number of sections
int IniReader::getSectionCount()
{
    return sections.size();
}

// Get number of properties
int IniReader::getPropertyCount()
{
    return properties.size();
}
