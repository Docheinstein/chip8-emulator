#include "debug.h"

#ifdef DEBUG
std::ostream &debug = std::cout;
#else
std::ostream null(nullptr);
std::ostream &debug = null;
#endif