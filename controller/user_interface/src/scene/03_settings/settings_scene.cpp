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

#include <memory>
#include <iostream>
#include <sstream>
#include <string>

#include <common.hpp>
#include <object/background_object.hpp>
#include <scene/03_settings/settings_scene.hpp>

settings_scene::settings_scene(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache)
  : scene(ctx, sur_cache)
{
    auto obj = std::shared_ptr<object>(new background_object(ctx_, sur_cache_, 0, 0,
            ctx->screen_width(), ctx->screen_height()));
    objects_.emplace_back(obj);

    auto daily_channels_path = "/usr/share/hydrotopia_ui/images/daily_channels.png";
    auto daily_channels = std::shared_ptr<image_object>(new image_object(ctx_, sur_cache_,
                                                                  25,
                                                                  25,
                                                                  0,
                                                                  0,
                                                                  daily_channels_path,
                                                                  ""));
    objects_.emplace_back(std::shared_ptr<object>(daily_channels));

    auto hourly_channels_path = "/usr/share/hydrotopia_ui/images/hourly_channels.png";
    auto hourly_channels = std::shared_ptr<image_object>(new image_object(ctx_, sur_cache_,
                                                                  25,
                                                                  800,
                                                                  0,
                                                                  0,
                                                                  hourly_channels_path,
                                                                  ""));
    objects_.emplace_back(std::shared_ptr<object>(hourly_channels));

    auto grass_path = "/usr/share/hydrotopia_ui/images/grass.png";
    auto grass = std::shared_ptr<image_object>(new image_object(ctx_, sur_cache_,
                                                                  165,
                                                                  659,
                                                                  0,
                                                                  0,
                                                                  grass_path,
                                                                  ""));
    objects_.emplace_back(std::shared_ptr<object>(grass));

    auto garden_path = "/usr/share/hydrotopia_ui/images/garden.png";
    auto garden = std::shared_ptr<image_object>(new image_object(ctx_, sur_cache_,
                                                                  1505,
                                                                  490,
                                                                  0,
                                                                  0,
                                                                  garden_path,
                                                                  ""));
    objects_.emplace_back(std::shared_ptr<object>(garden));

 double highlight_off = 0.8;
 
 daily_ch_01_off_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1050, 50, 320, 160, navigation_state::setting_off));
daily_ch_01_off_->set_highlight_off(highlight_off);

 daily_ch_01_3h_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1350, 50, 320, 160, navigation_state::setting_3h));
daily_ch_01_3h_->set_highlight_off(highlight_off);

 daily_ch_01_6h_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1650, 50, 320, 160, navigation_state::setting_6h));
daily_ch_01_6h_->set_highlight_off(highlight_off);

 daily_ch_01_12h_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1950, 50, 320, 160, navigation_state::setting_12h));
daily_ch_01_12h_->set_highlight_off(highlight_off);

 daily_ch_01_18h_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 2250, 50, 320, 160, navigation_state::setting_18h));
daily_ch_01_18h_->set_highlight_off(highlight_off);

 daily_ch_02_off_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1050, 300, 320, 160, navigation_state::setting_off));
daily_ch_02_off_->set_highlight_off(highlight_off);

 daily_ch_02_3h_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1350, 300, 320, 160, navigation_state::setting_3h));
daily_ch_02_3h_->set_highlight_off(highlight_off);

 daily_ch_02_6h_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1650, 300, 320, 160, navigation_state::setting_6h));
daily_ch_02_6h_->set_highlight_off(highlight_off);

 daily_ch_02_12h_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1950, 300, 320, 160, navigation_state::setting_12h));
daily_ch_02_12h_->set_highlight_off(highlight_off);

 daily_ch_02_18h_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 2250, 300, 320, 160, navigation_state::setting_18h));
daily_ch_02_18h_->set_highlight_off(highlight_off);

 hourly_ch_01_off_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1050, 950, 320, 160, navigation_state::setting_off));
hourly_ch_01_off_->set_highlight_off(highlight_off);

 hourly_ch_01_5m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1350, 850, 320, 160, navigation_state::setting_5m));
hourly_ch_01_5m_->set_highlight_off(highlight_off);

 hourly_ch_01_15m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1650, 850, 320, 160, navigation_state::setting_15m));
hourly_ch_01_15m_->set_highlight_off(highlight_off);

 hourly_ch_01_30m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1950, 850, 320, 160, navigation_state::setting_30m));
hourly_ch_01_30m_->set_highlight_off(highlight_off);

 hourly_ch_01_45m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 2250, 850, 320, 160, navigation_state::setting_45m));
hourly_ch_01_45m_->set_highlight_off(highlight_off);

 hourly_ch_02_5m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1350, 1100, 320, 160, navigation_state::setting_5m));
hourly_ch_02_5m_->set_highlight_off(highlight_off);

 hourly_ch_02_15m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1650, 1100, 320, 160, navigation_state::setting_15m));
hourly_ch_02_15m_->set_highlight_off(highlight_off);

 hourly_ch_02_30m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1950, 1100, 320, 160, navigation_state::setting_30m));
hourly_ch_02_30m_->set_highlight_off(highlight_off);

 hourly_ch_02_45m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 2250, 1100, 320, 160, navigation_state::setting_45m));
hourly_ch_02_45m_->set_highlight_off(highlight_off);

hourly_ch_03_off_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1050, 1350, 320, 160, navigation_state::setting_off));
hourly_ch_03_off_->set_highlight_off(highlight_off);

 hourly_ch_03_5m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1350, 1350, 320, 160, navigation_state::setting_5m));
hourly_ch_03_5m_->set_highlight_off(highlight_off);

 hourly_ch_03_15m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1650, 1350, 320, 160, navigation_state::setting_15m));
hourly_ch_03_15m_->set_highlight_off(highlight_off);

 hourly_ch_03_30m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1950, 1350, 320, 160, navigation_state::setting_30m));
hourly_ch_03_30m_->set_highlight_off(highlight_off);

 hourly_ch_03_45m_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 2250, 1350, 320, 160, navigation_state::setting_45m));
hourly_ch_03_45m_->set_highlight_off(highlight_off);

 lock_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 1085, 568, 320, 160, navigation_state::setting_lock));
 lock_->set_highlight_off(highlight_off);
}

void settings_scene::draw()
{
    if (!conn_established_) {
        control_channel_.connect("192.168.0.250", 10);
        retrieve_channel_state();
        conn_established_ = true;
    }

    for(auto&& object : objects_) {
        object->draw();
    }

    daily_ch_01_off_->draw();
    daily_ch_01_3h_->draw();
    daily_ch_01_6h_->draw();
    daily_ch_01_12h_->draw();
    daily_ch_01_18h_->draw();

    daily_ch_02_off_->draw();
    daily_ch_02_3h_->draw();
    daily_ch_02_6h_->draw();
    daily_ch_02_12h_->draw();
    daily_ch_02_18h_->draw();

    hourly_ch_01_off_->draw();
    hourly_ch_01_5m_->draw();
    hourly_ch_01_15m_->draw();
    hourly_ch_01_30m_->draw();
    hourly_ch_01_45m_->draw();

    // ch_02_off does not exit
    hourly_ch_02_5m_->draw();
    hourly_ch_02_15m_->draw();
    hourly_ch_02_30m_->draw();
    hourly_ch_02_45m_->draw();

    hourly_ch_03_off_->draw();
    hourly_ch_03_5m_->draw();
    hourly_ch_03_15m_->draw();
    hourly_ch_03_30m_->draw();
    hourly_ch_03_45m_->draw();

    lock_->draw();

   if (started_ts_ == 0) {
        started_ts_ = get_ts();
    }

    bool reset = false;
    auto timeout = std::chrono::duration_cast<std::chrono::milliseconds>(active_screen_timeout);
    auto elapsed_time = (get_ts() - started_ts_) / 1000; // unit: ms

    if (started_ts_ != 0 && elapsed_time > timeout.count()) {
      control_channel_.disconnect();
      exit(0);
    }

    if (reset) {
      for(auto&& object : objects_) {
        object->invalidate();
      }
    }
}

void settings_scene::retrieve_channel_state()
{
    // The remote side response is tailored for this user interface

    auto response = control_channel_.query_channel_states();

    std::stringstream ss(response);
    std::string line;
    while (std::getline(ss, line, '\n')) {
        if (line.find("daily_ch_01_off: 1") != std::string::npos) {
            daily_ch_01_off_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("daily_ch_01_3h: 1") != std::string::npos) {
            daily_ch_01_3h_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("daily_ch_01_6h: 1") != std::string::npos) {
            daily_ch_01_6h_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("daily_ch_01_12h: 1") != std::string::npos) {
            daily_ch_01_12h_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("daily_ch_01_18h: 1") != std::string::npos) {
            daily_ch_01_18h_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("daily_ch_02_off: 1") != std::string::npos) {
            daily_ch_02_off_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("daily_ch_02_3h: 1") != std::string::npos) {
            daily_ch_02_3h_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("daily_ch_02_6h: 1") != std::string::npos) {
            daily_ch_02_6h_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("daily_ch_02_12h: 1") != std::string::npos) {
            daily_ch_02_12h_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("daily_ch_02_18h: 1") != std::string::npos) {
            daily_ch_02_18h_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_01_off: 1") != std::string::npos) {
            hourly_ch_01_off_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_01_5m: 1") != std::string::npos) {
            hourly_ch_01_5m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_01_15m: 1") != std::string::npos) {
            hourly_ch_01_15m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_01_30m: 1") != std::string::npos) {
            hourly_ch_01_30m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_01_45m: 1") != std::string::npos) {
            hourly_ch_01_45m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_02_5m: 1") != std::string::npos) {
            hourly_ch_02_5m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_02_15m: 1") != std::string::npos) {
            hourly_ch_02_15m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_02_30m: 1") != std::string::npos) {
            hourly_ch_02_30m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_02_45m: 1") != std::string::npos) {
            hourly_ch_02_45m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_03_off: 1") != std::string::npos) {
            hourly_ch_03_off_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_03_5m: 1") != std::string::npos) {
            hourly_ch_03_5m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_03_15m: 1") != std::string::npos) {
            hourly_ch_03_15m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_03_30m: 1") != std::string::npos) {
            hourly_ch_03_30m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
        if (line.find("hourly_ch_03_45m: 1") != std::string::npos) {
            hourly_ch_03_45m_->update_ev_state(ui_event(ui_event_type::button_press), true);
        }
    }
}

void settings_scene::begin()
{

}

void settings_scene::draw(ui_event ev)
{
    if (!conn_established_) {
        control_channel_.connect("127.0.0.1", 10);
        retrieve_channel_state();
        conn_established_ = true;
    }

    // User interactions resets screen timer
    started_ts_ = get_ts();

  for(auto&& object : objects_) {
      object->draw(ev);
  }

  // Be very restrictive of what is updated
  // since it is costly to update the screen
  // This also avoids flickering

  //*********** FIRST ROW *******************
  if (daily_ch_01_off_->update_ev_state(ev, false)) {
    if (daily_ch_01_3h_->is_selected()) {
        daily_ch_01_3h_->unselect();
    }
    if (daily_ch_01_6h_->is_selected()) {
        daily_ch_01_6h_->unselect();
    }
    if (daily_ch_01_12h_->is_selected()) {
        daily_ch_01_12h_->unselect();
    }
    if (daily_ch_01_18h_->is_selected()) {
        daily_ch_01_18h_->unselect();
    }

    if (daily_ch_01_off_->is_selected()) {
        control_channel_.send_cmd("upper_full_spectrum_light_power_profile_off");
    }
  }
  else if (daily_ch_01_3h_->update_ev_state(ev, false)) {
    if (daily_ch_01_off_->is_selected()) {
        daily_ch_01_off_->unselect();
    }
    if (daily_ch_01_6h_->is_selected()) {
        daily_ch_01_6h_->unselect();
    }
    if (daily_ch_01_12h_->is_selected()) {
        daily_ch_01_12h_->unselect();
    }
    if (daily_ch_01_18h_->is_selected()) {
        daily_ch_01_18h_->unselect();
    }
    if (daily_ch_01_3h_->is_selected()) {
        control_channel_.send_cmd("upper_full_spectrum_light_power_profile_daily_03h");
    }
  }
  else if (daily_ch_01_6h_->update_ev_state(ev, false)) {
    if (daily_ch_01_off_->is_selected()) {
        daily_ch_01_off_->unselect();
    }
    if (daily_ch_01_3h_->is_selected()) {
        daily_ch_01_3h_->unselect();
    }
    if (daily_ch_01_12h_->is_selected()) {
        daily_ch_01_12h_->unselect();
    }
    if (daily_ch_01_18h_->is_selected()) {
        daily_ch_01_18h_->unselect();
    }
    if (daily_ch_01_6h_->is_selected()) {
        control_channel_.send_cmd("upper_full_spectrum_light_power_profile_daily_06h");
    }
  }
  else if (daily_ch_01_12h_->update_ev_state(ev, false)) {
    if (daily_ch_01_off_->is_selected()) {
        daily_ch_01_off_->unselect();
    }
    if (daily_ch_01_3h_->is_selected()) {
        daily_ch_01_3h_->unselect();
    }
    if (daily_ch_01_6h_->is_selected()) {
        daily_ch_01_6h_->unselect();
    }
    if (daily_ch_01_18h_->is_selected()) {
        daily_ch_01_18h_->unselect();
    }
    if (daily_ch_01_12h_->is_selected()) {
        control_channel_.send_cmd("upper_full_spectrum_light_power_profile_daily_12h");
    }
  }
  else if (daily_ch_01_18h_->update_ev_state(ev, false)) {
    if (daily_ch_01_off_->is_selected()) {
        daily_ch_01_off_->unselect();
    }
    if (daily_ch_01_3h_->is_selected()) {
        daily_ch_01_3h_->unselect();
    }
    if (daily_ch_01_6h_->is_selected()) {
        daily_ch_01_6h_->unselect();
    }
    if (daily_ch_01_12h_->is_selected()) {
        daily_ch_01_12h_->unselect();
    }
    if (daily_ch_01_18h_->is_selected()) {
        control_channel_.send_cmd("upper_full_spectrum_light_power_profile_daily_18h");
    }
  }

    //*********** SECOND ROW *******************
  if (daily_ch_02_off_->update_ev_state(ev, false)) {
    if (daily_ch_02_3h_->is_selected()) {
        daily_ch_02_3h_->unselect();
    }
    if (daily_ch_02_6h_->is_selected()) {
        daily_ch_02_6h_->unselect();
    }
    if (daily_ch_02_12h_->is_selected()) {
        daily_ch_02_12h_->unselect();
    }
    if (daily_ch_02_18h_->is_selected()) {
        daily_ch_02_18h_->unselect();
    }
    if (daily_ch_02_off_->is_selected()) {
        control_channel_.send_cmd("lower_full_spectrum_light_power_profile_off");
    }
  }
   else if (daily_ch_02_3h_->update_ev_state(ev, false)) {
    if (daily_ch_02_off_->is_selected()) {
        daily_ch_02_off_->unselect();
    }
    if (daily_ch_02_6h_->is_selected()) {
        daily_ch_02_6h_->unselect();
    }
    if (daily_ch_02_12h_->is_selected()) {
        daily_ch_02_12h_->unselect();
    }
    if (daily_ch_02_18h_->is_selected()) {
        daily_ch_02_18h_->unselect();
    }
    if (daily_ch_02_3h_->is_selected()) {
        control_channel_.send_cmd("lower_full_spectrum_light_power_profile_daily_03h");
    }
  }
  else if (daily_ch_02_6h_->update_ev_state(ev, false)) {
    if (daily_ch_02_off_->is_selected()) {
        daily_ch_02_off_->unselect();
    }
    if (daily_ch_02_3h_->is_selected()) {
        daily_ch_02_3h_->unselect();
    }
    if (daily_ch_02_12h_->is_selected()) {
        daily_ch_02_12h_->unselect();
    }
    if (daily_ch_02_18h_->is_selected()) {
        daily_ch_02_18h_->unselect();
    }
    if (daily_ch_02_6h_->is_selected()) {
        control_channel_.send_cmd("lower_full_spectrum_light_power_profile_daily_06h");
    }
  }
  else if (daily_ch_02_12h_->update_ev_state(ev, false)) {
    if (daily_ch_02_off_->is_selected()) {
        daily_ch_02_off_->unselect();
    }
    if (daily_ch_02_3h_->is_selected()) {
        daily_ch_02_3h_->unselect();
    }
    if (daily_ch_02_6h_->is_selected()) {
        daily_ch_02_6h_->unselect();
    }
    if (daily_ch_02_18h_->is_selected()) {
        daily_ch_02_18h_->unselect();
    }
    if (daily_ch_02_12h_->is_selected()) {
        control_channel_.send_cmd("lower_full_spectrum_light_power_profile_daily_12h");
    }
  }
  else if (daily_ch_02_18h_->update_ev_state(ev, false)) {
    if (daily_ch_02_off_->is_selected()) {
        daily_ch_02_off_->unselect();
    }
    if (daily_ch_02_3h_->is_selected()) {
        daily_ch_02_3h_->unselect();
    }
    if (daily_ch_02_6h_->is_selected()) {
        daily_ch_02_6h_->unselect();
    }
    if (daily_ch_02_12h_->is_selected()) {
        daily_ch_02_12h_->unselect();
    }
    if (daily_ch_02_18h_->is_selected()) {
        control_channel_.send_cmd("lower_full_spectrum_light_power_profile_daily_18h");
    }
  }

  //*********** THIRD ROW *******************
  if (hourly_ch_01_off_->update_ev_state(ev, false)) {
    if (hourly_ch_01_5m_->is_selected()) {
        hourly_ch_01_5m_->unselect();
    }
    if (hourly_ch_01_15m_->is_selected()) {
        hourly_ch_01_15m_->unselect();
    }
    if (hourly_ch_01_30m_->is_selected()) {
        hourly_ch_01_30m_->unselect();
    }
    if (hourly_ch_01_45m_->is_selected()) {
        hourly_ch_01_45m_->unselect();
    }
    if (hourly_ch_02_5m_->is_selected()) {
        hourly_ch_02_5m_->unselect();
    }
    if (hourly_ch_02_15m_->is_selected()) {
        hourly_ch_02_15m_->unselect();
    }
    if (hourly_ch_02_30m_->is_selected()) {
        hourly_ch_02_30m_->unselect();
    }
    if (hourly_ch_02_45m_->is_selected()) {
        hourly_ch_02_45m_->unselect();
    }
    if (hourly_ch_01_off_->is_selected()) {
        control_channel_.send_cmd("ventilation_fan_power_profile_off");
    }
  }
  else if (hourly_ch_01_5m_->update_ev_state(ev, false)) {
    if (hourly_ch_01_off_->is_selected()) {
        hourly_ch_01_off_->unselect();
    }
    if (hourly_ch_01_15m_->is_selected()) {
        hourly_ch_01_15m_->unselect();
    }
    if (hourly_ch_01_30m_->is_selected()) {
        hourly_ch_01_30m_->unselect();
    }
    if (hourly_ch_01_45m_->is_selected()) {
        hourly_ch_01_45m_->unselect();
    }
    if (hourly_ch_02_5m_->is_selected()) {
        hourly_ch_02_5m_->unselect();
    }
    if (hourly_ch_02_15m_->is_selected()) {
        hourly_ch_02_15m_->unselect();
    }
    if (hourly_ch_02_30m_->is_selected()) {
        hourly_ch_02_30m_->unselect();
    }
    if (hourly_ch_02_45m_->is_selected()) {
        hourly_ch_02_45m_->unselect();
    }
    if (hourly_ch_01_5m_->is_selected()) {
        control_channel_.send_cmd("ventilation_fan_rpm_high");
        control_channel_.send_cmd("ventilation_fan_power_profile_hourly_05min");
    }
  }
  
  else if (hourly_ch_01_15m_->update_ev_state(ev, false)) {
    if (hourly_ch_01_off_->is_selected()) {
        hourly_ch_01_off_->unselect();
    }
    if (hourly_ch_01_5m_->is_selected()) {
        hourly_ch_01_5m_->unselect();
    }
    if (hourly_ch_01_30m_->is_selected()) {
        hourly_ch_01_30m_->unselect();
    }
    if (hourly_ch_01_45m_->is_selected()) {
        hourly_ch_01_45m_->unselect();
    }
    if (hourly_ch_02_5m_->is_selected()) {
        hourly_ch_02_5m_->unselect();
    }
    if (hourly_ch_02_15m_->is_selected()) {
        hourly_ch_02_15m_->unselect();
    }
    if (hourly_ch_02_30m_->is_selected()) {
        hourly_ch_02_30m_->unselect();
    }
    if (hourly_ch_02_45m_->is_selected()) {
        hourly_ch_02_45m_->unselect();
    }
    if (hourly_ch_01_15m_->is_selected()) {
        control_channel_.send_cmd("ventilation_fan_rpm_high");
        control_channel_.send_cmd("ventilation_fan_power_profile_hourly_15min");
    }
  }
  
  else if (hourly_ch_01_30m_->update_ev_state(ev, false)) {
    if (hourly_ch_01_off_->is_selected()) {
        hourly_ch_01_off_->unselect();
    }
    if (hourly_ch_01_5m_->is_selected()) {
        hourly_ch_01_5m_->unselect();
    }
    if (hourly_ch_01_15m_->is_selected()) {
        hourly_ch_01_15m_->unselect();
    }
    if (hourly_ch_01_45m_->is_selected()) {
        hourly_ch_01_45m_->unselect();
    }
    if (hourly_ch_02_5m_->is_selected()) {
        hourly_ch_02_5m_->unselect();
    }
    if (hourly_ch_02_15m_->is_selected()) {
        hourly_ch_02_15m_->unselect();
    }
    if (hourly_ch_02_30m_->is_selected()) {
        hourly_ch_02_30m_->unselect();
    }
    if (hourly_ch_02_45m_->is_selected()) {
        hourly_ch_02_45m_->unselect();
    }
    if (hourly_ch_01_30m_->is_selected()) {
        control_channel_.send_cmd("ventilation_fan_rpm_high");
        control_channel_.send_cmd("ventilation_fan_power_profile_hourly_30min");
    }
  }
  
  else if (hourly_ch_01_45m_->update_ev_state(ev, false)) {
    if (hourly_ch_01_off_->is_selected()) {
        hourly_ch_01_off_->unselect();
    }
    if (hourly_ch_01_5m_->is_selected()) {
        hourly_ch_01_5m_->unselect();
    }
    if (hourly_ch_01_15m_->is_selected()) {
        hourly_ch_01_15m_->unselect();
    }
    if (hourly_ch_01_30m_->is_selected()) {
        hourly_ch_01_30m_->unselect();
    }
    if (hourly_ch_02_5m_->is_selected()) {
        hourly_ch_02_5m_->unselect();
    }
    if (hourly_ch_02_15m_->is_selected()) {
        hourly_ch_02_15m_->unselect();
    }
    if (hourly_ch_02_30m_->is_selected()) {
        hourly_ch_02_30m_->unselect();
    }
    if (hourly_ch_02_45m_->is_selected()) {
        hourly_ch_02_45m_->unselect();
    }
    if (hourly_ch_01_45m_) {
        control_channel_.send_cmd("ventilation_fan_rpm_high");
        control_channel_.send_cmd("ventilation_fan_power_profile_hourly_45min");
    }
  }

  //*********** FORTH ROW *******************

  if (hourly_ch_02_5m_->update_ev_state(ev, false)) {
    if (hourly_ch_02_15m_->is_selected()) {
        hourly_ch_02_15m_->unselect();
    }
    if (hourly_ch_02_30m_->is_selected()) {
        hourly_ch_02_30m_->unselect();
    }
    if (hourly_ch_02_45m_->is_selected()) {
        hourly_ch_02_45m_->unselect();
    }
    if (hourly_ch_01_off_->is_selected()) {
        hourly_ch_01_off_->unselect();
    }
    if (hourly_ch_01_5m_->is_selected()) {
        hourly_ch_01_5m_->unselect();
    }
    if (hourly_ch_01_15m_->is_selected()) {
        hourly_ch_01_15m_->unselect();
    }
    if (hourly_ch_01_30m_->is_selected()) {
        hourly_ch_01_30m_->unselect();
    }
    if (hourly_ch_01_45m_->is_selected()) {
        hourly_ch_01_45m_->unselect();
    }

    if (hourly_ch_02_5m_->is_selected()) {
        control_channel_.send_cmd("ventilation_fan_rpm_low");
        control_channel_.send_cmd("ventilation_fan_power_profile_hourly_05min");
    }
  }
  
  else if (hourly_ch_02_15m_->update_ev_state(ev, false)) {
    if (hourly_ch_02_5m_->is_selected()) {
        hourly_ch_02_5m_->unselect();
    }
    if (hourly_ch_02_30m_->is_selected()) {
        hourly_ch_02_30m_->unselect();
    }
    if (hourly_ch_02_45m_->is_selected()) {
        hourly_ch_02_45m_->unselect();
    }
    if (hourly_ch_01_off_->is_selected()) {
        hourly_ch_01_off_->unselect();
    }
    if (hourly_ch_01_5m_->is_selected()) {
        hourly_ch_01_5m_->unselect();
    }
    if (hourly_ch_01_15m_->is_selected()) {
        hourly_ch_01_15m_->unselect();
    }
    if (hourly_ch_01_30m_->is_selected()) {
        hourly_ch_01_30m_->unselect();
    }
    if (hourly_ch_01_45m_->is_selected()) {
        hourly_ch_01_45m_->unselect();
    }

    if (hourly_ch_02_15m_->is_selected()) {
        control_channel_.send_cmd("ventilation_fan_rpm_low");
        control_channel_.send_cmd("ventilation_fan_power_profile_hourly_15min");
    }
  }
  
  else if (hourly_ch_02_30m_->update_ev_state(ev, false)) {
    if (hourly_ch_02_5m_->is_selected()) {
        hourly_ch_02_5m_->unselect();
    }
    if (hourly_ch_02_15m_->is_selected()) {
        hourly_ch_02_15m_->unselect();
    }
    if (hourly_ch_02_45m_->is_selected()) {
        hourly_ch_02_45m_->unselect();
    }
    if (hourly_ch_01_off_->is_selected()) {
        hourly_ch_01_off_->unselect();
    }
    if (hourly_ch_01_5m_->is_selected()) {
        hourly_ch_01_5m_->unselect();
    }
    if (hourly_ch_01_15m_->is_selected()) {
        hourly_ch_01_15m_->unselect();
    }
    if (hourly_ch_01_30m_->is_selected()) {
        hourly_ch_01_30m_->unselect();
    }
    if (hourly_ch_01_45m_->is_selected()) {
        hourly_ch_01_45m_->unselect();
    }
    if (hourly_ch_02_30m_->is_selected()) {
        control_channel_.send_cmd("ventilation_fan_rpm_low");
        control_channel_.send_cmd("ventilation_fan_power_profile_hourly_30min");
    }
  }
  
  else if (hourly_ch_02_45m_->update_ev_state(ev, false)) {
    if (hourly_ch_02_5m_->is_selected()) {
        hourly_ch_02_5m_->unselect();
    }
    if (hourly_ch_02_15m_->is_selected()) {
        hourly_ch_02_15m_->unselect();
    }
    if (hourly_ch_02_30m_->is_selected()) {
        hourly_ch_02_30m_->unselect();
    }
    if (hourly_ch_01_off_->is_selected()) {
        hourly_ch_01_off_->unselect();
    }
    if (hourly_ch_01_5m_->is_selected()) {
        hourly_ch_01_5m_->unselect();
    }
    if (hourly_ch_01_15m_->is_selected()) {
        hourly_ch_01_15m_->unselect();
    }
    if (hourly_ch_01_30m_->is_selected()) {
        hourly_ch_01_30m_->unselect();
    }
    if (hourly_ch_01_45m_->is_selected()) {
        hourly_ch_01_45m_->unselect();
    }
    
    if (hourly_ch_02_45m_->is_selected()) {
        control_channel_.send_cmd("ventilation_fan_rpm_low");
        control_channel_.send_cmd("ventilation_fan_power_profile_hourly_45min");
    }
  }

  //*********** FIFTH ROW *******************
  if (hourly_ch_03_off_->update_ev_state(ev, false)) {
    if (hourly_ch_03_5m_->is_selected()) {
        hourly_ch_03_5m_->unselect();
    }
    if (hourly_ch_03_15m_->is_selected()) {
        hourly_ch_03_15m_->unselect();
    }
    if (hourly_ch_03_30m_->is_selected()) {
        hourly_ch_03_30m_->unselect();
    }
    if (hourly_ch_03_45m_->is_selected()) {
        hourly_ch_03_45m_->unselect();
    }
    if (hourly_ch_03_off_->is_selected()) {
        control_channel_.send_cmd("wind_simulation_fan_power_profile_off");
    }
  }
  else if (hourly_ch_03_5m_->update_ev_state(ev, false)) {
    if (hourly_ch_03_off_->is_selected()) {
        hourly_ch_03_off_->unselect();
    }
    if (hourly_ch_03_15m_->is_selected()) {
        hourly_ch_03_15m_->unselect();
    }
    if (hourly_ch_03_30m_->is_selected()) {
        hourly_ch_03_30m_->unselect();
    }
    if (hourly_ch_03_45m_->is_selected()) {
        hourly_ch_03_45m_->unselect();
    }
    if (hourly_ch_03_5m_->is_selected()) {
        control_channel_.send_cmd("wind_simulation_fan_power_profile_hourly_05min");
    }
  }
  
  else if (hourly_ch_03_15m_->update_ev_state(ev, false)) {
    if (hourly_ch_03_off_->is_selected()) {
        hourly_ch_03_off_->unselect();
    }
    if (hourly_ch_03_5m_->is_selected()) {
        hourly_ch_03_5m_->unselect();
    }
    if (hourly_ch_03_30m_->is_selected()) {
        hourly_ch_03_30m_->unselect();
    }
    if (hourly_ch_03_45m_->is_selected()) {
        hourly_ch_03_45m_->unselect();
    }
    if (hourly_ch_03_15m_->is_selected()) {
        control_channel_.send_cmd("wind_simulation_fan_power_profile_hourly_15min");
    }
  }
  
  else if (hourly_ch_03_30m_->update_ev_state(ev, false)) {
    if (hourly_ch_03_off_->is_selected()) {
        hourly_ch_03_off_->unselect();
    }
    if (hourly_ch_03_5m_->is_selected()) {
        hourly_ch_03_5m_->unselect();
    }
    if (hourly_ch_03_15m_->is_selected()) {
        hourly_ch_03_15m_->unselect();
    }
    if (hourly_ch_03_45m_->is_selected()) {
        hourly_ch_03_45m_->unselect();
    }
    if (hourly_ch_03_30m_->is_selected()) {
        control_channel_.send_cmd("wind_simulation_fan_power_profile_hourly_30min");
    }
  }
  
  else if (hourly_ch_03_45m_->update_ev_state(ev, false)) {
    if (hourly_ch_03_off_->is_selected()) {
        hourly_ch_03_off_->unselect();
    }
    if (hourly_ch_03_5m_->is_selected()) {
        hourly_ch_03_5m_->unselect();
    }
    if (hourly_ch_03_15m_->is_selected()) {
        hourly_ch_03_15m_->unselect();
    }
    if (hourly_ch_03_30m_->is_selected()) {
        hourly_ch_03_30m_->unselect();
    }
    if (hourly_ch_03_45m_->is_selected()) {
        control_channel_.send_cmd("wind_simulation_fan_power_profile_hourly_45min");
    }
  }

  // Draw updated state
  daily_ch_01_off_->draw(ev);
  daily_ch_01_3h_->draw(ev);
  daily_ch_01_6h_->draw(ev);
  daily_ch_01_12h_->draw(ev);
  daily_ch_01_18h_->draw(ev);

  daily_ch_02_off_->draw(ev);
  daily_ch_02_3h_->draw(ev);
  daily_ch_02_6h_->draw(ev);
  daily_ch_02_12h_->draw(ev);
  daily_ch_02_18h_->draw(ev);

  hourly_ch_01_off_->draw(ev);
  hourly_ch_01_5m_->draw(ev);
  hourly_ch_01_15m_->draw(ev);
  hourly_ch_01_30m_->draw(ev);
  hourly_ch_01_45m_->draw(ev);

  hourly_ch_02_5m_->draw(ev);
  hourly_ch_02_15m_->draw(ev);
  hourly_ch_02_30m_->draw(ev);
  hourly_ch_02_45m_->draw(ev);

  hourly_ch_03_off_->draw(ev);
  hourly_ch_03_5m_->draw(ev);
  hourly_ch_03_15m_->draw(ev);
  hourly_ch_03_30m_->draw(ev);
  hourly_ch_03_45m_->draw(ev);

  if (lock_->update_ev_state(ev, false)) {
    if (lock_->is_selected()) {
        exit(0);
    }
  }
}
