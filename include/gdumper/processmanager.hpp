#pragma once

#include <expected>
#include <string>
#include <vector>

namespace processManager {

std::string lineFormatter(const std::string_view defaultLine,
                          const std::string_view findOnly, bool showFullPath);

std::expected<std::vector<std::string>, std::string> getFormactedLine(
    const std::string path, const std::string findOnly, bool showFullPath);
}  // namespace processManager
