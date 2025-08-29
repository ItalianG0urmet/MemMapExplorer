#include "gdumper/argsmanager.hpp"

#include <unistd.h>

#include <stdexcept>

std::expected<Args, std::string> Args::parse(int argc, char* argv[]) {
    Args args;
    int opt;

    bool pidGiven = false;

    opterr = 0;
    while ((opt = getopt(argc, argv, "p:f:a")) != -1) {
        switch (opt) {
            case 'p':
                try {
                    args.pid = std::stol(optarg);
                    pidGiven = true;
                } catch (const std::invalid_argument&) {
                    return std::unexpected(std::string("Invalid PID (not a number): ") +
                                           optarg);
                } catch (const std::out_of_range&) {
                    return std::unexpected(std::string("PID out of range: ") + optarg);
                }
                break;

            case 'f':
                if (!optarg || *optarg == '\0')
                    return std::unexpected("Argument for -f missing");
                args.onlyFindString = optarg;
                break;

            case 'a':
                args.showFullPath = true;
                break;

            case '?':
            default:
                return std::unexpected("Use: -p <PID> [-f <filter>] [-a]");
        }
    }

    if (!pidGiven) {
        return std::unexpected("Use: -p <PID> [-f <filter>] [-a]");
    }
    return args;
}
