#ifdef CURSES_ENABLED

#include <algorithm>
#include "ncdisplay.h"
#include "ncurses.h"

static const int BLACK_ON_WHITE = 1;
static const int WHITE_ON_BLACK = 2;


NcDisplay::NcDisplay(const std::shared_ptr<NcWindow> &window, int horizontal_scaling, int vertical_scaling, bool frame) :
    window(window), horizontal_scaling(horizontal_scaling), vertical_scaling(vertical_scaling), frame(frame) {
    start_color();
    init_pair(BLACK_ON_WHITE, COLOR_BLACK, COLOR_WHITE);
    init_pair(WHITE_ON_BLACK, COLOR_WHITE, COLOR_BLACK);
    noecho();
    curs_set(0);
}

NcDisplay::~NcDisplay() {
    echo();
    curs_set(1);
}

bool NcDisplay::get(unsigned int x, unsigned int y) const {
    return display[y  * CHIP8_DISPLAY_WIDTH + x];
}

void NcDisplay::set(unsigned int x, unsigned int y, bool on) {
    display[y  * CHIP8_DISPLAY_WIDTH + x] = on;
}

void NcDisplay::clear() {
    std::fill_n(display, sizeof(display) / sizeof(display[0]), false);
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
void NcDisplay::refresh() {
    int maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    auto printblock = [maxx, maxy](int x, int y, const char *s) {
        if (x <= maxx && y <= maxy)
            mvprintw(y, x, s);
    };

    int y, x;
    for (y = 0; y < CHIP8_DISPLAY_HEIGHT; y++) {
        for (x = 0; x < CHIP8_DISPLAY_WIDTH; x++) {
            int color = display[y  * CHIP8_DISPLAY_WIDTH + x] ? BLACK_ON_WHITE : WHITE_ON_BLACK;
            attron(COLOR_PAIR(color));
            for (int r = 0; r < vertical_scaling; r++) {
                for (int c = 0; c < horizontal_scaling; c++) {
                    printblock(x * horizontal_scaling + c, y * vertical_scaling + r, " ");
                }
            }
            attroff(COLOR_PAIR(color));
        }

        if (frame) {
            attron(COLOR_PAIR(WHITE_ON_BLACK));
            printblock(CHIP8_DISPLAY_WIDTH * horizontal_scaling, y, "|");
            attroff(COLOR_PAIR(WHITE_ON_BLACK));
        }
    }

    if (frame) {
        attron(COLOR_PAIR(WHITE_ON_BLACK));
        for (x = 0; x <= CHIP8_DISPLAY_WIDTH * horizontal_scaling; x++)
            printblock(x, y, "-");
        attroff(COLOR_PAIR(WHITE_ON_BLACK));
    }

    ::refresh();
}
#pragma clang diagnostic pop

#endif // CURSES_ENABLED