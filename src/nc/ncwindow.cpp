#ifdef CURSES_ENABLED

#include "ncwindow.h"
#include "ncurses.h"

NcWindow::NcWindow() {
    initscr();
}

NcWindow::~NcWindow() {
	endwin();
}

NcWindow &NcWindow::operator=(NcWindow &&other)  noexcept {
    return *this;
}

NcWindow::NcWindow(NcWindow &&other) noexcept{

}

#endif // CURSES_ENABLED