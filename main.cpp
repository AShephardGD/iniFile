#include <iostream>

#include "iniFile.h"

int main()
{
    iniFile file("test.ini");
    std::cout << "Get" << std::endl;
    std::cout << "file.getSectionsCount() = " << file.getSectionsCount() << std::endl;
    std::vector<std::string> sections = file.getSections(), keys = file.getKeys(sections[0]), values = file.getValues(sections[0]);
    std::cout << "Sections: ";
    for (auto it = sections.begin(); it != sections.end(); ++it) {
        std::cout << '(' << file.getKeysCount(*it) << ')' << (*it) << ", ";
    }
    std::cout << std::endl << '[' << sections[0] << ']' << std::endl;
    for (size_t i = 0; i < file.getKeysCount(sections[0]); ++i) {
        std::cout << keys[i] << '=' << values[i] << std::endl;
    }
    std::cout << std::endl << std::endl;

    std::cout << "Reading information" << std::endl;
    std::cout << "Read int = " << file.readInt("section3", "int") << std::endl;
    std::cout << "Read int = " << file.readInt("section3", "\"int\"") << std::endl;

    std::cout << "Read double = " << file.readDouble("section3", "double") << std::endl;
    std::cout << "Read double = " << file.readDouble("section3", "\"double\"") << std::endl;

    std::cout << "Read string = " << file.readString("section1", "key11") << std::endl;
    std::cout << "Read string = " << file.readString("section1", "key12") << std::endl;
    std::cout << "Read string = " << file.readString("section1", "key13") << std::endl;
    std::cout << "Read string = " << file.readString("section2", "key21") << std::endl << std::endl;

    std::cout << "Write and delete information" << std::endl;
    std::cout << "Write int = " << file.writeInt("section4", "key41", 1337) << std::endl;
    std::cout << "Delete key = " << file.deleteKey("section4", "key41") << std::endl;
    std::cout << "Write double = " << file.writeDouble("section4", "key42", 2.718) << std::endl;
    std::cout << "Delete key = " << file.deleteKey("section4", "key42") << std::endl;
    std::cout << "Write string = " << file.writeString("section4", "key43", "Some shit") << std::endl;
    std::cout << "Write comment key = " << file.writeComment("section4", "key43", "Another shit") << std::endl;
    std::cout << "Write comment void key = " << file.writeComment("section4", "key44", "Just a shit") << std::endl;
    std::cout << "Write comment section = " << file.writeComment("section4", "section comment") << std::endl;
    std::cout << "Write comment void section = " << file.writeComment("section5", "void comment") << std::endl;
    std::cout << "Delete section = " << file.deleteSection("section4") << std::endl << std::endl;

    std::cout << "isExist" << std::endl;
    std::cout << "isExist = " << file.isExist() << std::endl;
    std::cout << "isSectionExist(\"section1\") = " << file.isSectionExist("section1") << std::endl;
    std::cout << "isSectionExist(\"section10\") = " << file.isSectionExist("section10") << std::endl;
    std::cout << "isKeysExist(\"section1\", \"key11\") = " << file.isKeysExist("section1", "key11") << std::endl;
    std::cout << "isKeysExist(\"section1\", \"key101\") = " << file.isKeysExist("section1", "key101") << std::endl;
    std::cout << "isKeysExist(\"section10\", \"key101\") = " << file.isKeysExist("section10", "key101") << std::endl;
    return 0;
}
