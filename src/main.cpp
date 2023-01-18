#include <iostream>
#include "argparse.h"
#include "chip8builder.h"
#include "flags.h"

constexpr int DEFAULT_INT_OPTION = -1;


#if defined(OPENGL_ENABLED) and defined(CURSES_ENABLED)
#define VIDEO_MODES "'opengl', 'curses', 'dummy'"
#elif defined(OPENGL_ENABLED)
#define VIDEO_MODES "'opengl', 'dummy'"
#elif defined(CURSES_ENABLED)
#define VIDEO_MODES "'curses', 'dummy'"
#else
#define VIDEO_MODES "'dummy'"
#endif


struct chip8_arguments {
    explicit chip8_arguments() :
        instructions_per_seconds(DEFAULT_INT_OPTION),
        scaling(DEFAULT_INT_OPTION),
        no_sound() {
    }

    std::string rom;
    int instructions_per_seconds;
    int scaling;
    std::string video_mode;
    bool no_sound;
};


int main(int argc, char *argv[]) {
    chip8_arguments args;

    auto parser = argumentum::argument_parser();
    auto params = parser.params();
    params
        .add_parameter(args.rom, "rom")
        .help("ROM");
    params
        .add_parameter(args.instructions_per_seconds, "-i", "--ips")
        .nargs(1)
        .default_value(DEFAULT_INT_OPTION)
        .help("CPU instructions per seconds");
    params
        .add_parameter(args.scaling, "-s", "--scaling")
        .nargs(1)
        .default_value(DEFAULT_INT_OPTION)
        .help("Video scaling factor");
    params
        .add_parameter(args.video_mode, "-v", "--video")
        .nargs(1)
        .default_value("")
        .help("Set video mode (" VIDEO_MODES ")");
    params
        .add_parameter(args.no_sound, "-m", "--no-sound")
        .nargs(0)
        .default_value(false)
        .help("Disable sound");
    params
        .add_default_help_option();

    if (!parser.parse_args(argc, argv, 1))
        return 1;

#ifndef SOLOUD_ENABLED
    std::cerr << "WARNING: sound disabled" << std::endl;
#endif

    Chip8Builder builder;
    Chip8Builder::CoreType core = Chip8Builder::CoreType::Dummy;
    Chip8Builder::AudioType audio = Chip8Builder::AudioType::Dummy;

#if defined(OPENGL_ENABLED) && defined(CURSES_ENABLED)
    if (!args.video_mode.empty()) {
        if (args.video_mode == "opengl")
            core = Chip8Builder::CoreType::OpenGL;
        else if (args.video_mode == "curses")
            core = Chip8Builder::CoreType::Curses;
        else if (args.video_mode == "dummy")
            core = Chip8Builder::CoreType::Dummy;
        else {
            std::cerr << "ERROR: unknown video mode '" << args.video_mode << "'" << std::endl;
            return -1;
        }
    } else
        core = Chip8Builder::CoreType::OpenGL;
#elif defined(OPENGL_ENABLED)
    if (!args.video_mode.empty()) {
        if (args.video_mode == "opengl")
            core = Chip8Builder::CoreType::OpenGL;
        else if (args.video_mode == "dummy")
            core = Chip8Builder::CoreType::Dummy;
        else {
            std::cerr << "ERROR: unknown video mode '" << args.video_mode << "'" << std::endl;
            return -1;
        }
    } else
        core = Chip8Builder::CoreType::OpenGL;
#elif defined(CURSES_ENABLED)
    if (!args.video_mode.empty()) {
        if (args.video_mode == "curses")
            core = Chip8Builder::CoreType::Curses;
        else if (args.video_mode == "dummy")
            core = Chip8Builder::CoreType::Dummy;
        else {
            std::cerr << "ERROR: unknown video mode '" << args.video_mode << "'" << std::endl;
            return -1;
        }
    } else
        core = Chip8Builder::CoreType::Curses;
#else
    std::cerr << "WARNING: video disabled" << std::endl;
#endif


#ifdef SOLOUD_ENABLED
    if (!args.no_sound)
        audio = Chip8Builder::AudioType::SoLoud;
#endif

    builder.set_core(core);
    builder.set_audio(audio);

    if (args.instructions_per_seconds != DEFAULT_INT_OPTION)
        builder.set_instructions_per_second(args.instructions_per_seconds);

    if (args.scaling != DEFAULT_INT_OPTION) {
        builder.set_horizontal_scaling(args.scaling);
        builder.set_vertical_scaling(args.scaling);
    }

    Chip8 chip8 = builder.build();

//    std::cout << "Loading " << args.rom << std::endl;
    if (!chip8.load(args.rom)) {
        std::cerr << "ERROR: failed to load " << args.rom << std::endl;
        return -1;
    }

    do {
        reset_flag = false;
        chip8.run();
        if (reset_flag)
            chip8.reset();
    } while (reset_flag);

    return 0;
}
