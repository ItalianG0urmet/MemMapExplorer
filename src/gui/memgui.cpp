#include "gdumper/gui/memgui.hpp"

#include <ncurses.h>
#include <unistd.h>

#include <expected>
#include <string>

#include "gdumper/gui/gui.hpp"
#include "gdumper/processmanager.hpp"

void MemGui::reloadHeader() {
    header_ = " PID: " + std::to_string(pid_);
    if (!onlyFindString_.empty()) header_ += " | Filter: " + onlyFindString_;
}

std::expected<void, std::string> MemGui::onRun() {
    reloadHeader();
    footer_ =
        "[Arrows] Navigate | [q] Quit | [r] Reload | [p] Toggle path | [/] "
        "Change filter";
    const std::string path{"/proc/" + std::to_string(pid_) + "/maps"};
    auto linesOrErr =
        processManager::getFormattedLines(path, onlyFindString_, showFullPath_);
    if (!linesOrErr) return std::unexpected("Error opening map file: " + path);

    strings_ = linesOrErr.value();
    currentLine_ = 0;

    return {};
}

std::expected<Gui::ReturnSignal, std::string> MemGui::handleKeys(int ch) {
    const std::string path{"/proc/" + std::to_string(pid_) + "/maps"};

    switch (ch) {
        case '\t':
            return NEXT_SIG;
            break;
        case 'q':
            return EXIT_SIG;
            break;
        case 'r':
            if (const auto check = reloadStrings(path); !check) {
                return std::unexpected(check.error());
            }
            break;
        case 'p':
            showFullPath_ = !showFullPath_;
            if (const auto check = reloadStrings(path); !check) {
                return std::unexpected(check.error());
            }
            break;
        case '/':
            if (const auto check = handleSearch(path); !check) {
                return std::unexpected(check.error());
            }
            break;
        case KEY_UP:
        case 'k':
            if (currentLine_ > 0) currentLine_--;
            break;
        case KEY_DOWN:
        case 'j':
            currentLine_ =
                std::min(currentLine_ + 1,
                         static_cast<int>(strings_.size()) - maxLine_ - 1);
            break;
    }
    return CONTINUE_SIG;
}

void MemGui::onResize() {
    maxLine_ = std::max(0, LINES - 5);
    if (currentLine_ > static_cast<int>(strings_.size()) - maxLine_)
        currentLine_ =
            std::max(0, static_cast<int>(strings_.size()) - maxLine_);
}

std::expected<void, std::string> MemGui::reloadStrings(
    const std::string& path) {
    auto linesOrErr =
        processManager::getFormattedLines(path, onlyFindString_, showFullPath_);
    if (!linesOrErr) return std::unexpected("Error opening map file: " + path);

    strings_ = linesOrErr.value();
    currentLine_ = 0;
    createBox();
    loadLines();
    return {};
}

std::expected<void, std::string> MemGui::beforeRefresh() {
    loadLines();
    return {};
}

std::expected<void, std::string> MemGui::handleSearch(const std::string& path) {
    inputBuffer_.clear();
    bool writing = true;

    auto drawPrompt = [&]() {
        attron(COLOR_PAIR(DEFAULT_TEXT));
        mvprintw(2, 2, "/");
        mvprintw(2, 3, "%-*s", COLS - 4, inputBuffer_.c_str());
        move(2, 3 + inputBuffer_.size());
        attroff(COLOR_PAIR(DEFAULT_TEXT));
        refresh();
    };

    curs_set(1);
    drawPrompt();

    while (writing) {
        int chn = getch();
        switch (chn) {
            case 10:
            case KEY_ENTER:
                onlyFindString_ = inputBuffer_;
                if (auto check = reloadStrings(path); !check)
                    return std::unexpected(check.error());
                writing = false;
                break;
            case 27:  // Escape
                writing = false;
                break;
            case KEY_BACKSPACE:
            case 127:
                if (!inputBuffer_.empty()) {
                    inputBuffer_.pop_back();
                    drawPrompt();
                }
                break;
            default:
                if (isprint(chn) && inputBuffer_.size() < MAX_INPUT_SIZE) {
                    inputBuffer_ += static_cast<char>(chn);
                    drawPrompt();
                }
                break;
        }
    }

    reloadHeader();
    attron(COLOR_PAIR(FRAME));
    mvprintw(1, 2, "%-*s", COLS - 4, header_.c_str());
    attroff(COLOR_PAIR(FRAME));

    curs_set(0);
    return {};
}

void MemGui::loadLines() const {
    const int maxVisibleLines{LINES - 6};
    const int start{currentLine_};
    const int end{
        std::min(start + maxVisibleLines, static_cast<int>(strings_.size()))};

    for (int i = 0; i < end - start; i++) {
        int lineY = 3 + i;
        int globalIndex = start + i;

        attron(COLOR_PAIR(LINE_NUMBER));
        mvprintw(lineY, 2, "%04d", globalIndex + 1);

        attron(COLOR_PAIR(DEFAULT_TEXT));
        std::string text = strings_[globalIndex];
        text.resize(COLS - 10, ' ');
        mvprintw(lineY, 10, "%s", text.c_str());

        if (globalIndex == currentLine_) {
            attron(COLOR_PAIR(HIGHLIGHT));
            mvprintw(lineY, 8, ">");
            attroff(COLOR_PAIR(HIGHLIGHT));
        }
    }
}

