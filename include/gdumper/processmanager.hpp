#pragma once

#include <expected>
#include <optional>
#include <string>
#include <vector>

namespace processManager {

std::optional<std::string> formatLine(std::string_view defaultLine,
                                      std::string_view findOnly,
                                      bool showFullPath);

std::expected<std::vector<std::string>, std::string> getFormattedLines(
    const std::string& path, std::string_view findOnly, bool showFullPath);

}  // namespace processManager
