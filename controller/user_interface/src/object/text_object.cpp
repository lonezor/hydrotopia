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

#include <object/text_object.hpp>

text_object::text_object(std::shared_ptr<rendering_context> ctx,
            std::shared_ptr<surface_cache> sur_cache,
            double x,
            double y,
            double width,
            double height,
            std::string str,
            double str_size)
    : object(ctx, sur_cache, x, y, width, height)
    , str_(str)
    , str_size_(str_size)
{

}

void text_object::set_text(std::string str)
{
    if (str_ != str) {
        str_ = str;
        invalidate();
    }
}

void text_object::set_bg(double r, double g, double b)
{
  bg_r_ = r;
  bg_g_ = g;
  bg_b_ = b;
}

void text_object::set_size(double size)
{
    str_size_ = size;
}

void text_object::draw()
{
    if (!state_changed()) {
        return;
    }

    ctx_->set_source_rgb(bg_r_, bg_g_, bg_b_);
    ctx_->rectangle(state_.x - (str_size_ / 3.1),
                    state_.y - (str_size_ / 3.1),
                    state_.width + (str_size_ / 3.1),
                    state_.height + (str_size_ / 3.1));
    ctx_->fill();

    ctx_->move_to(state_.x , state_.y + (7 * (str_size_ / 10)));
    ctx_->set_source_rgb(0, 0, 0);
    ctx_->font_size(str_size_);
    ctx_->show_text(str_.c_str());

    state_.invalidate = false;
}

void text_object::draw(ui_event ev)
{
    if (!state_changed()) {
        return;
    }

    state_.invalidate = false;
}

