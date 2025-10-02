#include "gdumper/gui/stringgui.hpp"

#include <expected>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "gdumper/gui/gui.hpp"

std::expected<void, std::string> StringGui::loadProgramStrings() {

    std::ifstream file(exePath_, std::ios::binary);
    if (!file) {
        return std::unexpected("Can't read the file strings");
    }

    std::vector<unsigned char> buffer((std::istreambuf_iterator<char>(file)),
                                      std::istreambuf_iterator<char>());

    std::string temp;
    std::vector<std::string> strings;

    for (unsigned char c : buffer) {
        if (isprint(c) || c == ' ' || c == '\t') {
            temp.push_back(c);
        } else {
            if (temp.size() >= 4) {
                strings.push_back(temp);
            }
            temp.clear();
        }
    }

    if (temp.size() >= 4) {
        strings.push_back(temp);
    }

    programStrings_ = strings;

    return {};
}

std::expected<Gui::ReturnSignal, std::string> StringGui::handleKeys(int ch) {
    switch (ch) {
        case 'q':
            return EXIT_SIG;
            break;
        case '\t':
            return NEXT_SIG;
            break;
        case KEY_UP:
        case 'k':
            if (currentLine_ > 0) currentLine_--;
            break;
        case KEY_DOWN:
        case 'j':
            if (programStrings_.size() <= 0) break;
            currentLine_ = std::min(
                currentLine_ + 1,
                static_cast<int>(programStrings_.size()) - maxLine_ - 1);
            break;
    }
    return CONTINUE_SIG;
}

std::expected<void, std::string> StringGui::onRun() {
    footer_ = "[Arrows] Navigate | [q] Quit";
    header_ = " Strings | PID: " + std::to_string(pid_);
    exePath_ = "/proc/" + std::to_string(pid_) + "/exe";

    if (const auto check = loadProgramStrings(); !check) {
        return std::unexpected(check.error());
    }

    return {};
}

void StringGui::loadLines() const {
    const int maxVisibleLines{LINES - 6};
    const int start{currentLine_};
    const int end{std::min(start + maxVisibleLines,
                           static_cast<int>(programStrings_.size()))};

    for (int i = 0; i < end - start; i++) {
        int lineY = 3 + i;
        int globalIndex = start + i;

        attron(COLOR_PAIR(LINE_NUMBER));
        mvprintw(lineY, 2, "%04d", globalIndex + 1);

        attron(COLOR_PAIR(DEFAULT_TEXT));
        std::string text = programStrings_[globalIndex];
        text.resize(COLS - 10, ' ');
        mvprintw(lineY, 10, "%s", text.c_str());

        if (globalIndex == currentLine_) {
            attron(COLOR_PAIR(HIGHLIGHT));
            mvprintw(lineY, 8, ">");
            attroff(COLOR_PAIR(HIGHLIGHT));
        }
    }
}

std::expected<void, std::string> StringGui::beforeRefresh() {
    loadLines();
    return {};
}
