#pragma once

#include <expected>
#include <memory>
#include <unordered_map>

#include "gdumper/gui.hpp"

class ManagerGui {
   public:
    // Main function
    std::expected<void, std::string> init();
    std::expected<void, std::string> run();

    // Module functions
    void addModule(std::unique_ptr<Gui>&& gui);

   private:

    bool running_{false};

    // Gui list functions
    std::unordered_map<int, std::unique_ptr<Gui>> guiList_;
    int focusGuiIndex_ = -1;
    void changeFocusGui(int index);
    int guiCount() const;
};
