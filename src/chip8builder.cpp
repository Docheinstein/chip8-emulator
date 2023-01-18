#include "chip8builder.h"
#include "gl/gldisplay.h"
#include "gl/glkeypad.h"
#include "nc/ncwindow.h"
#include "nc/ncdisplay.h"
#include "nc/nckeypad.h"
#include "soloud/soloudspeaker.h"
#include "dummy/dummyspeaker.h"
#include "dummy/dummydisplay.h"
#include "dummy/dummykeypad.h"

constexpr int USE_DEFAULT = -1;
constexpr int DEFAULT_CPU_INSTRUCTIONS_PER_SECONDS = 700;

constexpr int DEFAULT_OPENGL_HORIZONTAL_SCALING = 15;
constexpr int DEFAULT_OPENGL_VERTICAL_SCALING = 15;

constexpr int DEFAULT_CURSES_HORIZONTAL_SCALING = 1;
constexpr int DEFAULT_CURSES_VERTICAL_SCALING = 1;

Chip8Builder &Chip8Builder::Chip8Builder::set_core(Chip8Builder::CoreType core_) {
    core = core_;
    return *this;
}

Chip8Builder &Chip8Builder::Chip8Builder::set_audio(Chip8Builder::AudioType audio_) {
    audio = audio_;
    return *this;
}

Chip8Builder &Chip8Builder::Chip8Builder::set_instructions_per_second(int instructions_per_second_) {
    instructions_per_second = instructions_per_second_;
    return *this;
}

Chip8Builder &Chip8Builder::Chip8Builder::set_horizontal_scaling(int scaling) {
    horizontal_scaling = scaling;
    return *this;
}

Chip8Builder &Chip8Builder::Chip8Builder::set_vertical_scaling(int scaling) {
    vertical_scaling = scaling;
    return *this;
}

Chip8Builder::Chip8Builder() :
    core(CoreType::Dummy),
    audio(AudioType::Dummy),
    instructions_per_second(USE_DEFAULT),
    horizontal_scaling(USE_DEFAULT),
    vertical_scaling(USE_DEFAULT) {

}

Chip8 Chip8Builder::Chip8Builder::build() const {
    Chip8 chip8(instructions_per_second == USE_DEFAULT ? DEFAULT_CPU_INSTRUCTIONS_PER_SECONDS : instructions_per_second);
    switch (core) {
        case CoreType::Dummy:
            chip8.display = std::make_unique<DummyDisplay>();
            chip8.keypad = std::make_unique<DummyKeypad>();
            break;
#ifdef OPENGL_ENABLED
        case CoreType::OpenGL: {
            int hscaling = horizontal_scaling == USE_DEFAULT ? DEFAULT_OPENGL_HORIZONTAL_SCALING : horizontal_scaling;
            int vscaling = vertical_scaling == USE_DEFAULT ? DEFAULT_OPENGL_VERTICAL_SCALING : vertical_scaling;

            auto glwin = std::make_shared<GlWindow>(CHIP8_DISPLAY_WIDTH * hscaling,
                                                    CHIP8_DISPLAY_HEIGHT * vscaling, "Chip8");
            chip8.display = std::make_unique<GlDisplay>(glwin, hscaling, vscaling);
            chip8.keypad = std::make_unique<GlKeypad>(glwin);
            break;
        }
#endif
#ifdef CURSES_ENABLED
        case CoreType::Curses: {
            int hscaling = 2 * (horizontal_scaling == USE_DEFAULT ? DEFAULT_CURSES_HORIZONTAL_SCALING : horizontal_scaling);
            int vscaling = vertical_scaling == USE_DEFAULT ? DEFAULT_CURSES_VERTICAL_SCALING : vertical_scaling;
            auto ncwin = std::make_shared<NcWindow>();
            chip8.display = std::make_unique<NcDisplay>(ncwin, hscaling, vscaling, true);
            chip8.keypad = std::make_unique<NcKeypad>(ncwin);
            break;
        }
#endif
    }

    switch (audio) {
        case AudioType::Dummy:
            chip8.speaker = std::make_unique<DummySpeaker>();
            break;
#ifdef SOLOUD_ENABLED
        case AudioType::SoLoud:
            chip8.speaker = std::make_unique<SoLoudSpeaker>();
            break;
#endif
    }

    chip8.memory = std::make_unique<Memory>();
    chip8.cpu = std::make_unique<CPU>(*chip8.memory, *chip8.display, *chip8.keypad, *chip8.speaker);

    return chip8;
}