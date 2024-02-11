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

class image_object : public object
{
    public:
        image_object(std::shared_ptr<rendering_context> ctx,
                    std::shared_ptr<surface_cache> sur_cache,
                    double x,
                    double y,
                    double width,
                    double height,
                    std::string png_path,
                    std::string dino_name);

        void draw() final;

        void draw(ui_event ev);

        bool is_selected();

        std::string png_path() { return png_path_; }

        void unselect();
    private:
        void internal_draw();

        std::string png_path_;

        std::string dino_name_;

        bool hover_{false};

        bool selected_{false};

        std::shared_ptr<surface> checkmark_surface_;
};

