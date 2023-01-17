#ifdef CURSES_ENABLED

#ifndef NCWINDOW_H
#define NCWINDOW_H

class NcWindow {
public:
    NcWindow();
    ~NcWindow();

    NcWindow(const NcWindow &other) = delete;
    NcWindow & operator = (const NcWindow &other) = delete;

    NcWindow(NcWindow &&other) noexcept;
    NcWindow & operator = (NcWindow &&other) noexcept;
};

#endif // NCWINDOW_H

#endif // CURSES_ENABLED