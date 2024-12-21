#include "../../include/GuiManager.h"

#include <algorithm>
#include <iostream>
#include <ncurses.h>
#include <ostream>
#include <unistd.h>

GuiManager::GuiManager(int *currentLine, int *maxLine, std::unordered_map<int, std::string> *strings)
    : strings(strings), currentLine(currentLine), maxLine(maxLine) {
}

void GuiManager::run() const {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    }

    loadLines();
    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_UP:
                if ((*currentLine) > 1)
                    (*currentLine)--;
                break;
            case KEY_DOWN:
                if ((*currentLine) < static_cast<int>(strings->size()) - (*maxLine) + 1)
                    (*currentLine)++;
                break;
            case 'f':
                std::cout << "Skill issues";
                break;
        }
        loadLines();
        refresh();
    }

    endwin();

}

//todo: Make it more clear
void GuiManager::createBox() const {
    clear();
    box(stdscr, 0, 0);
    attron(COLOR_PAIR(1));
    mvprintw(1, 0, ">");
    attron(COLOR_PAIR(1));
    mvprintw(0, 5, " Lines: ");
    mvprintw(0, 13, "      | ");
    attron(COLOR_PAIR(2));
    printLineNumber(static_cast<int>((strings)->size()));
    attron(COLOR_PAIR(1));
    mvprintw(0, 21, "Current line: ");
    mvprintw(0, 35, "    ");
    attron(COLOR_PAIR(2));
    printCurrentLineNumber((*currentLine));
    attroff(COLOR_PAIR(2));
}

void GuiManager::loadLines() const {
    createBox();
    int lineIndex = 1;
    for (int i = (*currentLine); i < (*currentLine) + (*maxLine) && i <= static_cast<int>(strings->size()); i++) {
        mvprintw(lineIndex++, 2, (*strings)[i].c_str());
    }
    (*maxLine) = LINES - 2;
    refresh();
}

void GuiManager::printLineNumber(const int number) {
    mvprintw(0, 14, std::to_string(number).c_str());
}

void GuiManager::printCurrentLineNumber(const int number) {
    mvprintw(0, 35, std::to_string(number).c_str());
}

std::string GuiManager::getPathByLine(const int line) const {
    return (*strings)[line];
}

