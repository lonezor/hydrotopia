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

#include <object/object.hpp>
#include <graphics_context/rendering_context.hpp>


class dashed_line_object : public object
{
    public:
        dashed_line_object(std::shared_ptr<rendering_context> ctx,
                          std::shared_ptr<surface_cache> sur_cache,
                          coordinate start_coord,
                          coordinate end_coord,
                          double r,
                          double g,
                          double b,
                          double width,
                          std::vector<double>& dashes,
                          double dash_offset);
        
        void draw() final;

        void draw(ui_event ev);

     private:
        void internal_draw();

        coordinate start_coord_;
        coordinate end_coord_;
        double r_;
        double g_;
        double b_;
        double width_;
        std::vector<double> dashes_;
        double dash_offset_;
};

