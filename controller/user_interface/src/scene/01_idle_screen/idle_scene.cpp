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
#include <iostream>

#include <scene/01_idle_screen/idle_scene.hpp>
#include <object/background_object.hpp>
#include <object/splash_screen_object.hpp>
#include <common.hpp>

idle_scene::idle_scene(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache)
  : scene(ctx, sur_cache)
{
  auto bg = new background_object(ctx_, sur_cache_, 0, 0,
         ctx->screen_width(), ctx->screen_height());
  bg->set_bg(0,0,0);

  auto obj = std::shared_ptr<object>(bg);
  objects_.emplace_back(obj);
}

void idle_scene::begin()
{
  started_ts_ = get_ts();
}

void idle_scene::draw()
{
    for(auto&& object : objects_) {
        object->draw();
    }
}

void idle_scene::draw(ui_event ev)
{
    for(auto&& object : objects_) {
        object->draw();
    }

  if (ev.get_x() > 0 && ev.get_y() > 0) {
    ended_ = true;
  }
}
