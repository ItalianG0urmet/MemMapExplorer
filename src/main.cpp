#include "../include/argsmanager.hpp"
#include "../include/pidutils.hpp"
#include "../include/guimanager.hpp"

int main(int argc, char* argv[]) {

    Args args(argc, argv);

    if (!pid_utils::validatePid(args.pid)) return EXIT_FAILURE;

    GuiManager gui(args.pid, args.onlyFindString, args.showFullPath);
    gui.run();

    return EXIT_SUCCESS;
}

