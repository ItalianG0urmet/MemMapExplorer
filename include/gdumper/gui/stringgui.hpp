#pragma once

#include <expected>
#include <string>
#include <vector>

#include "gdumper/gui/gui.hpp"

class StringGui : public Gui {
   public:
    StringGui(pid_t pid) : currentLine_(0), maxLine_(0), pid_(pid) {}

   private:
    // Override null
    void onResize() override {}

    // Override
    std::expected<ReturnSignal, std::string> handleKeys(int ch) override;
    std::expected<void, std::string> beforeRefresh() override;
    std::expected<void, std::string> onRun() override;

    // Info function
    std::expected<void, std::string> loadProgramStrings();
    void loadLines() const;

    // Render var
    int currentLine_ = 0;
    int maxLine_ = 0;

    // Settings
    std::vector<std::string> programStrings_;
    std::string exePath_{};
    pid_t pid_ = 0;
};
