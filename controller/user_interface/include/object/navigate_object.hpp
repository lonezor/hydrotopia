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

#pragma once

#include <object/object.hpp>
#include <graphics_context/rendering_context.hpp>

enum class navigation_state {
    // Page navigation
    previous_first,
    previous,
    next,
    next_last,
    keypad_00,
    keypad_01,
    keypad_02,
    keypad_03,
    keypad_04,
    keypad_05,
    keypad_06,
    keypad_07,
    keypad_08,
    keypad_09,
    keypad_star,
    keypad_hash,
    setting_off,
    setting_5m,
    setting_15m,
    setting_30m,
    setting_45m,
    setting_60m,
    setting_3h,
    setting_6h,
    setting_12h,
    setting_18h,
    setting_lock,

    // Scene navigation
    continue_blocked,
    continue_ready,
};

constexpr double default_highlight_on = 0.0;
constexpr double default_highlight_off = 0.2;

class navigate_object : public object
{
    public:
        navigate_object(std::shared_ptr<rendering_context> ctx,
                        std::shared_ptr<surface_cache> sur_cache,
                        double x,
                        double y,
                        double width,
                        double height,
                        navigation_state nav_state);

        bool update_ev_state(ui_event ev, bool force_selection);

        void draw() final;
        void draw(ui_event ev);

        void change_state(navigation_state nav_state);
        navigation_state state();

        bool is_selected();
        void unselect();
        void select();

        void set_highlight_off(double value);
        void set_highlight_on(double value);
    private:
        void internal_draw();

        void load_svg();

        bool hover_{false};
        bool selected_{false};
        navigation_state nav_state_;
        int64_t selected_ts_{0};

        double highlight_on_{default_highlight_on};
        double highlight_off_{default_highlight_off};
};

