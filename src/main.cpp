#include <iostream>
#include <csignal>
#include <thread>
#include <vector>
#include "../include/ProcessManager.hpp"
#include "../include/GuiManager.hpp"
#include "../include/Globals.hpp" 

std::string onlyFindString;
std::string pid;

void loadArgs(int argc, char* argv[]);
bool showFullPath = false;
bool pidExistVerify(int pid);
bool validatePid(std::vector<std::string>* strings);

int main(int argc, char* argv[]) {
    std::vector<std::string> extractedPath;
    int currentLine = 1;
    int maxLineLength = 20;

    loadArgs(argc, argv);
    if (!validatePid(&extractedPath)) return EXIT_FAILURE;

    GuiManager gui(&currentLine, &maxLineLength, &extractedPath);
    std::thread guiThread(&GuiManager::run, &gui);
    guiThread.join();

    return EXIT_SUCCESS;
}

bool pidExistVerify(int pid) {
    return kill(pid, 0) != 0;
}

bool validatePid(std::vector<std::string>* strings) {
    std::string mapPath = "/proc/" + pid + "/maps";
    for (char const &c : pid){
      if(!std::isdigit(c)){
        std::cerr << "PID must contain only numbers \n";
        return false;
      }
    }
    try {
        if (pid.empty()) throw std::invalid_argument("No PID provided");
        int pidVal = std::stoi(pid);
        if (pidExistVerify(pidVal)) {
            std::cerr << "Invalid PID: " << pidVal << std::endl;
            return false;
        }
        return fileManager(mapPath, onlyFindString, strings, showFullPath);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

void loadArgs(int argc, char* argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "p:f:a")) != -1) { // Nota: 'a' non ha argomento
        switch (opt) {
            case 'p': pid = optarg; break;
            case 'f': onlyFindString = optarg; break;
            case 'a': showFullPath = true; break;
            default:
                std::cerr << "Usage: " << argv[0] << " -p <PID> [-f <filter>] [-a]\n";
                exit(EXIT_FAILURE);
        }
    }
    if (pid.empty()) {
        std::cerr << "Missing -p option\n";
        exit(EXIT_FAILURE);
    }
}
