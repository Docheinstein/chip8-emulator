#ifndef CHIP8BUILDER_H
#define CHIP8BUILDER_H

#include "chip8.h"


class Chip8Builder {
public:
    enum class CoreType {
        Dummy,
#ifdef OPENGL_ENABLED
        OpenGL,
#endif
#ifdef CURSES_ENABLED
        Curses
#endif
    };

    enum class AudioType {
        Dummy,
#ifdef SOLOUD_ENABLED
        SoLoud
#endif
    };

    Chip8Builder &set_core(Chip8Builder::CoreType core);
    Chip8Builder &set_audio(Chip8Builder::AudioType audio);
    Chip8Builder &set_instructions_per_second(int instructions_per_second);
    Chip8Builder &set_horizontal_scaling(int scaling);
    Chip8Builder &set_vertical_scaling(int scaling);

    Chip8Builder();
    [[nodiscard]] Chip8 build() const;

private:
    CoreType core;
    AudioType audio;

    int instructions_per_second;
    int horizontal_scaling;
    int vertical_scaling;
};

#endif // CHIP8BUILDER_H