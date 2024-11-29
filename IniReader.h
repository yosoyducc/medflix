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
  private:
    std::string    fileName;    // origin file name
    IniSections    sections;    // ini section names
    IniProperties  properties;  // all key-value pairs

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

    // === addSection =====================================================
    // Adds a section.
    //
    // Parameters:
    //      string section name
    // Returns:
    //      section number
    // ====================================================================
    int addSection(std::string const &name);

    // === addProperty ====================================================
    // Adds a property into a section. Specify 0 for the global area
    //
    // Parameters:
    //      section id
    // Returns:
    //      nothing
    // ====================================================================
    void addProperty(int section, std::string const &key, std::string const &value);

    // === getSectionCount ================================================
    // Get the number of sections in the ini.
    //
    // Parameters:
    //      none
    // Returns:
    //      number of sections
    // ====================================================================
    int getSectionCount();

    // === getPropertyCount ===============================================
    // Get the number of properties in the ini.
    //
    // Parameters:
    //      none
    // Returns:
    //      number of properties
    // ====================================================================
    int getPropertyCount();

    // === getSections ====================================================
    // Get all section data. Returns a reference to the internal
    // IniSections type.
    //
    // Parameters:
    //      none
    // Returns:
    //      IniSections pointer
    // ====================================================================
    IniSections const *getSections();

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
};
