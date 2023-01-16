#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#ifdef DEBUG
#define DBG() debug
#else
#define DBG() \
    if (false)\
        debug
#endif

extern std::ostream &debug;

#endif // DEBUG_H