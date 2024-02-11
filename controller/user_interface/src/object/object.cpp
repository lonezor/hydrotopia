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

#include <iostream>

#include <object/object.hpp>

 object::object(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache, double x, double y, double width, double height)
  : ctx_(ctx)
  , sur_cache_(sur_cache)
{
    state_.x = x;
    state_.y = y;
    state_.width = width;
    state_.height = height;
    state_.alpha = 1.0;
    state_.invalidate = true;
}

bool object::intersect(object& obj)
{
    return false;
    //return ((x_ >= obj.x() && x_ <= obj.x() + width_) &&
      //      (y_ >= obj.y() && y_ <= obj.y() + height_));
}

bool object::intersect(double mouse_x, double mouse_y)
{
    return ((mouse_x >= state_.x && mouse_x <= state_.x + state_.width) &&
            (mouse_y >= state_.y && mouse_y <= state_.y + state_.height));
}

bool object::state_changed()
{
    bool changed = state_.x != prev_state_.x || state_.y != prev_state_.y || 
        state_.alpha != prev_state_.alpha || state_.invalidate == true;

    prev_state_ = state_;

    return changed;
}

void object::draw_object_bg()
{
    ctx_->set_source_rgb(0,0,0);
    ctx_->rectangle(state_.x,
                    state_.y,
                    state_.width,
                    state_.height);
    ctx_->fill();
}

void object::draw_object_border()
{
    ctx_->line_width(2);
    ctx_->set_source_rgb(1,0,0);
    ctx_->rectangle(state_.x,
                    state_.y,
                    state_.width,
                    state_.height);
    ctx_->stroke();

    ctx_->move_to(state_.x + (state_.width / 2), state_.y);
    ctx_->line_to(state_.x + (state_.width / 2), state_.y + state_.height);
    ctx_->stroke();

    ctx_->move_to(state_.x, (state_.y + state_.height / 2));
    ctx_->line_to(state_.x + state_.width, (state_.y + state_.height / 2));
    ctx_->stroke();
}