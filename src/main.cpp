#include <iostream>
#include <csignal>
#include <thread>
#include <unordered_map>

#include "../include/ProcessManager.h"
#include "../include/GuiManager.h"

void loadArgs (int argc, char *argv[]);
bool pidExistVerify(int pid);
bool validatePid(std::unordered_map<int, std::string> *strings);

std::string onlyFindString;
std::string pid;

int main(const int argc, char *argv[]) {

    std::unordered_map<int, std::string> extractedPath;
    int currentLine = 1;
    int maxLineLength;

    loadArgs(argc, argv);
    if (!validatePid(&extractedPath))
        return EXIT_FAILURE;

    GuiManager guiManager(&currentLine, &maxLineLength, &extractedPath);
    std::thread guiThread(&GuiManager::run, &guiManager);
    guiThread.join();

    return EXIT_SUCCESS;
}

bool pidExistVerify(const int pid) { return (kill(pid, 0) != 0);}

bool validatePid(std::unordered_map<int, std::string> *strings) {

    std::string mapPath = "/proc/";
    try {

        if (pid.empty()) {
            std::cerr << "You must provide a process ID" << std::endl;
            return false;
        }

        int pidVal = std::stoi(pid);
        if (pidExistVerify(pidVal)) {
            std::cerr << "Invalid pid: " << pidVal << std::endl;
            return false;
        }

        mapPath.append(pid + "/maps");
        if (!fileManager(mapPath, onlyFindString, strings)) {
            return false;
        }
    } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid PID format" << std::endl;
        return false;
    }
    return true;
}

void loadArgs (const int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        switch (opt) {
            case 'p':
                pid = optarg;
            break;
            case '?':
                default:
                    std::cerr << "Use: " << argv[0] << " -p <PID> [-f <arg>]\n";
            exit(EXIT_FAILURE);
        }
    }
}

