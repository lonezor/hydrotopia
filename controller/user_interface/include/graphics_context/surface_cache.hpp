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

#include <memory>
#include <map>

#include "surface.hpp"

using surface_key = std::string; // path + '_' + width + '_' + height

struct cache_entry
{
    std::shared_ptr<surface> cached_surface;
    int64_t last_accessed;
};

class surface_cache
{
    public:
        surface_cache(int screen_width, int screen_height);

        std::shared_ptr<surface> get_svg_surface(std::string path, double width, double height);
        
        std::shared_ptr<surface> get_png_surface(std::string path);

    private:
        surface_key create_key(std::string path, double width, double height);

        void purge_outdated_entries();

        bool path_exists(const std::string& path);

        std::string
        utils_basename(std::string path);

        std::string get_home_path();

        std::string get_dino_root();

        std::string get_cache_filename(std::string path, double width, double height);

        void update_persistent_png_cache(std::string path, double width, double height, std::shared_ptr<surface> surface);

        std::shared_ptr<surface> load_from_persistent_cache(std::string path, double width, double height);

        std::map<surface_key,cache_entry> cache_;

        int screen_width_;

        int screen_height_;


};