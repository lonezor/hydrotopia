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

#include <vector>
#include <memory>
#include <vector>

#include <graphics_context/rendering_context.hpp>
#include <graphics_context/surface_cache.hpp>
#include <object/image_object.hpp>
#include <object/navigate_object.hpp>
#include <object/object.hpp>
#include <object/text_object.hpp>
#include <scene/scene.hpp>
#include <scene/03_settings/control_channel.hpp>

class settings_scene : public scene
{
    public:
        settings_scene(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache);

        void draw() final;
        void draw(ui_event ev) final;
        void begin() final;

    private:
        void retrieve_channel_state();

        int64_t started_ts_{0};

        std::shared_ptr<navigate_object> daily_ch_01_off_;
        std::shared_ptr<navigate_object> daily_ch_01_3h_;
        std::shared_ptr<navigate_object> daily_ch_01_6h_;
        std::shared_ptr<navigate_object> daily_ch_01_12h_;
        std::shared_ptr<navigate_object> daily_ch_01_18h_;

        std::shared_ptr<navigate_object> daily_ch_02_off_;
        std::shared_ptr<navigate_object> daily_ch_02_3h_;
        std::shared_ptr<navigate_object> daily_ch_02_6h_;
        std::shared_ptr<navigate_object> daily_ch_02_12h_;
        std::shared_ptr<navigate_object> daily_ch_02_18h_;

        std::shared_ptr<navigate_object> hourly_ch_01_off_;
        std::shared_ptr<navigate_object> hourly_ch_01_5m_;
        std::shared_ptr<navigate_object> hourly_ch_01_15m_;
        std::shared_ptr<navigate_object> hourly_ch_01_30m_;
        std::shared_ptr<navigate_object> hourly_ch_01_45m_;
        std::shared_ptr<navigate_object> hourly_ch_01_60m_;

        std::shared_ptr<navigate_object> hourly_ch_02_off_;
        std::shared_ptr<navigate_object> hourly_ch_02_5m_;
        std::shared_ptr<navigate_object> hourly_ch_02_15m_;
        std::shared_ptr<navigate_object> hourly_ch_02_30m_;
        std::shared_ptr<navigate_object> hourly_ch_02_45m_;
        std::shared_ptr<navigate_object> hourly_ch_02_60m_;

        std::shared_ptr<navigate_object> hourly_ch_03_off_;
        std::shared_ptr<navigate_object> hourly_ch_03_5m_;
        std::shared_ptr<navigate_object> hourly_ch_03_15m_;
        std::shared_ptr<navigate_object> hourly_ch_03_30m_;
        std::shared_ptr<navigate_object> hourly_ch_03_45m_;
        std::shared_ptr<navigate_object> hourly_ch_03_60m_;

        std::shared_ptr<navigate_object> lock_;

        control_channel control_channel_;

        bool conn_established_{false};

};

