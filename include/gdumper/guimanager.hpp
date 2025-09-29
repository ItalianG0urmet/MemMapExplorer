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

   private:
    bool running_ = false;
    bool showFullPath_ = false;

    std::string onlyFindString_{};
    std::string inputBuffer_{};
    std::vector<std::string> strings_;

    int currentLine_ = 0;
    int maxLine_ = 0;
    int lastLines_ = 0;
    int lastCols_ = 0;
    pid_t pid_ = 0;

    void handleResize();
    void initColors() const;
    void createBox() const;
    void loadLines() const;
    void drawHeader() const;
    void drawFooter() const;
    std::expected<void, std::string> handleSearch(const std::string& path);
    std::expected<void, std::string> reloadStrings(const std::string& path);

    enum ColorTheme { FRAME = 1, HEADER, HIGHLIGHT, LINE_NUMBER, DEFAULT_TEXT };
};
