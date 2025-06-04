#include "../../include/ProcessManager.hpp"
#include <fstream>
#include <algorithm>
#include <set>

bool lineFormatter(const std::string& line, const std::string& findOnly, std::string* outPath, bool showFullPath) {

    size_t lastSpace = line.find_last_of(' ');
    if (lastSpace == std::string::npos) return false;
    
    std::string path = line.substr(lastSpace + 1);
    if (path.empty() || path.find('/') == std::string::npos) return false;

    if (!showFullPath) {
        size_t lastSlash = path.find_last_of('/');
        if (lastSlash != std::string::npos) {
            path = path.substr(lastSlash + 1);
        }
    }

    std::string pathLower = path;
    std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(), ::tolower);

    std::string filterLower = findOnly;
    std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(), ::tolower);

    if (findOnly.empty() || pathLower.find(filterLower) != std::string::npos) {
        *outPath = path;
        return true;
    }
    return false;
}

bool fileManager(const std::string& path, const std::string& findOnly, std::vector<std::string>* strings, bool showFullPath) {
    std::ifstream infile(path);
    if (!infile.is_open()) return false;

    std::set<std::string> uniquePaths;
    std::string line;
    std::string extractedPath;

    while (getline(infile, line)) {
        if (lineFormatter(line, findOnly, &extractedPath, showFullPath)) {
            uniquePaths.insert(extractedPath);
        }
    }

    infile.close();
    strings->assign(uniquePaths.begin(), uniquePaths.end());
    return true;
}
