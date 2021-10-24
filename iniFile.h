#ifndef INIFILE_H_INCLUDED
#define INIFILE_H_INCLUDED

#include <fstream>
#include <string>
#include <map>
#include <vector>

class iniFile {
    std::string _fileName;
    std::map<std::string, std::map<std::string, std::string>> _data;
    void reload();
    void save();
public:
    iniFile(const std::string& fileName);

    int readInt(const std::string& section, const std::string& key, int def = 0);
    double readDouble(const std::string& section, const std::string& key, double def = 0);
    std::string readString(const std::string& section, const std::string& key, const std::string& def = "");

    bool writeInt(const std::string& section, const std::string& key, int value);
    bool writeDouble(const std::string& section, const std::string& key, double value);
    bool writeString(const std::string& section, const std::string& key, const std::string& value);
    bool writeComment(const std::string& section, const std::string& value);
    bool writeComment(const std::string& section, const std::string& key, const std::string& value);

    bool isExist();
    bool isSectionExist(const std::string& section);
    bool isKeysExist(const std::string& section, const std::string& key);

    size_t getSectionsCount();
    size_t getKeysCount(const std::string& section);

    std::vector<std::string> getSections();
    std::vector<std::string> getKeys(const std::string& section);
    std::vector<std::string> getValues(const std::string& section);

    bool deleteSection(const std::string& section);
    bool deleteKey(const std::string& section, const std::string& key);
};

#endif // INIFILE_H_INCLUDED
