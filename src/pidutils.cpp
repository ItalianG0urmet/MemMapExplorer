#include "gdumper/pidutils.hpp"

#include <signal.h>

#include <cerrno>

namespace pid_utils {

bool pidExists(pid_t pid) {
    if (pid <= 0) return false;
    errno = 0;
    int r = kill(pid, 0);
    if (r == 0) return true;
    if (r == -1 && errno == EPERM) return true;
    return false;
}

std::expected<pid_t, std::string> validatePid(pid_t pid) noexcept {
    if (pid <= 0) return std::unexpected(std::string{"Invalid PID (must be positive)"});
    if (!pidExists(pid)) return std::unexpected(std::string{"No process with PID " + std::to_string(pid)});
    return pid;
}

}  // namespace pid_utils

