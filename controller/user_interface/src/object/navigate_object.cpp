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

#include <object/navigate_object.hpp>
#include <common.hpp>
#include <iostream>

navigate_object::navigate_object(std::shared_ptr<rendering_context> ctx,
                std::shared_ptr<surface_cache> sur_cache,
                double x,
                double y,
                double width,
                double height,
                navigation_state nav_state)
 : object(ctx, sur_cache, x, y, width, height)
 , nav_state_(nav_state)
{
    load_svg();
    state_.alpha = highlight_off_;
}

void navigate_object::draw()
{
    if (!state_changed()) {
        return;
    }

    internal_draw();
}

bool navigate_object::update_ev_state(ui_event ev, bool force_selection)
{
    bool activation = false;

    if (force_selection || intersect(static_cast<double>(ev.get_x()),
                  static_cast<double>(ev.get_y()))) {
        state_.alpha = highlight_on_;
        hover_ = true;
        if (force_selection || ev.get_button_state() == button::left) {
            auto elapsed_time = (get_ts() - selected_ts_) / 1000; // unit: ms
            if (force_selection || (elapsed_time > 100 && nav_state_ != navigation_state::continue_blocked)) {
                selected_ = !selected_;
                activation = true;
                state_.invalidate = true;
                if (selected_) {
                    selected_ts_ = get_ts();
                }
            }
        }

    } else {
        hover_ = false;
        if (!selected_) {
            state_.alpha = highlight_off_;
        }
    }

    return activation;
}

void navigate_object::draw(ui_event ev)
{
    if (!state_changed()) {
        return;
    }

    internal_draw();
}

void navigate_object::change_state(navigation_state nav_state)
{
    nav_state_ = nav_state;
    load_svg();
}

navigation_state navigate_object::state()
{
    return nav_state_;
}

bool navigate_object::is_selected()
{
    return selected_;
}

void navigate_object::select()
{
    selected_ = true;
    state_.invalidate = true;
}

void navigate_object::unselect()
{
    selected_ = false;
    state_.invalidate = true;
}

void navigate_object::set_highlight_off(double value)
{
    highlight_off_ = value;
    state_.invalidate = true;
    state_.alpha = highlight_off_;
}

void navigate_object::set_highlight_on(double value)
{
    highlight_on_ = value;
    state_.invalidate = true;
    state_.alpha = highlight_on_;
}

void navigate_object::internal_draw()
{
    ctx_->set_source_rgb(1,1,1);
    ctx_->rectangle(state_.x,
                    state_.y,
                    state_.width,
                    state_.height);
    ctx_->fill();

    ctx_->draw_surface(surface_, state_.x, state_.y, 1);
    ctx_->set_source_rgba(1,1,1,state_.alpha);
    ctx_->rectangle(state_.x,
                    state_.y,
                    state_.width,
                    state_.height);
    ctx_->fill();

    state_.invalidate = false;
}

void navigate_object::load_svg()
{
    switch (nav_state_) {
        case navigation_state::keypad_00:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_00.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_01:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_01.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_02:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_02.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_03:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_03.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_04:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_04.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_05:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_05.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_06:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_06.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_07:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_07.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_08:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_08.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_09:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_09.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_star:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_star.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::keypad_hash:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Keypad_button_hash.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_off:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_off.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_5m:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_5m.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_15m:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_15m.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_30m:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_30m.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_45m:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_45m.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_60m:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_60m.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_3h:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_3h.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_6h:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_6h.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_12h:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_12h.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_18h:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_button_18h.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
        case navigation_state::setting_lock:
            surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/settings_lock.svg",
                                           ctx_->scale(state_.width),
                                           ctx_->scale(state_.height));
            break;
    }
}

