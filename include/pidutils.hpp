#pragma once

#include <csignal>
#include <iostream>
#include <string>

namespace pid_utils {

bool pidExist(int pid) { return kill(pid, 0) != 0; }

bool validatePid(std::string pid) {
    std::string mapPath = "/proc/" + pid + "/maps";
    for (char const& c : pid) {
        if (!std::isdigit(c)) {
            std::cerr << "PID must contain only numbers \n";
            return false;
        }
    }
    try {
        if (pid.empty()) throw std::invalid_argument("No PID provided");
        int pidVal = std::stoi(pid);
        if (pidExist(pidVal)) {
            std::cerr << "Invalid PID: " << pidVal << std::endl;
            return false;
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

}  // namespace pid_utils
