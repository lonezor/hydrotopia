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
#include <memory>

#include <scene/scene.hpp>
#include <object/object.hpp>
#include <graphics_context/rendering_context.hpp>
#include <graphics_context/surface_cache.hpp>

class cache_generation_scene : public scene
{
    public:
        cache_generation_scene(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache);

        void draw() final;
        void draw(ui_event ev) final;
};

