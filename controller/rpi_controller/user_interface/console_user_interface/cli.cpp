/*
 *  Hydrotopia
 *  Copyright (C) 2022 Johan Norberg <lonezor@gmail.com>
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

#include <getopt.h>
#include <iostream>

#include <user_interface/console_user_interface/cli.hpp>

namespace hydroctrl {
namespace user_interface {

//---------------------------------------------------------------------------------------------------------------------

/** CLI long options */
enum cli_option
{
    cli_option_log_level = 1000,
    cli_option_help
};

//---------------------------------------------------------------------------------------------------------------------

static struct option long_options[] = {
    {"log-level", required_argument, nullptr, cli_option_log_level},
    {"help", no_argument, nullptr, cli_option_help},
    {nullptr, 0, nullptr, 0}};

//---------------------------------------------------------------------------------------------------------------------

bool parse_command_line_arguments(std::shared_ptr<common::configuration> cfg,
                                  int argc, char *argv[])
{
    // Parse command line arguments
    int c = 0;
    int option_index = 0;
    while (true) {
        c = getopt_long(argc, argv, "hl:", long_options, &option_index);

        // All options parsed
        if (c == -1) {
            break;
        }

        switch (c) {
        case 'h':
        case cli_option_help:
            cfg->help_screen = true;
            break;

        case 'l':
        case cli_option_log_level:
            cfg->log_level = static_cast<common::log_level>(
                static_cast<int>(strtol(optarg, nullptr, 10)));
            if (cfg->log_level < common::log_level_emerg ||
                cfg->log_level > common::log_level_debug) {
                std::cerr << "Error: Invalid log level -> "
                          << static_cast<int>(cfg->log_level) << " (range "
                          << static_cast<int>(common::log_level_emerg) << "-"
                          << static_cast<int>(common::log_level_debug) << ")"
                          << std::endl;
                return false;
            }
            break;

        default:
            break;
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace user_interface
} // namespace hydroctrl
