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

#include <cstdio>
#include <iostream>
#include <memory>

#include <controller/controller.hpp>
#include <user_interface/console_user_interface/cli.hpp>

//---------------------------------------------------------------------------------------------------------------------

static void print_help()
{
    std::cout << "Usage: hydroctrl [OPTIONS]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options (in order of importance):" << std::endl;
    std::cout << " -l --log-level=INTEGER            Log level (levels 0-7). "
                 "Printed to stderr. Default: "
              << static_cast<int>(hydroctrl::common::log_level_default)
              << std::endl;
    std::cout << " -h --help                         This help screen"
              << std::endl;
    std::cout << std::endl;
}

//---------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    // Parse mandatory and optional arguments
    auto cfg = std::make_shared<hydroctrl::common::configuration>();
    if (!hydroctrl::user_interface::parse_command_line_arguments(cfg, argc,
                                                                 argv)) {
        return EXIT_FAILURE;
    }

    // Print Help Screen and exit
    if (cfg->help_screen) {
        print_help();
        return EXIT_SUCCESS;
    }

    hydroctrl::common::set_log_level(cfg->log_level);

    auto ctrl = hydroctrl::controller::controller(cfg);
    ctrl.run();

    return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
