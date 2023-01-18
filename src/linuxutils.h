#ifdef __unix__

#ifndef LINUXUTILS_H
#define LINUXUTILS_H

// get (and open) the fd of the current keyboard or -1 on failure
int open_keyboard();

#endif // LINUXUTILS_H

#endif // __unix__