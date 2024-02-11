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

#include <object/object.hpp>
#include <graphics_context/rendering_context.hpp>

class text_object : public object
{
    public:
        text_object(std::shared_ptr<rendering_context> ctx,
                        std::shared_ptr<surface_cache> sur_cache,
                        double x,
                        double y,
                        double width,
                        double height,
                        std::string str,
                        double str_size);

        void draw() final;
        void draw(ui_event ev);

        void set_text(std::string str);
        void set_size(double size);

        void set_bg(double r, double g, double b);

    private:
        std::string str_;
        double str_size_;
        double bg_r_{0};
        double bg_g_{0};
        double bg_b_{0};
};

