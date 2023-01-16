#include <iostream>
#include "chip8.h"
#include "config.h"
#include "glchip8factory.h"
#include "glsoloudchip8factory.h"
#include <argp.h>

#define DEFAULT_INSTRUCTIONS_PER_SECONDS 700
#define DEFAULT_SCALING 15

#define STRINGIFY2(X) #X
#define STRINGIFY(X) STRINGIFY2(X)

static const char *argp_args_doc = "ROM";
static const char *argp_doc = "chip8";

enum chip8_option {
    OPTION_INSTRUCTIONS_PER_SECONDS = 'f',
    OPTION_SCALING = 's'
};

static struct argp_option argp_options[] = {
  { "frequency", OPTION_INSTRUCTIONS_PER_SECONDS, "FREQ", 0,
        "CPU instructions per seconds (default is " STRINGIFY(DEFAULT_INSTRUCTIONS_PER_SECONDS) ")"},
  { "scaling", OPTION_SCALING, "SCALING", 0,
        "Scaling factor (default is " STRINGIFY(DEFAULT_SCALING) ")" },
  { nullptr }
};

struct chip8_arguments {
    explicit chip8_arguments() :
        instructions_per_seconds(DEFAULT_INSTRUCTIONS_PER_SECONDS),
        scaling(DEFAULT_SCALING),
        rom(nullptr) {}

    int instructions_per_seconds;
    int scaling;
    const char *rom;
};

static error_t parse_option(int key, char *arg, struct argp_state *state) {
    auto args = (chip8_arguments *) state->input;

    switch (key) {
    case OPTION_INSTRUCTIONS_PER_SECONDS:
        args->instructions_per_seconds = std::stoi(arg);
        break;
    case OPTION_SCALING:
        args->scaling = std::stoi(arg);
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
    chip8_arguments arguments;
    argp_parse(&argp, argc, argv, 0, nullptr, &arguments);

#ifndef SOUND_ENABLED
    std::cout << "WARNING: sound disabled (only ALSA is supported)" << std::endl;
#endif

    Config cfg {
        .instructions_per_seconds = arguments.instructions_per_seconds,
        .scaling = arguments.scaling,
    };

    GlWindow glwin(CHIP8_DISPLAY_WIDTH * cfg.scaling, CHIP8_DISPLAY_HEIGHT * cfg.scaling, "Chip8");
    glwin.init();

    GlSoLoudChip8Factory factory(glwin.window);

    Chip8 chip8(factory, cfg);
    std::cout << "Loading \"" << arguments.rom << "\"" << std::endl;
    if (!chip8.load(arguments.rom)) {
        std::cerr << "ERROR: failed to load \"" << arguments.rom << "\"" << std::endl;
        exit(EXIT_FAILURE);
    }
    chip8.run();

    return 0;
}
