#include <iostream>
#include <argp.h>
#include <cstring>
#include "chip8builder.h"
#include "flags.h"

constexpr int DEFAULT_INT_OPTION = -1;

static const char *argp_args_doc = "ROM";
static const char *argp_doc = "chip8";

enum chip8_option {
    OPTION_VIDEO = 'v',
    OPTION_FREQUENCY = 'f',
    OPTION_SCALING = 's',
    OPTION_NO_SOUND = 'm'
};

#if defined(OPENGL_ENABLED) and defined(CURSES_ENABLED)
#define VIDEO_MODES "'opengl', 'curses', 'dummy'"
#elif defined(OPENGL_ENABLED)
#define VIDEO_MODES "'opengl', 'dummy'"
#elif defined(CURSES_ENABLED)
#define VIDEO_MODES "'curses', 'dummy'"
#else
#define VIDEO_MODES "'dummy'"
#endif

static struct argp_option argp_options[] = {
  { "video", OPTION_VIDEO, "VIDEO", 0, "Set video mode (" VIDEO_MODES ")" },
  { "frequency", OPTION_FREQUENCY, "FREQ", 0, "CPU instructions per seconds"},
  { "scaling", OPTION_SCALING, "SCALING", 0, "Video scaling factor" },
  { "no-sound", OPTION_NO_SOUND, nullptr, 0, "Disable sound" },
  { nullptr }
};

struct chip8_arguments {
    explicit chip8_arguments() :
        rom(nullptr),
        instructions_per_seconds(DEFAULT_INT_OPTION),
        scaling(DEFAULT_INT_OPTION),
        video_mode(nullptr),
        no_sound() {

    }

    const char *rom;
    int instructions_per_seconds;
    int scaling;
    const char *video_mode;
    bool no_sound;
};

static error_t parse_option(int key, char *arg, struct argp_state *state) {
    auto args = (chip8_arguments *) state->input;

    switch (key) {
    case OPTION_VIDEO:
        args->video_mode = arg;
        break;
    case OPTION_FREQUENCY:
        args->instructions_per_seconds = std::stoi(arg);
        break;
    case OPTION_SCALING:
        args->scaling = std::stoi(arg);
        break;
    case OPTION_NO_SOUND:
        args->no_sound = true;
        break;
    case ARGP_KEY_ARG:
        switch (state->arg_num) {
        case 0:
            args->rom = arg;
            break;
        default:
            argp_usage(state);
        }
        break;
   case ARGP_KEY_END:
        if (state->arg_num < 1)
            argp_usage(state);
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    static struct argp argp = { argp_options, parse_option, argp_args_doc, argp_doc };
    chip8_arguments args;
    argp_parse(&argp, argc, argv, 0, nullptr, &args);

#ifndef SOLOUD_ENABLED
    std::cerr << "WARNING: sound disabled" << std::endl;
#endif

    Chip8Builder builder;
    Chip8Builder::CoreType core = Chip8Builder::CoreType::Dummy;
    Chip8Builder::AudioType audio = Chip8Builder::AudioType::Dummy;

#if defined(OPENGL_ENABLED) && defined(CURSES_ENABLED)
    if (args.video_mode) {
        if (strcmp(args.video_mode, "opengl") == 0)
            core = Chip8Builder::CoreType::OpenGL;
        else if (strcmp(args.video_mode, "curses") == 0)
            core = Chip8Builder::CoreType::Curses;
        else if (strcmp(args.video_mode, "dummy") == 0)
            core = Chip8Builder::CoreType::Dummy;
        else {
            std::cerr << "ERROR: unknown video mode '" << args.video_mode << "'" << std::endl;
            return -1;
        }
    } else
        core = Chip8Builder::CoreType::OpenGL;
#elif defined(OPENGL_ENABLED)
    if (args.video_mode) {
        if (strcmp(args.video_mode, "opengl") == 0)
            core = Chip8Builder::CoreType::OpenGL;
        else if (strcmp(args.video_mode, "dummy") == 0)
            core = Chip8Builder::CoreType::Dummy;
        else {
            std::cerr << "ERROR: unknown video mode '" << args.video_mode << "'" << std::endl;
            return -1;
        }
    } else
        core = Chip8Builder::CoreType::OpenGL;
#elif defined(CURSES_ENABLED)
    if (args.video_mode) {
        if (strcmp(args.video_mode, "curses") == 0)
            core = Chip8Builder::CoreType::Curses;
        else if (strcmp(args.video_mode, "dummy") == 0)
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
