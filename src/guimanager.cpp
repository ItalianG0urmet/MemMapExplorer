#include "../include/guimanager.hpp"

#include <locale.h>
#include <signal.h>

#include "../include/processmanager.hpp"

void sigwinchHandler(int) {
    endwin();
    refresh();
    clear();
}

void GuiManager::initColors() {
    start_color();
    init_pair(FRAME, COLOR_CYAN, COLOR_BLACK);
    init_pair(HEADER, COLOR_WHITE, COLOR_BLUE);
    init_pair(HIGHLIGHT, COLOR_YELLOW, COLOR_BLACK);
    init_pair(LINE_NUMBER, COLOR_CYAN, COLOR_BLACK);
    init_pair(DEFAULT_TEXT, COLOR_WHITE, COLOR_BLACK);
}

void GuiManager::run() {
    std::string path = "/proc/" + pid_ + "/maps";
    strings_ = process_manager::formactedLineGetter(path, onlyFindString_,
                                                    showFullPath_);

    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        fprintf(stderr, "[ERR] TTY not supported.\n");
        return;
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

    while (true) {
        maxLine_ = std::max(0, LINES - 5);

        erase();
        createBox();
        loadLines();
        refresh();

        int ch = getch();
        if (ch == 'q' || ch == 'Q') break;

        switch (ch) {
            case 'k':
            case KEY_UP:
                if (currentLine_ > 0) currentLine_--;
                break;
            case 'j':
            case KEY_DOWN:
                int limit =
                    std::max(0, static_cast<int>(strings_.size()) - maxLine_);
                if (currentLine_ < limit) ++currentLine_;
                break;
        }
    }
    endwin();
}

void GuiManager::createBox() const {
    attron(COLOR_PAIR(FRAME));
    border('|', '|', '-', '-', '+', '+', '+', '+');

    std::string title = " Process Memory Mapper ";
    mvprintw(0, (COLS - title.length()) / 2, "%s", title.c_str());

    drawHeader();
    drawFooter();
}

void GuiManager::drawHeader() const {
    attron(COLOR_PAIR(HEADER));

    std::string header = " PID: " + pid_ + " ";
    if (!onlyFindString_.empty())
        header += "| Filter: " + onlyFindString_ + " ";

    header.resize(COLS - 4, ' ');
    mvprintw(1, 2, "%s", header.c_str());
}

void GuiManager::drawFooter() const {
    attron(COLOR_PAIR(HEADER));
    std::string footer = "[Arrows] Navigate | [Q] Quit";
    mvprintw(LINES - 2, 2, "%-*s", COLS - 4, footer.c_str());
}

void GuiManager::loadLines() const {
    int maxVisibleLines = LINES - 5;
    int start = currentLine_;
    int end =
        std::min(start + maxVisibleLines, static_cast<int>(strings_.size()));

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
