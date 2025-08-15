#pragma once

#include <string>

class Args {
    public:
        std::string pid;
        std::string onlyFindString;
        bool showFullPath;
        Args(int argc, char* argv[]);
};
