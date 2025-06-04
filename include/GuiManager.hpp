#pragma once
#include <vector>
#include <string>
#include <ncurses.h>

class GuiManager {
public:
    GuiManager(int* currentLine, int* maxLine, std::vector<std::string>* strings);
    void run() const;
    static void initColors();

private:
    void createBox() const;
    void loadLines() const;
    void drawHeader() const;
    void drawFooter() const;
    void handleResize() const;

    std::vector<std::string>* strings;
    int* currentLine;
    int* maxLine;

    enum ColorTheme {
        FRAME = 1,
        HEADER,
        HIGHLIGHT,
        LINE_NUMBER,
        DEFAULT_TEXT
    };
};