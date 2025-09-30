#include "gdumper/managergui.hpp"

#include <expected>

#include "gdumper/gui.hpp"

std::expected<void, std::string> ManagerGui::init() {
    if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
        return std::unexpected("TTY not supported");
    }

    for (auto& g : guiList_) {
        if (const auto check = g.second->init(); !check) {
            return std::unexpected(check.error());
        }
    }
    return {};
}

std::expected<void, std::string> ManagerGui::run() {
    if (running_) {
        return std::unexpected("ManagerGui already running");
    }

    auto renderGui = [](Gui& gui,
                        bool& running_) -> std::expected<void, std::string> {
        auto ret = gui.render();
        if (!ret) {
            return std::unexpected(ret.error());
        }

        switch (ret.value()) {
            case Gui::EXIT_SIG:
                running_ = false;
                break;
            case Gui::NEXT_SIG:
                // TODO: manage switch focus
                break;
            case Gui::CONTINUE_SIG:
                return {};
        }

        return {};
    };

    if (focusGuiIndex_ == -1) {
        focusGuiIndex_ = guiList_.begin()->first;
    }

    running_ = true;
    while (running_) {
        auto it = guiList_.find(focusGuiIndex_);
        if (it == guiList_.end()) {
            return std::unexpected("Invalid gui index");
        }

        if (auto check = renderGui(*it->second, running_); !check) {
            return std::unexpected(check.error());
        }
    }

    return {};
}

void ManagerGui::addModule(std::unique_ptr<Gui>&& gui) {
    guiList_.emplace(guiCount() + 1, std::move(gui));
}

int ManagerGui::guiCount() const { return static_cast<int>(guiList_.size()); }

void ManagerGui::changeFocusGui(int index) {
    focusGuiIndex_ = (guiList_.find(index) != guiList_.end()) ? index : -1;
}

