// Attached: Lab #13
// File: IniReader.h
// ========================================================================
//
// Programmers: Aiden Asar, Filip Vracevic
// Class: CS 1D
// Instructor: Med Mogasemi
//
// ========================================================================
// Program: MedFlix (Movie tracking system) (lab#13)
// ========================================================================
// Declarations for the INI reader/writer.
// ========================================================================
// ========================================================================


#pragma once

#include <string>
#include <vector>

typedef
    std::vector<
        std::string             // section name
    >
    IniSections;

typedef
    std::vector<
        std::pair<
            int,                // section number
            std::pair<
                std::string,    // key
                std::string     // value
            >
        >
    >
    IniProperties;


class IniReader {
  public:
    // === IniReader ======================================================
    // Creates an empty INI object in memory.
    // ====================================================================
    IniReader();

    // === IniReader ======================================================
    // Loads an INI file from disk.
    //
    // Parameters:
    //      File name
    // ====================================================================
    IniReader(std::string const &fileName);

    // === ~IniReader =====================================================
    // Destroys the INI object.
    // ====================================================================
    ~IniReader();


    // === getSectionCount ================================================
    // Get the number of sections in the ini.
    //
    // Parameters:
    //      none
    // Returns:
    //      number of sections
    // ====================================================================
    int getSectionCount();

    // === getSectionName =================================================
    // Given a section index, return a pointer to its name.
    //
    // Parameters:
    //      int index of section
    // Returns:
    //      number of sections
    // ====================================================================
    std::string const *getSectionName(int section);

    // === getSections ====================================================
    // Get all section data. Returns a reference to the internal
    // IniSections type, which is essentially a vector of strings.
    //
    // Parameters:
    //      none
    // Returns:
    //      IniSections pointer
    // ====================================================================
    IniSections const *getSections();

    // === addSection =====================================================
    // Adds a section.
    //
    // Parameters:
    //      string section name
    // Returns:
    //      section number
    // ====================================================================
    int addSection(std::string const &name);


    // === getPropertyCount ===============================================
    // Get the number of properties in the ini.
    //
    // Parameters:
    //      none
    // Returns:
    //      number of properties
    // ====================================================================
    int getPropertyCount();

    // === getPropertyName ================================================
    // Given a section and property index, return a pointer to key name.
    //
    // Parameters:
    //      int index of section
    // Returns:
    //      number of sections
    // ====================================================================
    std::string const *getPropertyName(int section, int property);

    // === getProperties ==================================================
    // Get all property data. Returns a reference to the internal
    // IniProperties type.
    //
    // Parameters:
    //      none
    // Returns:
    //      IniProperties pointer
    // ====================================================================
    IniProperties const *getProperties();

    // === addProperty ====================================================
    // Adds a property into a section. Specify 0 for the global area
    //
    // Parameters:
    //      section id
    // Returns:
    //      nothing
    // ====================================================================
    void addProperty(int section, std::string const &key, std::string const &value);

  private:
    std::string    fileName;    // origin file name
    IniSections    sections;    // ini section names
    IniProperties  properties;  // all key-value pairs
};
