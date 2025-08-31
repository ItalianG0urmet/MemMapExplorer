#pragma once

#include <expected>
#include <string>
#include <vector>

namespace process_manager {

std::string lineFormatter(const std::string& defaultLine,
                          const std::string& findOnly, bool showFullPath);

std::expected<std::vector<std::string>, std::string> formactedLineGetter(
    const std::string path, const std::string findOnly, bool showFullPath);
}  // namespace process_manager
