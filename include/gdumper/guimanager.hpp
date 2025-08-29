#pragma once
#include <ncurses.h>

#include <string>
#include <vector>

class GuiManager {
   public:
    GuiManager(pid_t pid, std::string onlyFindString, bool showFullPath)
        : pid_(pid),
          onlyFindString_(onlyFindString),
          showFullPath_(showFullPath),
          currentLine_(0),
          maxLine_(0) {}
    void run();
    static void initColors();

   private:
    pid_t pid_;
    std::string onlyFindString_;
    bool showFullPath_;

    std::vector<std::string> strings_;
    int currentLine_;
    int maxLine_;

    void createBox() const;
    void loadLines() const;
    void drawHeader() const;
    void drawFooter() const;
    void handleResize() const;

    enum ColorTheme { FRAME = 1, HEADER, HIGHLIGHT, LINE_NUMBER, DEFAULT_TEXT };
};
