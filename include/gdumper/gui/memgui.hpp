#pragma once
#include <ncurses.h>

#include <expected>
#include <string>
#include <vector>

#include "gdumper/gui/gui.hpp"

class MemGui : public Gui {
   public:
    MemGui(pid_t pid, std::string onlyFindString, bool showFullPath)
        : currentLine_(0),
          maxLine_(0),
          pid_(pid),
          onlyFindString_(onlyFindString),
          showFullPath_(showFullPath) {}

   private:
    // Override
    void onResize() override;
    std::expected<ReturnSignal, std::string> handleKeys(int ch) override;
    std::expected<void, std::string> onRun() override;
    std::expected<void, std::string> beforeRefresh() override;

    // Render
    std::expected<void, std::string> handleSearch(const std::string& path);
    std::expected<void, std::string> reloadStrings(const std::string& path);
    void reloadHeader();
    void loadLines() const;

    // Render var
    int currentLine_ = 0;
    int maxLine_ = 0;
    std::vector<std::string> strings_;

    // Settings
    pid_t pid_ = 0;
    std::string onlyFindString_{};
    std::string inputBuffer_{};
    bool showFullPath_ = false;

};
