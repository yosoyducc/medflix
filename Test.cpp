#include "IniReader.h"
#include <iostream>

using namespace std;

int main()
{
    IniReader ini("test.ini");

    /*IniSections const sect = ini.getSections();
    IniProperties const prop = ini.getProperties();*/

    cout << "Sections count:   " << ini.getSectionCount()  << endl;
    cout << "Properties count: " << ini.getPropertyCount() << endl;

    int sec = ini.findSection("goodbye to a world");
    int pro = ini.findProperty(sec, "LA_SEINE");

    cout << "Located in section `" << ini.getSectionName(sec)
         << "` in key `" << ini.getPropertyKey(sec, pro) << "`\n";

    cout << "This equals: " << ini.getPropertyValue(sec, pro) << endl;

    return 0;
}
