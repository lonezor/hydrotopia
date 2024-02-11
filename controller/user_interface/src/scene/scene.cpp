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

#include <iostream>

#include <scene/scene.hpp>

scene::scene(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache)
 : ctx_(ctx)
 , sur_cache_(sur_cache)
{

}

#if 0
void scene::draw()
{
    std::cout << "scene::draw()" << std::endl;
}
#endif

void scene::add_object(std::shared_ptr<object> object)
{
    objects_.emplace_back(object);
}

void scene::invalidate()
{
    for(auto&& object : objects_) {
        object->invalidate();
    }
}