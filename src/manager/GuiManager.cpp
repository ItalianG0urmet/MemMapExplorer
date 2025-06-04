#include "../../include/GuiManager.hpp"
#include "../../include/Globals.hpp"
#include <locale.h>
#include <signal.h>

void sigwinchHandler(int) {
    endwin();
    refresh();
}


void GuiManager::initColors() {
    start_color();
    init_pair(FRAME, COLOR_CYAN, COLOR_BLACK);
    init_pair(HEADER, COLOR_WHITE, COLOR_BLUE);
    init_pair(HIGHLIGHT, COLOR_YELLOW, COLOR_BLACK);
    init_pair(LINE_NUMBER, COLOR_CYAN, COLOR_BLACK);
    init_pair(DEFAULT_TEXT, COLOR_WHITE, COLOR_BLACK);
}

GuiManager::GuiManager(int* currentLine, int* maxLine, std::vector<std::string>* strings)
    : strings(strings), currentLine(currentLine), maxLine(maxLine) {}

void GuiManager::run() const {
    setlocale(LC_ALL, "");
    signal(SIGWINCH, sigwinchHandler);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    initColors();

    while(true) {
        clear();
        createBox();
        loadLines();
        refresh();

        int ch = getch();
        if(ch == 'q' || ch == 'Q') break;

        switch(ch) {
            case KEY_UP:
                if(*currentLine > 0) (*currentLine)--;
                break;
            case KEY_DOWN:
                if(*currentLine < static_cast<int>(strings->size()) - *maxLine)
                    (*currentLine)++;
                break;
        }
    }
    endwin();
}

void GuiManager::createBox() const {
    attron(COLOR_PAIR(FRAME));
    border('|', '|', '-', '-', '+', '+', '+', '+');

    std::string title = " Process Memory Mapper ";
    mvprintw(0, (COLS - title.length())/2, "%s", title.c_str());

    drawHeader();
    drawFooter();
}

void GuiManager::drawHeader() const {
    attron(COLOR_PAIR(HEADER));

    std::string header = " PID: " + pid + " ";
    if(!onlyFindString.empty())
        header += "| Filter: " + onlyFindString + " ";

    header.resize(COLS - 4, ' ');
    mvprintw(1, 2, "%s", header.c_str());
}

void GuiManager::drawFooter() const {
    attron(COLOR_PAIR(HEADER));
    std::string footer = "[Arrows] Navigate | [Q] Quit";
    mvprintw(LINES-2, 2, "%-*s", COLS-4, footer.c_str());
}

void GuiManager::loadLines() const {
    int maxVisibleLines = LINES - 5;
    int start = *currentLine;
    int end = std::min(start + maxVisibleLines, static_cast<int>(strings->size()));

    for(int i = 0; i < end - start; i++) {
        int lineY = 3 + i;
        int globalIndex = start + i;

        attron(COLOR_PAIR(LINE_NUMBER));
        mvprintw(lineY, 2, "%04d", globalIndex + 1);

        attron(COLOR_PAIR(DEFAULT_TEXT));

        std::string text = (*strings)[globalIndex];
        text.resize(COLS - 10, ' ');
        mvprintw(lineY, 10, "%s", text.c_str());

        if(globalIndex == *currentLine) {
            attron(COLOR_PAIR(HIGHLIGHT));
            mvprintw(lineY, 8, ">");
            attroff(COLOR_PAIR(HIGHLIGHT));
        }
    }
}
