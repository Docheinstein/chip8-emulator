#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "debug.h"

static const int PROC_DEVICES_FILE_SIZE = 32768;

int open_keyboard() {
    static const char *proc_devices = "/proc/bus/input/devices";

    int devs_fd = open(proc_devices, O_RDONLY);
    if (devs_fd < 0)
        return -1;

    char devs_buffer[PROC_DEVICES_FILE_SIZE];

    auto n = read(devs_fd, devs_buffer, sizeof(devs_buffer) - 1);
    if (n < strlen("EV=120013Handlers=eventX"))
        return -1;

    std::string devs = devs_buffer;

    auto ev120013_pos = devs.find("EV=120013");
    if (ev120013_pos == std::string::npos)
        return -1;

    auto handlers_pos = devs.rfind("Handlers=", ev120013_pos);

    if (handlers_pos == std::string::npos)
        return -1;

    auto event_pos = devs.find("event", handlers_pos);

    if (event_pos == std::string::npos)
        return -1;

    auto cursor = event_pos + strlen("event");
    std::string event_num;

    while (cursor < devs.size()) {
        char ch = devs[cursor++];
        if (!isdigit(ch))
            break;
        event_num += ch;
    }

    std::string dev_input_event = "/dev/input/event" + event_num;

    DEBUG() << "Keyboard found at " << dev_input_event << std::endl;

    return open(dev_input_event.c_str(), O_RDONLY);
}
