/*
 *  Hydrotopia UI
 * 
 *  Copyright (C) 2024 Johan Norberg <lonezor@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>
 */

#include <iostream>
#include <getopt.h>
#include <sstream>

#include <hydrotopia_ui.hpp>

//-------------------------------------------------------------------------------------------------------------------

constexpr int default_screen_width = 2560;
constexpr int default_screen_height = 1600;

//-------------------------------------------------------------------------------------------------------------------

static bool g_fullscreen = false;
static bool g_help = false;
static int g_screen_width = default_screen_width;
static int g_screen_height = default_screen_height;

//-------------------------------------------------------------------------------------------------------------------

enum cli_option
{
    cli_option_fullscreen = 1000, // value higher thann short options
    cli_option_screen_width,
    cli_option_screen_height,
    cli_option_help,
};

//-------------------------------------------------------------------------------------------------------------------

static struct option long_options[] = {
    { "fullscreen",     no_argument,       nullptr,  cli_option_fullscreen    },
    { "screen-width",   required_argument, nullptr,  cli_option_screen_width  },
    { "screen-height",  required_argument, nullptr,  cli_option_screen_height },
    { "help",           no_argument,       nullptr,  cli_option_help          },
    { nullptr,          0,                 nullptr,  0                        }
};

//-------------------------------------------------------------------------------------------------------------------

static bool
parse_arguments(int argc, char* argv[])
{
    // Parse command line arguments
    int c;
    int option_index = 0;
    while (true) {
        c = getopt_long(argc, argv, "fh", long_options, &option_index);

        // All options parsed
        if (c == -1) {
            break;
        }

        switch (c) // Listed in order of importance
        {
            case 'f':
            case cli_option_fullscreen:
                g_fullscreen = true;
                break;

            case cli_option_screen_width:
                g_screen_width = (int)strtol(optarg, nullptr, 10);
                break;

            case cli_option_screen_height:
                g_screen_height = (int)strtol(optarg, nullptr, 10);
                break;

            case 'h':
            case cli_option_help:
                g_help = true;
                break;

            default:
                continue;
        }
    }

    return true;
}

static void
print_help()
{
    std::stringstream ss;

    // clang-format off
    ss << "usage: hydrotopia_ui [OPTION]" << std::endl;
    ss << std::endl;
    ss << "Options:" << std::endl;
    ss << " -f --fullscreen-games   Fullscreen mode" << std::endl;
    ss << "    --screen-width=INT   Screen width (default " << default_screen_width << ")" << std::endl;
    ss << "    --screen-height=INT  Screen height (default " << default_screen_height << ")" << std::endl;
    ss << " -h --help               Show this help screen" << std::endl;
    // clang-format on

    std::cout << ss.str() << std::flush;
}

//-------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Parse CLI arguments
    if (!parse_arguments(argc, argv)) {
        return EXIT_FAILURE;
    }

    // Help screen
    if (g_help) {
        print_help();
        return EXIT_SUCCESS;
    }

    auto game = hydrotopia_ui(g_screen_width,
                          g_screen_height,
                          g_fullscreen);

    game.run();

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------
