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

class keypad_scene : public scene
{
    public:
        keypad_scene(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache);

        void draw() final;
        void draw(ui_event ev) final;
        void begin() final;

    private:
        int64_t started_ts_{0};

        std::shared_ptr<image_object> background_;
        std::shared_ptr<text_object> header_text_;
        std::shared_ptr<text_object> status_text_;

        std::string code_;

        std::shared_ptr<navigate_object> keypad_object_00_;
        std::shared_ptr<navigate_object> keypad_object_01_;
        std::shared_ptr<navigate_object> keypad_object_02_;
        std::shared_ptr<navigate_object> keypad_object_03_;
        std::shared_ptr<navigate_object> keypad_object_04_;
        std::shared_ptr<navigate_object> keypad_object_05_;
        std::shared_ptr<navigate_object> keypad_object_06_;
        std::shared_ptr<navigate_object> keypad_object_07_;
        std::shared_ptr<navigate_object> keypad_object_08_;
        std::shared_ptr<navigate_object> keypad_object_09_;
        std::shared_ptr<navigate_object> keypad_object_star_;
        std::shared_ptr<navigate_object> keypad_object_hash_;
};

