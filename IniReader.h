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

    // === write ==========================================================
    // Write the ini object to the file. If no parameters are specified,
    // the default file will be written. On successful write, returns true.
    // If default file not found, false is returned.
    //
    // Parameters:
    //      none OR string fileName
    // Returns:
    //      boolean truth
    // ====================================================================
    bool write();
    bool write(std::string const &fileName);


    /**** Sections-related functions ****/

    // === getSectionCount ================================================
    // Get the number of sections in the ini.
    //
    // Parameters:
    //      none
    // Returns:
    //      number of sections
    // ====================================================================
    int getSectionCount() const;

    // === getSectionName =================================================
    // Given a section index, return a pointer to its name.
    //
    // Parameters:
    //      int index of section
    // Returns:
    //      number of sections
    // ====================================================================
    std::string const &getSectionName(int section);

    // === getSections ====================================================
    // Get all section data. Returns a reference to the internal
    // IniSections type, which is essentially a vector of strings.
    //
    // Parameters:
    //      none
    // Returns:
    //      IniSections reference
    // ====================================================================
    IniSections const &getSections();

    // === addSection =====================================================
    // Adds a section.
    //
    // Parameters:
    //      string section name
    // Returns:
    //      section number
    // ====================================================================
    int addSection(std::string const &name);

    // === findSection ====================================================
    // Find a section with the name passed in args.
    //
    // Parameters:
    //      string section name
    // Returns:
    //      index of this section
    // ====================================================================
    int findSection(std::string const &name);

    // === setSectionName =================================================
    // Set/rename a section.
    //
    // Parameters:
    //      int section, string name
    // Returns:
    //      void
    // ====================================================================
    void setSectionName(int section, std::string const &name);

    // === dropSection ====================================================
    // Remove an entire section from the ini, including all key-values
    // that fall under it.
    //
    // Parameters:
    //      int section
    // Returns:
    //      void
    // ====================================================================
    void dropSection(int section);


    /**** Key-value/properties related functions ****/

    // === getPropertyCount ===============================================
    // Get the number of key/value pairs in the ini.
    //
    // Parameters:
    //      none
    // Returns:
    //      number of properties
    // ====================================================================
    int getPropertyCount() const;

    // === getPropertyKey =================================================
    // Given a section and property index, return a pointer to key name.
    //
    // Parameters:
    //      int index of section and property
    // Returns:
    //      reference to string containing key name
    // ====================================================================
    std::string const &getPropertyKey(int section, int property);

    // === getPropertyValue ===============================================
    // Given a section and property index, return a pointer to value name.
    //
    // Parameters:
    //      int index of section and property
    // Returns:
    //      reference to string containing value name
    // ====================================================================
    std::string const &getPropertyValue(int section, int property);

    // === getProperties ==================================================
    // Get all property data. Returns a reference to the internal
    // IniProperties type.
    //
    // Parameters:
    //      none
    // Returns:
    //      IniProperties reference
    // ====================================================================
    IniProperties const &getProperties();

    // === addProperty ====================================================
    // Adds a property into a section. Specify 0 for the global area
    //
    // Parameters:
    //      section id
    // Returns:
    //      nothing
    // ====================================================================
    void addProperty(int section, std::string const &key, std::string const &value);

    // === findProperty ===================================================
    // Find the property index (key-value pair) given a section index and
    // the name of the key.
    //
    // Parameters:
    //      int section, string name
    // Returns:
    //      index of the property
    // ====================================================================
    int findProperty(int section, std::string const &name);

    // === setPropertyKey =================================================
    // Set/rename a property key.
    //
    // Parameters:
    //      int section, int property, string name
    // Returns:
    //      void
    // ====================================================================
    void setPropertyKey(int section, int property, std::string const &key);

    // === setPropertyValue ===============================================
    // Set/rename a property value.
    //
    // Parameters:
    //      int section, int property, string name
    // Returns:
    //      void
    // ====================================================================
    void setPropertyValue(int section, int property, std::string const &value);

    // === dropProperty ===================================================
    // Remove a key-value pair property.
    //
    // Parameters:
    //      int section, int property
    // Returns:
    //      void
    // ====================================================================
    void dropProperty(int section, int property);


  private:
    std::string    fileName;    // origin file name
    IniSections    sections;    // ini section names
    IniProperties  properties;  // all key-value pairs

    // for returning on reference-returning functions
    static std::string const ZEROSTR;

    // === _propertyIndex =================================================
    // Get the global index of a key-value property in `properties`.
    // The global index is the index of this property disregarding offsets
    // like the section index. Only used internally.
    //
    // Parameters:
    //      int section and property
    // Returns:
    //      index of the property
    // ====================================================================
    int _propertyIndex(int section, int property);

    // === _strieq ========================================================
    // Case-insensitive strings comparison. Returns true if equal.
    // Name is "STRing Insensitive EQual"
    //
    // Parameters:
    //      Two strings
    // Returns:
    //      boolean truth
    // ====================================================================
    bool _strieq(std::string const &lhs, std::string const &rhs);
};
