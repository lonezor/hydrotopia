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

/** @file log.hpp
 *  @brief Logging framework with syslog compatibility
 */

#pragma once

#include <string>
#include <syslog.h>

namespace hydroctrl {
namespace common {

//-------------------------------------------------------------------------------------------------------------------

/** @brief Log level
 *
 * Intentionally align with the syslog levels.
 */
enum log_level
{
    log_level_emerg = LOG_EMERG, /**< A panic condition */
    log_level_alert =
        LOG_ALERT, /**< A condition that should be corrected immediately */
    log_level_crit = LOG_CRIT,       /**< A critical condition */
    log_level_err = LOG_ERR,         /**< An error message */
    log_level_warning = LOG_WARNING, /**< A warning message */
    log_level_notice =
        LOG_NOTICE,              /**< A condition requiring special handling */
    log_level_info = LOG_INFO,   /**< A general information message */
    log_level_debug = LOG_DEBUG, /**< A message useful for debugging programs */
};

//-------------------------------------------------------------------------------------------------------------------

/** @brief Default log level
 */
constexpr log_level log_level_default = log_level_crit;

//-------------------------------------------------------------------------------------------------------------------

/** @brief Set log level
 *
 * If this method isn't called log_level_default will be used.
 *
 * @param level  Log level
 */
void set_log_level(log_level level);

//-------------------------------------------------------------------------------------------------------------------

/** @brief Get current log level
 *
 * @return Log level
 */
log_level get_log_level();

//-------------------------------------------------------------------------------------------------------------------

/** @brief Print log message
 *
 * Based on the current log level the method will determine if the print to
 * stderr should be performed or not.
 *
 * @param level  Log level for message
 * @param msg    Message
 */
void log(log_level level, const std::string &msg);

//-------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl