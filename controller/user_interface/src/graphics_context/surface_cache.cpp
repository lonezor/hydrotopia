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

#include <vector>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <stdio.h>


static pthread_mutex_t utils_basename_mutex = PTHREAD_MUTEX_INITIALIZER;

#include <graphics_context/surface_cache.hpp>

surface_cache::surface_cache(int screen_width, int screen_height)
    : screen_width_(screen_width)
    , screen_height_(screen_height)
{

}

surface_key surface_cache::create_key(std::string path, double width, double height)
{
    return path + "_" + std::to_string(width) + "_" + std::to_string(height);
}

bool
surface_cache::path_exists(const std::string& path)
{
    struct stat buffer
    {};
    return (stat(path.c_str(), &buffer) == 0);
}

std::string
surface_cache::utils_basename(std::string path)
{
    char* dup_str = strdup(path.c_str()); // path may be const

    pthread_mutex_lock(&utils_basename_mutex);
    std::string base_name = std::string(basename(dup_str));
    pthread_mutex_unlock(&utils_basename_mutex);

    free(dup_str);

    return base_name;
}

std::string surface_cache::get_home_path()
{
    std::string path = "/";
    auto uid = getuid();
    auto pws = getpwuid(uid);
    if (pws != nullptr) {
        auto name = std::string(pws->pw_name);
        if (name == "root") {
            path += name;
        } else {
           path += "home/" + name;
        }
    }

    return path;
}


std::string surface_cache::get_cache_filename(std::string path, double width, double height)
{
    auto filename = utils_basename(path);
    filename += ".cache_";
    filename += std::to_string(screen_width_) + "_";
    filename += std::to_string(screen_height_) + "_";
    filename += std::to_string(static_cast<int>(width)) + "_";
    filename += std::to_string(static_cast<int>(height)) + ".png";
    return filename;
}

std::string surface_cache::get_dino_root()
{
    auto home_path = get_home_path();
    return home_path + "/.hydrotopia_ui";
}

void surface_cache::update_persistent_png_cache(std::string path, double width, double height, std::shared_ptr<surface> surface)
{

    std::string cmd = "mkdir -p " + get_dino_root();
    system(cmd.c_str());
    std::string cache_path = get_dino_root() + "/" + get_cache_filename(path, width, height);

    if (path_exists(cache_path)) {
        return;
    }

    surface->write_png(cache_path.c_str());
}

std::shared_ptr<surface> surface_cache::load_from_persistent_cache(std::string path, double width, double height)
{
    auto cache_filename = get_cache_filename(path, width, height);
    std::string cache_path = get_dino_root() + "/" + cache_filename;

    if (!path_exists(cache_path)) {
        return nullptr;
    }

    return get_png_surface(cache_path);
}

std::shared_ptr<surface> surface_cache::get_svg_surface(std::string path, double width, double height)
{
    //purge_outdated_entries();

    auto key = create_key(path, width, height);

    // Already available
    if (cache_.find(key) != cache_.end()) {
        cache_[key].last_accessed = get_ts();
        return cache_[key].cached_surface;
    }

    auto s = load_from_persistent_cache(path, width, height);
    if (s == nullptr) {
        // Create
        s = std::shared_ptr<surface>(new surface(width, height));
        s->load_from_svg(path);
        update_persistent_png_cache(path, width, height, s);

        // Populate cache   
        cache_entry entry;
        entry.last_accessed = get_ts();
        entry.cached_surface = s;
        cache_[key] = entry;
    }

    return s;
}

std::shared_ptr<surface> surface_cache::get_png_surface(std::string path)
{
    //purge_outdated_entries();

    auto key = create_key(path, 0, 0);

    // Already available
    if (cache_.find(key) != cache_.end()) {
        cache_[key].last_accessed = get_ts();
        return cache_[key].cached_surface;
    }

    // Create
    auto s = std::shared_ptr<surface>(new surface());
    s->load_from_png(path);

    // Populate cache
    cache_entry entry;
    entry.last_accessed = get_ts();
    entry.cached_surface = s;
    cache_[key] = entry;

    return s;
}

void surface_cache::purge_outdated_entries()
{
    std::vector<std::string> keys_to_remove;

    for(auto&& e : cache_) {
        auto key = e.first;

        auto elapsed_time_ms = (get_ts() - e.second.last_accessed) / 1000;
        if (elapsed_time_ms > 60000) {
            keys_to_remove.emplace_back(key);
            e.second.cached_surface = nullptr;
        }
    }

    for(auto&& key : keys_to_remove) {
        cache_.erase(key);
    }
}

