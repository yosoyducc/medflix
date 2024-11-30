#include "IniReader.h"
#include <iostream>

using namespace std;

int main()
{
    // this already calls add*() so i won't use those here
    IniReader ini("test.ini");

    cout << "Sections count:   " << ini.getSectionCount()  << endl;
    cout << "Properties count: " << ini.getPropertyCount() << endl;

    string section = "goodbye to a world";
    string key = "LA_SEINE";

    // Sample display using overloaded constructor
    cout << "At Section " << section
         << " and key name " << key
         << "\nis: " << ini(section, key) << "\n";
    // wow!

    int sec = ini.findSection(section);
    int pro = ini.findProperty(sec, key);

    // intentionally get references before renaming section, key, value
    auto &secName = ini.getSectionName(sec);
    auto &keyName = ini.getPropertyKey(sec, pro);
    auto &valName = ini(sec, pro);

    section = "Hello To A World!";
    ini.setSectionName(sec, section);
    ini.setPropertyKey(sec, pro, "El_Sena");
    ini.setPropertyValue(sec, pro, "777 kilometers");

    cout << "Recently changed!\n"
         << "Located in section `" << secName
         << "` in key `" << keyName << "`\n"
         << "This equals: " << valName << endl;

    // drop the first "Hello World" section
    ini.dropSection(1);
    // drop "ok" key-value
    sec = ini.findSection(section);
    pro = ini.findProperty(sec, "ok");
    ini.dropProperty(sec, pro);

    cout << "Dropped 'ok' from " << section << ".\n";

    cout << "Sections count:   " << ini.getSectionCount()  << endl;
    cout << "Properties count: " << ini.getPropertyCount() << endl;

    // write file
    if (ini.write("output.ini"))
        cout << "Success! wrote file\n";
    else
        cout << "no write";

    return 0;
}
