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
#include <cairo.h>
#include <librsvg/rsvg.h>

#include <common.hpp>

class surface
{
    public:
        surface();

        surface(double width, double height);

        surface(cairo_surface_t* surface, cairo_t* cr, double width, double height);

        ~surface();

        void load_background(double r, double g, double b);

        void load_from_svg(std::string path);

        void load_from_png(std::string path);

        void write_png(std::string path);

        void draw_surface(std::shared_ptr<surface> surface, double x, double y, double alpha);

        void fill(double r, double g, double b);

        cairo_surface_t* handle() { return surface_; }

        cairo_t* cr() { return cr_; }

        double width() { return width_; }

        double height() { return height_; }

        void destroy();

    private:
        double width_;

        double height_;

        cairo_surface_t* surface_{nullptr};

        cairo_t* cr_{nullptr};

        RsvgHandle* rsvg_;

        RsvgDimensionData dim_;

        double center_x_;

        double center_y_;
};