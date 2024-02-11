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

#include <fstream>
#include <memory>

#include <graphics_context/surface.hpp>

surface::surface()
{
}

surface::surface(double width, double height)
 : width_(width)
 , height_(height)
{
}

surface::surface(cairo_surface_t* surface, cairo_t* cr, double width, double height)
 : surface_(surface)
 , cr_(cr)
 , width_(width)
 , height_(height)
{
}

surface::~surface()
{
    destroy();
}

void surface::destroy()
{
    if (cr_ != nullptr) {
        cairo_destroy(cr_);
        cr_ = nullptr;
    }

    if (surface_ != nullptr) {
        cairo_surface_destroy(surface_);
        surface_ = nullptr;
    }
}

void surface::load_background(double r, double g, double b)
{
    surface_ = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width_, height_);

    if (surface_ != nullptr) {
        cr_ = cairo_create(surface_);
        width_ = static_cast<double>(cairo_image_surface_get_width(surface_));
        height_ = static_cast<double>(cairo_image_surface_get_height(surface_));

        cairo_set_source_rgb(cr_, r, g, b);
        cairo_rectangle(cr_, 0, 0, width_, height_);
        cairo_fill(cr_);
    }
}

void surface::load_from_svg(std::string path)
{
    std::ifstream f(path, std::ios::in|std::ios::binary|std::ios::ate);
    if (!f.is_open()) {
        return; // todo: exception
    }

    auto size = f.tellg();
    std::unique_ptr<char> buffer(new char[size]);

    f.seekg (0, std::ios::beg);
    f.read (buffer.get(), size);
    f.close();

    GError* error = nullptr;
     
    rsvg_ = rsvg_handle_new_from_data((const guint8*)buffer.get(), (gsize)size, &error);
    
    if (error != nullptr)
    {
        printf("Error: %s\n", error->message);
        return;
    }

    // Set high DPI to accomodate any screen resolution up to 8k
    // Scale it with respect to the actually needed resolution since
    // this has a very significant effect on performance.
    double prop = width_ / static_cast<double>(ref_width);
    double dpi = 660 * prop;
    if (dpi < 1) {
        dpi = 1;
    }
    if (dpi > 660) {
        dpi = 660;
    }
    printf("Loading %s (DPI %.3f)\n", path.c_str(), dpi);

    rsvg_handle_set_dpi(rsvg_, dpi);

    // Assumption: SVG object scaled to full width of the document
    rsvg_handle_get_dimensions(rsvg_, &dim_);

    // Create full size surface
    double width = (double)dim_.width;
    double height = (double)dim_.height;
    surface_ = cairo_image_surface_create (CAIRO_FORMAT_ARGB32, width, height);
    cr_ = cairo_create(surface_);

    // Before rendering, set scaling to requested resolution
    double ar = width_ / height_;
    double sx = width_ / width;
    double sy = height_ / (width / ar);

    center_x_ = (width * sx) / 2;
    center_y_ = center_x_; // currently, this is more useful

    cairo_scale(cr_, sx, sy);

    // Render SVG to cairo surface
    rsvg_handle_render_cairo(rsvg_, cr_);

    auto w =  cairo_image_surface_get_width (surface_);
    auto h = cairo_image_surface_get_height (surface_);

    g_object_unref(rsvg_);
}

void surface::load_from_png(std::string path)
{
    surface_ = cairo_image_surface_create_from_png(path.c_str());

    if (surface_ != nullptr) {
        cr_ = cairo_create(surface_);
        width_ = static_cast<double>(cairo_image_surface_get_width(surface_));
        height_ = static_cast<double>(cairo_image_surface_get_height(surface_));
    }
}

void surface::write_png(std::string path)
{
    if (surface_ != nullptr) {
        printf("Writing %s\n", path.c_str());
        cairo_surface_write_to_png(surface_, path.c_str());
    }
}

void surface::fill(double r, double g, double b) {
    if (cr_ == nullptr) {
        return;
    }

    cairo_set_source_rgb(cr_, r, g, b);
    cairo_rectangle(cr_, 0, 0, width_, height_);
    cairo_fill(cr_);
}

void surface::draw_surface(std::shared_ptr<surface> surface, double x, double y, double alpha)
{
    if (cr_ == nullptr) {
        return;
    }

    cairo_set_source_surface (cr_, surface->handle(), x, y);
    cairo_paint_with_alpha (cr_, alpha);
    cairo_paint(cr_);
}

