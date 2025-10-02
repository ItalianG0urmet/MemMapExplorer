#pragma once

#include <ncurses.h>

#include <expected>
#include <string>

// RAII ncruses protector
struct NcursesGuard {
    NcursesGuard() {
        setlocale(LC_ALL, "");
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);
    }
    ~NcursesGuard() { endwin(); }

    NcursesGuard(const NcursesGuard&) = delete;
    NcursesGuard operator=(const NcursesGuard&) = delete;
};

class Gui {
   public:
    virtual ~Gui() = default;
    Gui() = default;

    // Return signal for ManagerGui
    enum ReturnSignal { CONTINUE_SIG = 0, EXIT_SIG = 1, NEXT_SIG = 2 };

    // Main functions
    std::expected<void, std::string> init();
    std::expected<ReturnSignal, std::string> render();

   protected:
    // Theme var
    std::string footer_{};
    std::string header_{};
    enum ColorTheme { FRAME = 1, HEADER, HIGHLIGHT, LINE_NUMBER, DEFAULT_TEXT };

    // Margin definition
    inline static constexpr int HEADER_Y = 1;
    inline static constexpr int FOOTER_Y_OFFSET = 2;
    inline static constexpr int LEFT_MARGIN = 2;
    inline static constexpr int RIGHT_MARGIN = 4;
    inline static constexpr size_t MAX_INPUT_SIZE = 256;

    void createBox() const;

   private:
    // Gui extend functions
    virtual void onResize() = 0;
    virtual std::expected<ReturnSignal, std::string> handleKeys(int ch) = 0;
    virtual std::expected<void, std::string> onRun() = 0;
    virtual std::expected<void, std::string> beforeRefresh() = 0;

    // Gui basic functions
    void handleResize();
    void initColors() const;
    void drawFooter() const;
    void drawHeader() const;

    // Var
    NcursesGuard nguard_;
};
