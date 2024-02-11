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

#include <object/dashed_line_object.hpp>

dashed_line_object::dashed_line_object(std::shared_ptr<rendering_context> ctx,
                          std::shared_ptr<surface_cache> sur_cache,
                          coordinate start_coord,
                          coordinate end_coord,
                          double r,
                          double g,
                          double b,
                          double width,
                          std::vector<double>& dashes,
                          double dash_offset)
    : object(ctx, sur_cache, 0, 0, 0, 0)
    , start_coord_(start_coord)
    , end_coord_(end_coord)
    , r_(r)
    , g_(g)
    , b_(b)
    , width_(width)
    , dashes_(dashes)
    , dash_offset_(dash_offset)
{

}
        
void dashed_line_object::internal_draw()
{
    double* dashes = static_cast<double*>(calloc(dashes_.size(), sizeof(double)));

    size_t idx = 0;
    for(auto&& d : dashes_) {
        dashes[idx++] = d;
    }

    ctx_->set_source_rgb(r_, g_, b_);
    ctx_->line_width(width_);
    ctx_->set_dash(dashes, dashes_.size(), dash_offset_);
    ctx_->move_to(start_coord_.x, start_coord_.y);
    ctx_->line_to(end_coord_.x, end_coord_.y);
    ctx_->stroke();

    free(dashes);
}

void dashed_line_object::draw()
{
    if (!state_changed()) {
        return;
    }

    internal_draw();

    state_.invalidate = false;
}

void dashed_line_object::draw(ui_event ev)
{
    if (!state_changed()) {
        return;
    }

    internal_draw();

    state_.invalidate = false;
}

