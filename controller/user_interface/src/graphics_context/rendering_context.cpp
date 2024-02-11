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

#include <graphics_context/rendering_context.hpp>

rendering_context::rendering_context(std::shared_ptr<screen> screen,
                  double ref_width,
                  double ref_height,
                  anti_aliasing anti_aliasing)
    : screen_(screen)
    , ref_width_(ref_width)
    , ref_height_(ref_height)
    , anti_aliasing_(anti_aliasing)
{
    // Calculate scale multiplier
    // Assumption: using the same aspect ratio
    scale_multiplier_ = static_cast<double>(screen_->width()) / ref_width_;

    auto cr = screen_->root_surface()->cr();

    cairo_antialias_t cr_antialias = CAIRO_ANTIALIAS_NONE;
    switch(anti_aliasing) {
        case anti_aliasing::none:
            cr_antialias = CAIRO_ANTIALIAS_NONE;
            break;
        case anti_aliasing::fast:
            cr_antialias = CAIRO_ANTIALIAS_FAST;
            break;
        case anti_aliasing::best:
            cr_antialias = CAIRO_ANTIALIAS_BEST;
            break;
    }
    cairo_set_antialias(cr, cr_antialias);

    font_face("DejaVu Sans Book", font_slant::normal, font_weight::normal);
}

double rendering_context::screen_width()
{
    return static_cast<double>(screen_->width());
}

double rendering_context::screen_height()
{
    return static_cast<double>(screen_->height());
}

double rendering_context::scale(double value)
{
    return value * scale_multiplier_;
}

double rendering_context::iscale(double value)
{
    return value / scale_multiplier_;
}

void rendering_context::draw_surface(std::shared_ptr<surface> surface, double x, double y, double alpha)
{
    auto cr = screen_->root_surface()->cr();
    cairo_set_source_surface (cr, surface->handle(), scale(x), scale(y));
    cairo_paint_with_alpha (cr, alpha);
    cairo_paint(cr);
}

void rendering_context::set_source_rgb(double r, double g, double b)
{
    auto cr = screen_->root_surface()->cr();
    cairo_set_source_rgb(cr, r, g, b);
}

void rendering_context::set_source_rgba(double r, double g, double b, double a)
{
    auto cr = screen_->root_surface()->cr();
    cairo_set_source_rgba(cr, r, g, b, a);
}

void rendering_context::line_width(double width)
{
    auto cr = screen_->root_surface()->cr();
    cairo_set_line_width(cr, scale(width));
}

void rendering_context::move_to(double x, double y)
{
    auto cr = screen_->root_surface()->cr();
    cairo_move_to(cr, scale(x), scale(y));
}

void rendering_context::line_to(double x, double y)
{
    auto cr = screen_->root_surface()->cr();
    cairo_line_to(cr, scale(x), scale(y));
}

void rendering_context::close_path()
{
    auto cr = screen_->root_surface()->cr();
    cairo_close_path(cr);
}

void rendering_context::arc(double xc, double yc, double radius, double angle1, double angle2)
{
    auto cr = screen_->root_surface()->cr();
    cairo_arc(cr, scale(xc), scale(yc), scale(radius), angle1, angle2);
}

void rendering_context::rectangle(double x, double y, double width, double height)
{
    auto cr = screen_->root_surface()->cr();
    cairo_rectangle(cr, scale(x), scale(y), scale(width), scale(height));
}

void rendering_context::fill()
{
    auto cr = screen_->root_surface()->cr();
    cairo_fill(cr);
}

void rendering_context::stroke()
{
    auto cr = screen_->root_surface()->cr();
    cairo_stroke(cr);
}

void rendering_context::paint()
{
    auto cr = screen_->root_surface()->cr();
    cairo_paint(cr);
}

void rendering_context::font_face(std::string name, font_slant slant, font_weight weight)
{
    cairo_font_slant_t cr_slant;
    switch(slant)
    {
        case font_slant::normal:
            cr_slant = CAIRO_FONT_SLANT_NORMAL;
            break;
        case font_slant::italic:
            cr_slant = CAIRO_FONT_SLANT_ITALIC;
            break;
        case font_slant::oblique:
            cr_slant = CAIRO_FONT_SLANT_OBLIQUE;
            break;
    }

    cairo_font_weight_t cr_weight;
    switch(weight)
    {
        case font_weight::normal:
            cr_weight = CAIRO_FONT_WEIGHT_NORMAL;
            break;
        case font_weight::bold:
            cr_weight = CAIRO_FONT_WEIGHT_BOLD;
            break;
    }
    
    auto cr = screen_->root_surface()->cr();
    cairo_select_font_face(cr, name.c_str(), cr_slant, cr_weight);
}

void rendering_context::font_size(double size)
{
    auto cr = screen_->root_surface()->cr();
    cairo_set_font_size(cr, scale(size));
}

void rendering_context::show_text(std::string text)
{
    auto cr = screen_->root_surface()->cr();
    cairo_show_text(cr, text.c_str());
}

void rendering_context::set_dash(const double* dashes,
                      int num_dashes,
                      double offset)
{
    auto cr = screen_->root_surface()->cr();
    cairo_set_dash (cr,
                    dashes,
                    num_dashes,
                    offset);  
}
