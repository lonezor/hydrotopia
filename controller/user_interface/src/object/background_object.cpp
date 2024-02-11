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

#include <object/background_object.hpp>

background_object::background_object(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache, double x, double y, double width, double height)
 : object(ctx, sur_cache, x, y, width, height)
{


}

void background_object::set_bg(double r, double g, double b)
{
    bg_r_ = r;
    bg_g_ = g;
    bg_b_ = b;
}

void background_object::draw()
{
    if (!state_changed()) {
        return;
    }

    ctx_->set_source_rgb(bg_r_, bg_g_, bg_b_);

    ctx_->rectangle(0,0,ref_width, ref_height);
    ctx_->fill();

    //draw_object_border();

    state_.invalidate = false;
}

void background_object::draw(ui_event ev)
{

}

