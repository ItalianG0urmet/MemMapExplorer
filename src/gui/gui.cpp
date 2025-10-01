#include "gdumper/gui/gui.hpp"

#include <ncurses.h>
#include <unistd.h>

#include <algorithm>
#include <expected>
#include <string>

void Gui::handleResize() {
    onResize();
    erase();
    createBox();
    refresh();
}

std::expected<void, std::string> Gui::init() {

    if (const auto check = onRun(); !check) {
        return std::unexpected(check.error());
    }

    if (has_colors()) {
        start_color();
        initColors();
    }

    return {};
}

std::expected<Gui::ReturnSignal, std::string> Gui::render() {
    erase();
    createBox();
    if (const auto check = beforeRefresh(); !check) {
        return std::unexpected(check.error());
    }
    refresh();

    const int ch{getch()};
    if (const auto check = handleKeys(ch); !check) {
        return std::unexpected(check.error());
    } else {
        return check.value();
    }
}

void Gui::initColors() const {
    init_pair(FRAME, COLOR_WHITE, COLOR_BLACK);
    init_pair(HEADER, COLOR_BLACK, COLOR_WHITE);
    init_pair(DEFAULT_TEXT, COLOR_WHITE, COLOR_BLACK);
    init_pair(LINE_NUMBER, COLOR_CYAN, COLOR_BLACK);
    init_pair(HIGHLIGHT, COLOR_YELLOW, COLOR_BLACK);
}

void Gui::createBox() const {
    drawHeader();
    drawFooter();
}

void Gui::drawHeader() const {
    attron(COLOR_PAIR(HEADER));
    std::string temp = header_;
    int size = std::max(0, COLS - 4);
    temp.resize(size, ' ');
    mvprintw(HEADER_Y, LEFT_MARGIN, "%s", temp.c_str());
    attroff(COLOR_PAIR(HEADER));
}

void Gui::drawFooter() const {
    attron(COLOR_PAIR(HEADER));
    mvprintw(LINES - FOOTER_Y_OFFSET, LEFT_MARGIN, "%-*s",
             COLS - LEFT_MARGIN - RIGHT_MARGIN, footer_.c_str());
    attroff(COLOR_PAIR(HEADER));
}

