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

#include <user_interface/screen.hpp>
#include <memory>
#include <graphics_context/surface.hpp>
#include <common.hpp>

enum class anti_aliasing
{
    none,
    fast,
    best

};

enum class font_slant
{
    normal,
    italic,
    oblique,
};

enum class font_weight
{
    normal,
    bold,
};

constexpr double m_pi = 3.14159265358979323846;

class rendering_context
{
    public:
        rendering_context(std::shared_ptr<screen> screen,
                          double ref_width,
                          double ref_height,
                          anti_aliasing anti_aliasing);
        
        void set_source_rgb(double r, double g, double b);
        void set_source_rgba(double r, double g, double b, double a);
        void fill();
        void stroke();
        void paint();
        double scale(double value);
        double iscale(double value);

        double screen_width();
        double screen_height();

        void set_dash(const double* dashes,
                      int num_dashes,
                      double offset);

        void line_width(double width);
        void move_to(double x, double y);
        void line_to(double x, double y);
        void close_path();
        void arc(double xc, double yc, double radius, double angle1, double angle2);
        void rectangle(double x, double y, double width, double height);
        void font_face(std::string name, font_slant slant, font_weight weight);
        void font_size(double size);
        void show_text(std::string text);



        void draw_surface(std::shared_ptr<surface> surface, double x, double y, double alpha);

    private:
        std::shared_ptr<screen> screen_;
        double ref_width_;
        double ref_height_;
        anti_aliasing anti_aliasing_;
        double scale_multiplier_;
};