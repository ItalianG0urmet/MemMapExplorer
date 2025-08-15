#pragma once
#include <string>
#include <vector>

namespace process_manager {

std::string lineFormatter(const std::string& defaultLine,
                          const std::string& findOnly, bool showFullPath);

std::vector<std::string> formactedLineGetter(const std::string path,
                                             const std::string findOnly,
                                             bool showFullPath);
}  // namespace process_manager
