#include "gdumper/stringgui.hpp"

#include "gdumper/gui.hpp"

void StringGui::onResize() {}

std::expected<Gui::ReturnSignal, std::string> StringGui::handleKeys(int ch) {
    switch (ch) {
        case 'q':
            return EXIT_SIG;
            break;
        case '\t':
            return NEXT_SIG;
            break;
    }
    return {};
}

std::expected<void, std::string> StringGui::onRun() {
    footer_ = "[Arrows] Navigate | [q] Quit";

    currentLine_ = 0;

    return {};
}

std::expected<void, std::string> StringGui::beforeRefresh() { return {}; }

