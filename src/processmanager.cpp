#include "gdumper/processmanager.hpp"

#include <algorithm>
#include <expected>
#include <fstream>
#include <set>

#define IGNORE_PRESET "ignore"

namespace processManager {
std::string lineFormatter(const std::string& defaultLine,
                          const std::string& findOnly, bool showFullPath) {
    const size_t lastSpace{defaultLine.find_last_of(' ')};
    if (lastSpace == std::string::npos) return IGNORE_PRESET;

    std::string path{defaultLine.substr(lastSpace + 1)};
    if (path.empty() || path.find('/') == std::string::npos)
        return IGNORE_PRESET;

    if (!showFullPath) {
        const size_t lastSlash{path.find_last_of('/')};
        if (lastSlash != std::string::npos) {
            path = path.substr(lastSlash + 1);
        }
    }

    std::string pathLower{path};
    std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(),
                   ::tolower);

    std::string filterLower{findOnly};
    std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(),
                   ::tolower);

    if (findOnly.empty() || pathLower.find(filterLower) != std::string::npos) {
        return path;
    }

    return IGNORE_PRESET;
}

std::expected<std::vector<std::string>, std::string> getFormactedLine(
    const std::string path, const std::string findOnly, bool showFullPath) {
    std::ifstream infile(path);
    if (!infile.is_open()) {
        return std::unexpected("Error while opening map file");
    };

    std::vector<std::string> formactedLineVector;
    std::set<std::string> seenLines;
    std::string line;

    while (getline(infile, line)) {
        const std::string formactedLine{
            lineFormatter(line, findOnly, showFullPath)};
        if (formactedLine != IGNORE_PRESET &&
            seenLines.insert(formactedLine).second) {
            formactedLineVector.push_back(formactedLine);
        }
    }

    infile.close();
    return formactedLineVector;
}
}  // namespace processManager
