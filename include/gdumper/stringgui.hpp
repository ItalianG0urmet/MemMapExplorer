#pragma once

#include "gui.hpp"

class StringGui : public Gui {
   public:
    StringGui(pid_t pid)
        : currentLine_(0),
          maxLine_(0),
          pid_(pid) {}

   private:
    // Override
    void onResize() override;
    std::expected<ReturnSignal, std::string> handleKeys(int ch) override;
    std::expected<void, std::string> onRun() override;
    std::expected<void, std::string> beforeRefresh() override;

    // Render var
    int currentLine_ = 0;
    int maxLine_ = 0;

    // Settings
    pid_t pid_ = 0;

};
