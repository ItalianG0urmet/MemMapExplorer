#include <iostream>

#include "gdumper/argsmanager.hpp"
#include "gdumper/guimanager.hpp"
#include "gdumper/pidutils.hpp"

int main(int argc, char* argv[]) {
    auto argsOrError = Args::parse(argc, argv);
    if (!argsOrError) {
        std::cerr << argsOrError.error() << "\n";
        return EXIT_FAILURE;
    }
    Args args = argsOrError.value();

    auto pidOrErr = pid_utils::validatePid(args.pid);
    if (!pidOrErr) {
        std::cerr << pidOrErr.error() << "\n";
        return EXIT_FAILURE;
    }

    GuiManager gui(pidOrErr.value(), args.onlyFindString, args.showFullPath);
    if (auto check = gui.run(); !check) {
        std::cerr << check.error() << "\n";
    }

    return EXIT_SUCCESS;
}

