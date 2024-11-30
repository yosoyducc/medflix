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

// How much memory should be allocated for internal variables
#define INI_SECTION_RESERVE  128
#define INI_PROPERTY_RESERVE 1024

using namespace std;

IniReader::IniReader()
{
    sections.reserve(INI_SECTION_RESERVE);
    properties.reserve(INI_PROPERTY_RESERVE);

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
        size_t const bufLen = buf.length();
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
            while (++cur < bufLen && buf[cur] != ']');

            // End of section name
            if (buf[cur] == ']')
                sect = addSection(buf.substr(start, cur - start));
        } else {    // Otherwise it's a key-value pair (properties)
            size_t keyStart  = cur;     // beginnning of property
            size_t keyLength = 1;       // assume key is size 1
            while (cur < bufLen && buf[++cur] != '=')
                ++keyLength;

            if (buf[cur] == '=') {
                // Find last non-whitespace character in key,
                // hereby defining the key string size
                size_t keyEnd = keyStart + keyLength;
                while (keyLength > 0 && buf[keyEnd-- - 1] <= ' ')
                    --keyLength;

                // Find first non-whitespace character in value
                ++cur;      // Skip equals sign
                while (cur < bufLen && buf[cur] <= ' ')
                    ++cur;
                size_t valueStart = cur;

                // Navigate to the end of the value
                cur = bufLen;
                // Exclude trailing whitespace characters
                while (cur >= valueStart && buf[cur] <= ' ')
                    --cur;

                // Determine length of value
                size_t valueLength = cur - valueStart + 1;

                // If value doesn't exist (length zero)
                if (valueLength < 1)
                    addProperty(sect, buf.substr(keyStart, keyLength), "");
                else
                    addProperty(sect, buf.substr(keyStart, keyLength), buf.substr(valueStart, valueLength));
            }
        }
    }
}

IniReader::~IniReader()
{
    // no need for the below
    //fileName.clear();
    //sections.clear();
    //properties.clear();
}

bool IniReader::empty()
{
    return sections.empty() && properties.empty();
}

/**** Sections-related functions ****/

int IniReader::getSectionCount() const
{
    // Get the number of sections
    return sections.size();
}

string const *IniReader::getSectionName(int section)
{
    // Only return the reference if index is valid
    if (section < getSectionCount())
        return &sections[section];

    return nullptr;
}

IniSections const *IniReader::getSections()
{
    // Get reference to internal sections
    return &sections;
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


/**** Key-value/properties related functions ****/

int IniReader::getPropertyCount() const
{
    // Get number of properties
    return properties.size();
}

string const *IniReader::getPropertyName(int section, int property)
{
    if (section >= 0 && section < getSectionCount()) {
        // If property was found
        int _prop = _propertyIndex(section, property);
        if (_prop != -1)
            return &properties[_prop].second.first;
    }
    return nullptr;
}

IniProperties const *IniReader::getProperties()
{
    // Get reference to internal properties
    return &properties;
}

void IniReader::addProperty(int section, const string &key, const string &value)
{
    // Before constructing the IniProperty pair at the end of `properties`,
    // make sure name isn't empty and section isn't absurd
    if (!key.empty() && section >= 0 && section < getSectionCount())
        properties.emplace_back(section, make_pair(key, value));
}


/**** Private implementation ****/

int IniReader::_propertyIndex(int section, int property)
{
    // Check to see if section specified is valid
    if (section >= 0 && section < getSectionCount()) {
        int _prop = 0;
        // Search through all properties
        for (int i = 0; i < getPropertyCount(); ++i) {
            if (properties[i].first == section) {
                if (_prop == property)
                    return i;   // It is this local prop! Return global index
                ++_prop;        // It wasn't this property, iterate
            }
        }
    }
    return -1;      // It wasn't found :(
}