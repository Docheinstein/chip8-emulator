#ifdef CURSES_ENABLED

#include "nceventlooper.h"

NcEventLooper::NcEventLooper(const std::shared_ptr<NcWindow> &window) : window(window) {

}

void NcEventLooper::poll() {

}

#endif // CURSES_ENABLED