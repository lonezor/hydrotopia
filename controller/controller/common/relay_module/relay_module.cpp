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

#include <cstddef>
#include <exception>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <common/log.hpp>
#include <common/relay_module/relay_module.hpp>

namespace hydroctrl {
namespace common {

//---------------------------------------------------------------------------------------------------------------------

relay_module::relay_module()
{
    activation_state_.resize(size_);
    activation_histogram_.resize(size_);
    activation_timepoint_refs_.resize(size_);
    duration_histogram_.resize(size_);

#ifdef HC_RELAY_MODULE_ENABLED
    rc_relay_channel_init();
#endif // HC_RELAY_MODULE_ENABLED

    clear();

    // After clear, mark all relays as deactivated
    for (int i = 0; i < size_; i++) {
        activation_state_[i] = false;
    }
}

//---------------------------------------------------------------------------------------------------------------------

int relay_module::size() { return size_; }

//---------------------------------------------------------------------------------------------------------------------

#ifdef HC_RELAY_MODULE_32_CHANNELS
rc_relay_port_t relay_module::index_to_relay_port(int index)
{
    if (index < 0 || index > 31) {
        throw std::runtime_error("[relay_module::index_to_relay_port] failed "
                                 "to map index to relay port");
    }

    if (index < 16) {
        return rc_relay_port_a;
    }

    return rc_relay_port_b;
}
#endif //HC_RELAY_MODULE_32_CHANNELS

//---------------------------------------------------------------------------------------------------------------------

rc_relay_channel_t relay_module::index_to_channel_type(int index)
{
    rc_relay_channel_t ch;

#ifdef HC_RELAY_MODULE_16_CHANNELS
    switch (index) {
    case 0:
        ch = rc_relay_channel_01;
        break;
    case 1:
        ch = rc_relay_channel_02;
        break;
    case 2:
        ch = rc_relay_channel_03;
        break;
    case 3:
        ch = rc_relay_channel_04;
        break;
    case 4:
        ch = rc_relay_channel_05;
        break;
    case 5:
        ch = rc_relay_channel_06;
        break;
    case 6:
        ch = rc_relay_channel_07;
        break;
    case 7:
        ch = rc_relay_channel_08;
        break;
    case 8:
        ch = rc_relay_channel_09;
        break;
    case 9:
        ch = rc_relay_channel_10;
        break;
    case 10:
        ch = rc_relay_channel_11;
        break;
    case 11:
        ch = rc_relay_channel_12;
        break;
    case 12:
        ch = rc_relay_channel_13;
        break;
    case 13:
        ch = rc_relay_channel_14;
        break;
    case 14:
        ch = rc_relay_channel_15;
        break;
   case 15:
        ch = rc_relay_channel_16;
        break;
    default:
        throw std::runtime_error("[relay_module::index_to_channel_type] failed "
                                 "to map index to channel type");
    }
#endif // HC_RELAY_MODULE_16_CHANNELS

#ifdef HC_RELAY_MODULE_32_CHANNELS
    switch (index) {
    case 0:
    case 16:
        ch = rc_relay_channel_01;
        break;
    case 1:
    case 17:
        ch = rc_relay_channel_02;
        break;
    case 2:
    case 18:
        ch = rc_relay_channel_03;
        break;
    case 3:
    case 19:
        ch = rc_relay_channel_04;
        break;
    case 4:
    case 20:
        ch = rc_relay_channel_05;
        break;
    case 5:
    case 21:
        ch = rc_relay_channel_06;
        break;
    case 6:
    case 22:
        ch = rc_relay_channel_07;
        break;
    case 7:
    case 23:
        ch = rc_relay_channel_08;
        break;
    case 8:
    case 24:
        ch = rc_relay_channel_09;
        break;
    case 9:
    case 25:
        ch = rc_relay_channel_10;
        break;
    case 10:
    case 26:
        ch = rc_relay_channel_11;
        break;
    case 11:
    case 27:
        ch = rc_relay_channel_12;
        break;
    case 12:
    case 28:
        ch = rc_relay_channel_13;
        break;
    case 13:
    case 29:
        ch = rc_relay_channel_14;
        break;
    case 14:
    case 30:
        ch = rc_relay_channel_15;
        break;
    case 15:
    case 31:
        ch = rc_relay_channel_16;
        break;
    default:
        throw std::runtime_error("[relay_module::index_to_channel_type] failed "
                                 "to map index to channel type");
    }
#endif // HC_RELAY_MODULE_32_CHANNELS

    return ch;
}

//---------------------------------------------------------------------------------------------------------------------

void relay_module::activate(int index)
{
    if (index < 0 || index > size_) {
        throw std::runtime_error("[relay_module::activate] invalid index");
    }

    // Trying to activate while already in active state: nothing to do
    if (activation_state_[index] == true) {
        return;
    }

#ifdef HC_RELAY_MODULE_ENABLED

    common::log(common::log_level::log_level_debug, "[relay_module::activate]");

#ifdef HC_RELAY_MODULE_16_CHANNELS
    auto ch = relay_module::index_to_channel_type(index);
    rc_relay_channel_set(ch, true);
#endif // HC_RELAY_MODULE_32_CHANNELS

#ifdef HC_RELAY_MODULE_32_CHANNELS
    auto port = index_to_relay_port(index);
    auto ch = relay_module::index_to_channel_type(index);
    rc_relay_channel_set(port, ch, true);
#endif // HC_RELAY_MODULE_32_CHANNELS

#else  // HC_RELAY_MODULE_ENABLED
    std::stringstream msg;
    msg << "[relay_module::activate] idx " << index << " (stubbed)";
    common::log(common::log_level::log_level_notice, msg.str());
#endif // HC_RELAY_MODULE_ENABLED

    activation_state_[index] = true;
    activation_histogram_[index]++;
    activation_timepoint_refs_[index] = std::chrono::steady_clock::now();
}

//---------------------------------------------------------------------------------------------------------------------

void relay_module::deactivate(int index)
{
    if (index < 0 || index > size_) {
        throw std::runtime_error("[relay_module::activate] invalid index");
    }

    // Trying to deactivate while already in inactive state: nothing to do
    if (activation_state_[index] == false) {
        return;
    }

#ifdef HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_debug,
                "[relay_module::deactivate]");

#ifdef HC_RELAY_MODULE_16_CHANNELS
    auto ch = relay_module::index_to_channel_type(index);
    rc_relay_channel_set(ch, false);
#endif // HC_RELAY_MODULE_16_CHANNELS

#ifdef HC_RELAY_MODULE_32_CHANNELS
    auto port = index_to_relay_port(index);
    auto ch = relay_module::index_to_channel_type(index);
    rc_relay_channel_set(port, ch, false);
#endif // HC_RELAY_MODULE_32_CHANNELS

#else  // HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_notice,
                "[relay_module::deactivate] stubbed");
#endif // HC_RELAY_MODULE_ENABLED

    activation_state_[index] = false;

    auto tp_now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        tp_now - activation_timepoint_refs_[index]);
    duration_histogram_[index] += elapsed;
}

//---------------------------------------------------------------------------------------------------------------------

void relay_module::clear()
{
#ifdef HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_debug, "[relay_module::clear]");

#ifdef HC_RELAY_MODULE_16_CHANNELS
    rc_relay_channel_set(rc_relay_channel_01, false);
    rc_relay_channel_set(rc_relay_channel_02, false);
    rc_relay_channel_set(rc_relay_channel_03, false);
    rc_relay_channel_set(rc_relay_channel_04, false);
    rc_relay_channel_set(rc_relay_channel_05, false);
    rc_relay_channel_set(rc_relay_channel_06, false);
    rc_relay_channel_set(rc_relay_channel_07, false);
    rc_relay_channel_set(rc_relay_channel_08, false);
    rc_relay_channel_set(rc_relay_channel_09, false);
    rc_relay_channel_set(rc_relay_channel_10, false);
    rc_relay_channel_set(rc_relay_channel_11, false);
    rc_relay_channel_set(rc_relay_channel_12, false);
    rc_relay_channel_set(rc_relay_channel_13, false);
    rc_relay_channel_set(rc_relay_channel_14, false);
    rc_relay_channel_set(rc_relay_channel_15, false);
    rc_relay_channel_set(rc_relay_channel_16, false);
#endif // HC_RELAY_MODULE_16_CHANNELS

#ifdef HC_RELAY_MODULE_32_CHANNELS
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_01, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_02, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_03, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_04, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_05, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_06, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_07, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_08, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_09, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_10, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_11, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_12, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_13, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_14, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_15, false);
    rc_relay_channel_set(rc_relay_port_a, rc_relay_channel_16, false);

    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_01, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_02, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_03, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_04, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_05, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_06, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_07, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_08, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_09, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_10, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_11, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_12, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_13, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_14, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_15, false);
    rc_relay_channel_set(rc_relay_port_b, rc_relay_channel_16, false);
#endif // HC_RELAY_MODULE_32_CHANNELS

#else  // HC_RELAY_MODULE_ENABLED
    common::log(common::log_level::log_level_notice,
                "[relay_module::clear] stubbed");
#endif // HC_RELAY_MODULE_ENABLED
}

//---------------------------------------------------------------------------------------------------------------------

std::string relay_module::stats()
{
    std::stringstream stats;

    stats << "~~~~~~~~~~~~~" << std::endl
          << "Relay Module:" << std::endl
          << "~~~~~~~~~~~~~" << std::endl;
    for (int i = 0; i < size_; i++) {
        auto duration = duration_histogram_[i];

        auto days = std::chrono::duration_cast<std::chrono::days>(duration);
        duration -= days;

        auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
        duration -= hours;

        auto minutes =
            std::chrono::duration_cast<std::chrono::minutes>(duration);
        duration -= minutes;

        auto seconds =
            std::chrono::duration_cast<std::chrono::seconds>(duration);
        duration -= seconds;

        auto milliseconds = duration;

        stats << "Relay " << std::setfill('0') << std::setw(2) << i << ": "
              << std::setfill('0') << std::setw(8) << activation_histogram_[i]
              << " activations, total duration " << std::setfill('0')
              << std::setw(5) << days.count() << " days, " << std::setfill('0')
              << std::setw(2) << hours.count() << " hours, "
              << std::setfill('0') << std::setw(2) << minutes.count()
              << " minutes, " << std::setfill('0') << std::setw(2)
              << seconds.count() << " seconds and " << std::setfill('0')
              << std::setw(3) << milliseconds.count() << " milliseconds"
              << std::endl;
    }

    stats << std::endl;

    return stats.str();
}

//---------------------------------------------------------------------------------------------------------------------

} // namespace common
} // namespace hydroctrl
