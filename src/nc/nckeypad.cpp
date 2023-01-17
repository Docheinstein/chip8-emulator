#ifdef CURSES_ENABLED

#include <ncurses.h>
#include "nckeypad.h"
#include "debug.h"

#include <linux/input.h>
#include <fcntl.h>
#include "linuxutils.h"
#include "flags.h"

/*
 *     CHIP-8             Keyboard
 *  1 | 2 | 3 | C       1 | 2 | 3 | 4
 *  4 | 5 | 6 | D       Q | W | E | R
 *  7 | 8 | 9 | E       A | S | D | F
 *  A | 0 | B | F       Z | X | C | V
 */

static const int  CHIP8_KEYS_TO_LINUX_KEYS[] = {
    KEY_X, // 0x0
    KEY_1, // 0x1
    KEY_2, // 0x2
    KEY_3, // 0x3
    KEY_Q, // 0x4
    KEY_W, // 0x5
    KEY_E, // 0x6
    KEY_A, // 0x7
    KEY_S, // 0x8
    KEY_D, // 0x9
    KEY_Z, // 0xA
    KEY_C, // 0xB
    KEY_4, // 0xC
    KEY_R, // 0xD
    KEY_F, // 0xE
    KEY_V, // 0xF
};

NcKeypad::NcKeypad(const std::shared_ptr<NcWindow> &window) : window(window) {
    cbreak();
    nodelay(stdscr, TRUE);
    keyboard_fd = open_keyboard();
    if (keyboard_fd < 0)
        throw std::runtime_error("failed to get keyboard file descriptor");
    int flags = fcntl(keyboard_fd, F_GETFL, 0);
    fcntl(keyboard_fd, F_SETFL, flags | O_NONBLOCK);
}

NcKeypad::~NcKeypad() {
    nocbreak();
    nodelay(stdscr, FALSE);
}

void NcKeypad::read_keys() {
    static const int NUM_EVENTS = 8;
	struct input_event inputs[NUM_EVENTS];

    auto n = read(keyboard_fd, inputs, sizeof(struct input_event) * NUM_EVENTS);
    if (n < 0)
        return;
    if (n < (int) sizeof(struct input_event))
        return;
    for (int i = 0; i < n / sizeof(struct input_event); i++) {
        auto ev = inputs[i];

        if (ev.type == EV_KEY) {
            auto key = ev.code;
            for (int k = 0; k < CHIP8_KEYPAD_KEYS; k++) {
                if (CHIP8_KEYS_TO_LINUX_KEYS[k] == key) {
                    DEBUG() << "Key " << std::hex << k << " " << (ev.value == 0 ? "down" : "up") << std::endl;
                    keys_state[k] = ev.value == 0 ? Keypad::KEY_STATE_RELEASED : Keypad::KEY_STATE_PRESSED;
                }
            }

            if (key == KEY_ESC && ev.value != 0)
                shutdown_flag = true;
            else if (key == KEY_SPACE && ev.value != 0)
                reset_flag = true;
        }
    }
}

Keypad::KeyState NcKeypad::get(int key) {
    /*
     * Consideration: right now read_keys is called on NcKeypad::get()
     * therefore at a rate decided by the CPU's instructions per second.
     * Is this ok, or maybe it's better transfer the responsibility to a
     * task with a different rate?
     */
    read_keys();
    return keys_state[key];
}

#endif // CURSES_ENABLED