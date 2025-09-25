#include "gdumper/processmanager.hpp"

#include <algorithm>
#include <expected>
#include <fstream>
#include <optional>
#include <set>

namespace processManager {
std::optional<std::string> formatLine(std::string_view defaultLine,
                                      std::string_view findOnly,
                                      bool showFullPath) {
    const size_t lastSpace{defaultLine.find_last_of(' ')};
    if (lastSpace == std::string::npos) return std::nullopt;

    std::string path{defaultLine.substr(lastSpace + 1)};
    if (path.empty() || path.find('/') == std::string::npos)
        return std::nullopt;

    if (!showFullPath) {
        const size_t lastSlash{path.find_last_of('/')};
        if (lastSlash != std::string::npos) {
            path = path.substr(lastSlash + 1);
        }
    }

    std::string pathLower{path};
    std::transform(pathLower.begin(), pathLower.end(), pathLower.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });

    std::string filterLower{findOnly};
    std::transform(filterLower.begin(), filterLower.end(), filterLower.begin(),
                   [](unsigned char c){ return static_cast<char>(std::tolower(c)); });


    return (findOnly.empty() ||
            pathLower.find(filterLower) != std::string::npos)
               ? std::optional<std::string>{path}
               : std::nullopt;
}

std::expected<std::vector<std::string>, std::string> getFormattedLines(
    const std::string& path, std::string_view findOnly, bool showFullPath) {
    std::ifstream infile(path);
    if (!infile.is_open()) {
        return std::unexpected("Error while opening map file");
    };

    std::vector<std::string> formactedLineVector;
    std::set<std::string> seenLines;
    std::string line;

    while (getline(infile, line)) {
        const auto formactedLine = formatLine(line, findOnly, showFullPath);
        if (formactedLine && seenLines.insert(*formactedLine).second) {
            formactedLineVector.push_back(*formactedLine);
        }
    }

    infile.close();
    return formactedLineVector;
}
}  // namespace processManager
