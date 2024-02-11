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

#include <object/splash_screen_object.hpp>

splash_screen_object::splash_screen_object(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache, double x, double y, double width, double height)
 : object(ctx, sur_cache, x, y, width, height)
{
    // workaround due to issues with very big svg image on high dpi resolutions
    if (ctx->screen_height() > 1200) {
        surface_ = sur_cache_->get_png_surface("/usr/share/hydrotopia_ui/images/Title_hq.png");
    } else {
        surface_ = sur_cache_->get_svg_surface("/usr/share/hydrotopia_ui/images/Title.svg",
                                               ctx_->scale(width),
                                               ctx_->scale(height));
    }
}

void splash_screen_object::draw()
{
    if (!state_changed()) {
        return;
    }

    ctx_->draw_surface(surface_, 0, 0, 1.0);
    state_.invalidate = false;
}

void splash_screen_object::draw(ui_event ev)
{

}
