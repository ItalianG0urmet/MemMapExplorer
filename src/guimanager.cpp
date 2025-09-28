#include "gdumper/guimanager.hpp"

#include <unistd.h>

#include <expected>

#include "gdumper/processmanager.hpp"

std::expected<void, std::string> GuiManager::reloadStrings(std::string path) {
    auto pathsOrErr =
        processManager::getFormattedLines(path, onlyFindString_, showFullPath_);
    if (!pathsOrErr) {
        return std::unexpected("Error opening map file: " + path);
    }
    strings_ = pathsOrErr.value();
    currentLine_ = 0;
    erase();
    createBox();
    loadLines();
    refresh();
    return {};
}

void GuiManager::handleResize() {
    maxLine_ = std::max(0, LINES - 5);
    if (currentLine_ >
        std::max(0, static_cast<int>(strings_.size()) - maxLine_)) {
        currentLine_ =
            std::max(0, static_cast<int>(strings_.size()) - maxLine_);
    }
    erase();
    createBox();
    loadLines();
    refresh();
}

std::expected<void, std::string> GuiManager::run() {
    const std::string path{"/proc/" + std::to_string(pid_) + "/maps"};

    auto pathsOrErr =
        processManager::getFormattedLines(path, onlyFindString_, showFullPath_);
    if (!pathsOrErr) {
        return std::unexpected("Error opening map file: " + path);
    }
    strings_ = pathsOrErr.value();

    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        return std::unexpected("TTY not supported");
    }

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        initColors();
    }

    int lastLines = LINES;
    int lastCols = COLS;

    running_ = true;
    while (running_) {
        if (LINES != lastLines || COLS != lastCols) {
            lastLines = LINES;
            lastCols = COLS;
            handleResize();
        }

        erase();
        createBox();
        loadLines();
        refresh();

        const int ch{getch()};
        switch (ch) {
            case 'r': {  // Reload
                if (auto check = reloadStrings(path); !check) {
                    return std::unexpected(check.error());
                }
                break;
            }
            case 'p': {  // Switch Paths
                showFullPath_ = !showFullPath_;
                if (auto check = reloadStrings(path); !check) {
                    return std::unexpected(check.error());
                }
                break;
            }
            case 'q':  // Quit
                running_ = false;
                break;
            case 'k':
            case KEY_UP:  // Up
                if (currentLine_ > 0) currentLine_--;
                break;
            case 'j':  // Down
            case KEY_DOWN: {
                const int limit{std::max(
                    0, static_cast<int>(strings_.size()) - maxLine_ - 1)};
                if (currentLine_ < limit) ++currentLine_;
                break;
            }
        }
    }

    endwin();
    return {};
}

void GuiManager::initColors() const {
    init_pair(FRAME, COLOR_WHITE, COLOR_BLACK);
    init_pair(HEADER, COLOR_BLACK, COLOR_WHITE);
    init_pair(DEFAULT_TEXT, COLOR_WHITE, COLOR_BLACK);
    init_pair(LINE_NUMBER, COLOR_CYAN, COLOR_BLACK);
    init_pair(HIGHLIGHT, COLOR_YELLOW, COLOR_BLACK);
}

void GuiManager::createBox() const {
    attron(COLOR_PAIR(FRAME));

    drawHeader();
    drawFooter();
}

void GuiManager::drawHeader() const {
    attron(COLOR_PAIR(HEADER));

    std::string header{" PID: " + std::to_string(pid_) + " "};
    if (!onlyFindString_.empty())
        header += "| Filter: " + onlyFindString_ + " ";

    header.resize(COLS - 4, ' ');
    mvprintw(1, 2, "%s", header.c_str());
}

void GuiManager::drawFooter() const {
    attron(COLOR_PAIR(HEADER));
    const std::string footer{
        "[Arrows] Navigate | [q] Quit | [r] Reload | [p] Toggle path"};
    mvprintw(LINES - 2, 2, "%-*s", COLS - 4, footer.c_str());
}

void GuiManager::loadLines() const {
    const int maxVisibleLines{LINES - 6};
    const int start{currentLine_};
    const int end{
        std::min(start + maxVisibleLines, static_cast<int>(strings_.size()))};

    for (int i = 0; i < end - start; i++) {
        const int lineY{3 + i};
        const int globalIndex{start + i};

        attron(COLOR_PAIR(LINE_NUMBER));
        mvprintw(lineY, 2, "%04d", globalIndex + 1);

        attron(COLOR_PAIR(DEFAULT_TEXT));

        std::string text{strings_[globalIndex]};
        text.resize(COLS - 10, ' ');
        mvprintw(lineY, 10, "%s", text.c_str());

        if (globalIndex == currentLine_) {
            attron(COLOR_PAIR(HIGHLIGHT));
            mvprintw(lineY, 8, ">");
            attroff(COLOR_PAIR(HIGHLIGHT));
        }
    }
}

