#pragma once
#include <ncurses.h>

#include <string>
#include <vector>
#include <expected>

class GuiManager {
   public:
    GuiManager(pid_t pid, std::string onlyFindString, bool showFullPath)
        : pid_(pid),
          onlyFindString_(onlyFindString),
          showFullPath_(showFullPath),
          currentLine_(0),
          maxLine_(0) {}
    std::expected<void, std::string> run();
    static void initColors();

   private:
    bool running_;
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
