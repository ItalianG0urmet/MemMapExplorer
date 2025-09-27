#pragma once
#include <ncurses.h>

#include <expected>
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

    std::expected<void, std::string> run();
    static void initColors();

   private:
    bool running_ = false;
    pid_t pid_ = 0;
    std::string onlyFindString_{};
    bool showFullPath_ = false;

    std::vector<std::string> strings_;
    int currentLine_ = 0;
    int maxLine_ = 0;
    int lastLines_ = 0;
    int lastCols_ = 0;

    void handleResize();
    void createBox() const;
    void loadLines() const;
    void drawHeader() const;
    void drawFooter() const;
    std::expected<void, std::string> reloadStrings(std::string path);

    enum ColorTheme { FRAME = 1, HEADER, HIGHLIGHT, LINE_NUMBER, DEFAULT_TEXT };
};
