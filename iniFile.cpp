#include <string>
#include <stdexcept>
#include <iostream>

#include "iniFile.h"

iniFile::iniFile(const std::string& fileName) : _fileName(fileName) {
    reload();
}


void iniFile::reload() {
    std::ifstream file(_fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Can't open file");
    }
    std::string readingString;
    _data = std::map<std::string, std::map<std::string, std::string>>();
    std::string section;
    while (std::getline(file, readingString)) {
        if (readingString[0] == '[') {
            section = readingString;
            _data[section] = std::map<std::string, std::string>();
        }
        else {
            std::string key = readingString.substr(0, readingString.find('='));
            std::string value = readingString.substr(readingString.find('=') + 1);
            _data[section][key] = value;
        }
    }
    file.close();
}

void iniFile::save() {
    std::ofstream file(_fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Can't open file");
    }
    for (auto it = _data.begin(); it != _data.end(); ++it) {
        file << (*it).first << '\n';
        for (auto iter = (*it).second.begin(); iter != (*it).second.end(); ++iter) {
            file << (*iter).first << '=' << (*iter).second << '\n';
        }
    }
}

int iniFile::readInt(const std::string& section, const std::string& key, int def) {
    try {
        return std::stoi(readString(section, key));
    }
    catch (std::invalid_argument& e) {
        return def;
    }
}

double iniFile::readDouble(const std::string& section, const std::string& key, double def) {
    try {
        return std::stod(readString(section, key));
    }
    catch (std::invalid_argument& e) {
        return def;
    }
}

std::string iniFile::readString(const std::string& section, const std::string& key, const std::string& def) {
    reload();
    try {
        if (!isKeysExist(section, key)) {
            return def;
        }
        std::string sectionInData = "[" + section + "]";
        for (auto it = _data.begin(); it != _data.end(); it++) {
            if ((*it).first.find(sectionInData) + 1) {
                std::string result = (*it).second[key];
                result = result.substr(0, result.find('#'));
                result = result.substr(0, result.find(';'));
                if (result.find('"') + 1) {
                    result = result.substr(1, result.find('"', 1) - 1);
                }
                return result;
            }
        }
    }
    catch (...) {
        return def;
    }
    return def;
}

bool iniFile::writeInt(const std::string& section, const std::string& key, int value) {
    return writeString(section, key, std::to_string(value));
}

bool iniFile::writeDouble(const std::string& section, const std::string& key, double value) {
    return writeString(section, key, std::to_string(value));
}

bool iniFile::writeString(const std::string& section, const std::string& key, const std::string& value) {
    reload();
    try {
        std::string sectionInData = "[" + section + "]";
        for (auto it = _data.begin(); it != _data.end(); it++) {
            if ((*it).first.find(sectionInData) + 1) {
                std::string oldComment = (*it).second[key];
                if (oldComment.find('#') + 1) {
                    oldComment = oldComment.substr(oldComment.find('#'));
                }
                if (oldComment.find(';') + 1) {
                    oldComment = oldComment.substr(oldComment.find(';'));
                }
                (*it).second[key] = value + oldComment;
                save();
                return true;
            }
        }
        _data[sectionInData][key] = value;
        save();
        return true;
    }
    catch (...) {
    }
    return false;
}

bool iniFile::writeComment(const std::string& section, const std::string& value) {
    reload();
    try {
        std::string sectionInData = '[' + section + ']';
        for (auto it = _data.begin(); it != _data.end(); ++it) {
            if ((*it).first.find(sectionInData) + 1) {
                _data[sectionInData + '#' + value] = (*it).second;
                _data.erase((*it).first);
                save();
                return true;
            }
        }
        _data[sectionInData + '#' + value] = std::map<std::string, std::string>();
        save();
        return true;
    }
    catch (...) {}
    return false;
}

bool iniFile::writeComment(const std::string& section, const std::string& key, const std::string& value) {
    reload();
    try {
        std::string sectionInData = '[' + section + ']';
        for (auto it = _data.begin(); it != _data.end(); ++it) {
            if ((*it).first.find(sectionInData) + 1) {
                std::string data = (*it).second[key];
                data = data.substr(0, data.find(';'));
                data = data.substr(0, data.find('#'));
                if (data.find('"') + 1) {
                    data = data.substr(1, data.find('"', 1) - 1);
                }
                (*it).second[key] = data + '#' + value;
                save();
                return true;
            }
        }
        _data[sectionInData][key] = '#' + value;
        save();
        return true;
    }
    catch (...) {}
    return false;
}

bool iniFile::isExist() {
    std::ifstream file(_fileName);
    return file.good();
}

bool iniFile::isSectionExist(const std::string& section) {
    reload();
    try {
        std::string sectionInData = "[" + section + "]";
        for (auto it = _data.begin(); it != _data.end(); it++) {
            if ((*it).first.find(sectionInData) + 1) {
                return true;
            }
        }
    }
    catch (...) {}
    return false;
}

bool iniFile::isKeysExist(const std::string& section, const std::string& key) {
    if (isSectionExist(section)) {
        std::string sectionInData = "[" + section + "]";
        for (auto it = _data.begin(); it != _data.end(); it++) {
            if ((*it).first.find(sectionInData) + 1) {
                for (auto iter = (*it).second.begin(); iter != (*it).second.end(); iter++) {
                    if ((*iter).first.find(key) + 1) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

size_t iniFile::getSectionsCount() {
    reload();
    return _data.size();
}

size_t iniFile::getKeysCount(const std::string& section) {
    size_t res = 0;
    std::string sectionInData = '[' + section + ']';
    for (auto it = _data.begin(); it != _data.end(); ++it) {
        if ((*it).first.find(sectionInData) + 1) {
            for (auto iter = (*it).second.begin(); iter != (*it).second.end(); ++iter) {
                ++res;
            }
            return res;
        }
    }
    return res;
}

std::vector<std::string> iniFile::getSections() {
    std::vector<std::string> result(getSectionsCount());
    size_t idx = 0;
    for (auto it = _data.begin(); it != _data.end(); ++it) {
        std::string getSection = (*it).first;
        size_t start = getSection.find('[') + 1, len = getSection.find(']') - start;
        result[idx++] = getSection.substr(start, len);
    }
    return result;
}

std::vector<std::string> iniFile::getKeys(const std::string& section) {
    std::vector<std::string> result(getKeysCount(section));
    size_t idx = 0;
    std::string sectionInData = '[' + section + ']';
    for (auto it = _data.begin(); it != _data.end(); ++it) {
        if ((*it).first.find(sectionInData) + 1) {
            for (auto iter = (*it).second.begin(); iter != (*it).second.end(); ++iter) {
                result[idx++] = (*iter).first;
            }
            return result;
        }
    }
    return result;
}

std::vector<std::string> iniFile::getValues(const std::string& section) {
    std::vector<std::string> result(getKeysCount(section));
    size_t idx = 0;
    std::string sectionInData = '[' + section + ']';
    for (auto it = _data.begin(); it != _data.end(); ++it) {
        if ((*it).first.find(sectionInData) + 1) {
            for (auto iter = (*it).second.begin(); iter != (*it).second.end(); ++iter) {
                std::string data = (*iter).second;
                data = data.substr(0, data.find('#'));
                data = data.substr(0, data.find(';'));
                if (data.find('"') + 1) {
                    data = data.substr(1, data.find('"', 1) - 1);
                }
                result[idx++] = data;
            }
            return result;
        }
    }
    return result;
}

bool iniFile::deleteSection(const std::string& section) {
    try {
        std::string sectionInData = '[' + section + ']';
        for (auto it = _data.begin(); it != _data.end(); ++it) {
            if ((*it).first.find(sectionInData) + 1) {
                _data.erase((*it).first);
                save();
                return true;
            }
        }
    }
    catch (...) {}
    return false;
}

bool iniFile::deleteKey(const std::string& section, const std::string& key) {
    try {
        std::string sectionInData = '[' + section + ']';
        for (auto it = _data.begin(); it != _data.end(); ++it) {
            if ((*it).first.find(sectionInData) + 1) {
                (*it).second.erase(key);
                save();
                return true;
            }
        }
    }
    catch (...) {}
    return false;
}
