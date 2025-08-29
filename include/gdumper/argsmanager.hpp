#pragma once

#include <string>
#include <expected>
#include <sys/types.h>

class Args {
    public:
        pid_t pid = 0;
        std::string onlyFindString = {};
        bool showFullPath = false;
        static std::expected<Args, std::string> parse(int argc, char* argv[]);
};
