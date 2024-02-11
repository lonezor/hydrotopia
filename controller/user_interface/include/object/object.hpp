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

#include <string>
#include <memory>

#include <user_interface/ui_event.hpp>
#include <graphics_context/rendering_context.hpp>
#include <graphics_context/surface.hpp>
#include <graphics_context/surface_cache.hpp>

struct state
{
    double angle;
    double x;
    double y;
    double width;
    double height;
    double alpha;
    bool invalidate;
};

class object
{
    public:
        object(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache, double x, double y, double width, double height);

        virtual void draw() = 0;

        virtual void draw(ui_event ev) = 0;

        bool intersect(object& obj);

        bool intersect(double mouse_x, double mouse_y);

        double x() { return state_.x; }

        double y() { return state_.y; }

        double angle() { return state_.angle; }

        double width() { return state_.width; }

        double height() { return state_.height; }

        void set_x(double x) { state_.x = x; }

        void set_y(double y) { state_.y = y; }

        void set_visibility(bool visible) { visible_ = visible; }

        void set_angle(double angle) { state_.angle = angle; }

        bool state_changed();

        void invalidate() { state_.invalidate = true; }

        void draw_object_bg();

        void draw_object_border();

    protected:
        bool visible_;

        state state_;

        state prev_state_;

        std::shared_ptr<rendering_context> ctx_;

        std::shared_ptr<surface_cache> sur_cache_;

        std::shared_ptr<surface> surface_;
};

