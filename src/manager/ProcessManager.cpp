#include "../../include/ProcessManager.h"

#include <fstream>
#include <iostream>
#include <regex>

bool lineFormatter(const std::string& line,  const std::string& findOnly, std::unordered_map<int, std::string> *strings, const int count) {
    if (line.find('/') == std::string::npos) return false;

    const size_t pos = line.find('/');
    const std::string modifiedLine = line.substr(pos + 1);

    if (modifiedLine.empty()) return false;

    if (findOnly.empty() || modifiedLine.find(findOnly) != std::string::npos) {
        (*strings)[count] = modifiedLine;
        return true;
    }
    return false;

}

bool fileManager(const std::string& path,const std::string& findOnly, std::unordered_map<int, std::string> *strings ) {
    std::ifstream infile;
    infile.open(path);
    std::string line;

    if (infile.fail()) return false;

    int count = 1;
    while (getline(infile, line)){
        if (lineFormatter(line, findOnly, strings, count)) {
            count++;
        }
    }
    infile.close();

    return true;
}