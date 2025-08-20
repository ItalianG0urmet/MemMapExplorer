#include "gdumper/argsmanager.hpp"

#include <iostream>
#include <unistd.h>

Args::Args(int argc, char* argv[]){
    int opt;
    while ((opt = getopt(argc, argv, "p:f:a")) != -1) {
        switch (opt) {
            case 'p':
                pid = optarg;
                break;
            case 'f':
                onlyFindString = optarg;
                break;
            case 'a':
                showFullPath = true;
                break;
            default:
                std::cerr << "Usage: " << argv[0]
                          << " -p <PID> [-f <filter>] [-a]\n";
                exit(EXIT_FAILURE);
        }
    }

    if (pid.empty()) {
        std::cerr << "Missing -p option\n";
        exit(EXIT_FAILURE);
    }
}
