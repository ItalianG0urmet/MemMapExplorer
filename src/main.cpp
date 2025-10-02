#include <iostream>
#include <memory>

#include "gdumper/argsmanager.hpp"
#include "gdumper/gui/managergui.hpp"
#include "gdumper/gui/memgui.hpp"
#include "gdumper/pidutils.hpp"
#include "gdumper/gui/stringgui.hpp"

int main(int argc, char* argv[]) {
    // Get args
    auto argsOrError = Args::parse(argc, argv);
    if (!argsOrError) {
        std::cerr << argsOrError.error() << "\n";
        return EXIT_FAILURE;
    }
    Args args = argsOrError.value();

    // Validated pid
    auto pidOrErr = pidUtils::validatePid(args.pid);
    if (!pidOrErr) {
        std::cerr << pidOrErr.error() << "\n";
        return EXIT_FAILURE;
    }

    // Start gui
    ManagerGui managergui;

    managergui.addModule(
        std::make_unique<StringGui>(pidOrErr.value())
    );
    managergui.addModule(
        std::make_unique<MemGui>(pidOrErr.value(), args.onlyFindString, args.showFullPath)
    );

    if (auto check = managergui.init(); !check) {
        std::cerr << check.error() << "\n";
        return EXIT_FAILURE;
    }
    if (auto check = managergui.run(); !check) {
        std::cerr << check.error() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

