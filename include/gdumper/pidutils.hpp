#pragma once

#include <string>
#include <expected>
#include <sys/types.h>

namespace pid_utils {

bool pidExists(pid_t pid);
std::expected<pid_t, std::string> validatePid(pid_t pid) noexcept;

}  // namespace pid_utils
