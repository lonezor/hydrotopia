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

#include <memory>

#include <scene/00_cache_generation_scene/cache_generation_scene.hpp>
#include <object/text_object.hpp>
#include <common.hpp>

cache_generation_scene::cache_generation_scene(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache)
  : scene(ctx, sur_cache)
{
  auto obj = std::shared_ptr<object>(new text_object(ctx_, sur_cache_, 520, 345, 1270, 25, "Loading...", 60));
  objects_.emplace_back(obj);
}

void cache_generation_scene::draw()
{
    for(auto&& object : objects_) {
        object->draw();
    }
}

void cache_generation_scene::draw(ui_event ev)
{
    for(auto&& object : objects_) {
        object->draw();
    }
}

