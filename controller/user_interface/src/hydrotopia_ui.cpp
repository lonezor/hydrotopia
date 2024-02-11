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

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <time.h>
#include <timer.hpp>
#include <unistd.h>

#include <hydrotopia_ui.hpp>
#include <scene/00_cache_generation_scene/cache_generation_scene.hpp>
#include <scene/01_idle_screen/idle_scene.hpp>
#include <scene/02_security/keypad_scene.hpp>
#include <scene/03_settings/settings_scene.hpp>

hydrotopia_ui::hydrotopia_ui(int screen_width, int screen_height, bool fullscreen)
 : screen_width_(screen_width)
 , screen_height_(screen_height)
 , fullscreen_(fullscreen)
{
    target_fps_ = 120;
    current_fps_ = target_fps_;

    start_ts_ = get_ts();
}

void hydrotopia_ui::scene_init()
{
    scene_idx_++;

    auto scene_01 = std::make_shared<idle_scene>(idle_scene(ctx_, sur_cache_));
    scenes_[scene_idx_++] = scene_01;

    auto scene_02 = std::make_shared<keypad_scene>(keypad_scene(ctx_, sur_cache_));
    scenes_[scene_idx_++] = scene_02;

    auto scene_03 = std::make_shared<settings_scene>(settings_scene(ctx_, sur_cache_));
    scenes_[scene_idx_++] = scene_03;
    
    scene_idx_ = 0;
    scenes_[scene_idx_]->end();
    scenes_[scene_idx_]->invalidate();
}

bool hydrotopia_ui::check_ui_events()
{
    bool scene_updated = false;
    auto events = screen_->poll_events();
    for(auto&& event : events) {
        switch (event->get_type()) {
            case ui_event_type::expose: {
                scenes_[scene_idx_]->invalidate();
                draw_scene();
                scene_updated = true;

                if (!initial_expose_event_) {
                  scene_init();
                  scene_updated = false;
                  initial_expose_event_ = true;
                }
                break;
            }
            case ui_event_type::key_press: {
                auto c = event->get_c();

                printf("c %d\n", c);

                if (c == 27) { // ESC
                    exit_ = true;
                }
                else if (c == 96) {
                    osd_ = !osd_;
                    clear_on_screen_display();
                    draw_scene();
                    scene_updated = true;
                } else {
                    draw_scene(*event);
                }

                break;
            }
            case ui_event_type::pointer_motion:
            case ui_event_type::button_press: {
                // Update mouse coordinates (inverted scaling)
                event->set_x(ctx_->iscale(event->get_x()));
                event->set_y(ctx_->iscale(event->get_y()));

                draw_scene(*event);
                break;
            }
            default:
                break;
        }
    } // end of events loop

    return scene_updated;
}

void hydrotopia_ui::draw_scene()
{
    auto scene = scenes_[scene_idx_];

    if (scene->idle_screen()) {
        scene->reset();
        scene_idx_ = 1;
        scene = scenes_[scene_idx_];
        scene->invalidate();
        scene->begin();
    } else if (scene->ended()) {
        // Last scene. Rewind
        if (scenes_.find(scene_idx_ + 1) == scenes_.end()) {
           exit(0);
        }
        
        scene->reset();
        auto old_scene =  scene;
            
        scene_idx_++;
        auto new_scene = scenes_[scene_idx_];
        new_scene->set_selected_svg_paths(old_scene->selected_svg_paths());
        scene = new_scene;
        scene->invalidate();
        scene->begin();
    } 

    if (scene_idx_ == 2) {
        osd_ = true;
    } else {
        osd_ = false;
    }

    // On screen display (debug)
    if (osd_) {
        draw_on_screen_display();
    } else if (scene_idx_ >= 2) {
        clear_on_screen_display();
    }

    scene->draw();
}

void hydrotopia_ui::draw_scene(ui_event ev)
{
    auto scene = scenes_[scene_idx_];

    if (scene_idx_ == 2) {
        osd_ = true;
    } else {
        osd_ = false;
    }

    // On screen display (debug)
    if (osd_) {
        draw_on_screen_display();
    } else if (scene_idx_ >= 2) {
        clear_on_screen_display();
    }

    scene->draw(ev);
}

std::string hydrotopia_ui::elapsed_time_str(int64_t elapsed_time) {
    elapsed_time /= 1000000; // to seconds

    auto days = elapsed_time / (3600 * 24);
    elapsed_time -= days * (3600 * 24);
    
    auto hours = elapsed_time / 3600;
    elapsed_time -= hours * (3600);

    auto minutes = elapsed_time / 60;
    auto seconds = elapsed_time % 60;

    std::ostringstream oss;
    oss << std::setfill('0');
    oss << std::setw(3);
    oss << days << ":";
    oss << std::setw(2);
    oss << hours << ":";
    oss << std::setw(2);
    oss << minutes << ":";
    oss << std::setw(2);
    oss << seconds;

    return oss.str();
}

void hydrotopia_ui::clear_on_screen_display()
{
    ctx_->set_source_rgb(1, 1, 1);
    ctx_->rectangle(1970,1520, 750, 100);
    ctx_->fill();
}

void hydrotopia_ui::draw_on_screen_display()
{
    auto elapsed_time = get_ts() - start_ts_;
    auto elapsed_str = elapsed_time_str(elapsed_time);

    time_t t = time(nullptr);
    auto ts = std::string(asctime(localtime(&t)));
    for(int i=0; i<9; i++) {
        ts.pop_back();
    }
    
    std::ostringstream oss;
    oss << ts;
    
    ctx_->set_source_rgb(1, 1, 1);
    ctx_->rectangle(1970,1520, 750, 100);
    ctx_->fill();

    ctx_->set_source_rgb(0, 0, 0);
    ctx_->move_to(1975,1585);
    ctx_->font_size(75);
    ctx_->show_text(oss.str());
}

void hydrotopia_ui::run()
{
    screen_ = std::shared_ptr<xlib_screen>(new xlib_screen(screen_width_, screen_height_, 0, 0, "Hydrotopia User Interface", fullscreen_));

    ctx_ = std::make_shared<rendering_context>(rendering_context(screen_,
                          ref_width,
                          ref_height,
                          anti_aliasing::best));

    ctx_->font_face("Lato Black", font_slant::normal, font_weight::normal);

    sur_cache_ = std::make_shared<surface_cache>(surface_cache(screen_width_, screen_height_));

    // Display splash screen while loading background
    scenes_[scene_idx_] = std::make_shared<cache_generation_scene>(cache_generation_scene(ctx_, sur_cache_));
    scene_idx_ = 0;

    auto screen_update_timer = timer();
    double period = 1000.0 / target_fps_;
    auto timer_id = screen_update_timer.register_one_shot_timer(std::chrono::milliseconds(static_cast<int>(period)));

    while(!exit_) {
        auto events = screen_update_timer.wait_for_events();
        for(auto&& event : events) {
            if (event->id() != timer_id) {
                continue;
            }
            auto ts1 = get_ts();

            auto scene_updated = check_ui_events();
            if (!scene_updated) {
                draw_scene();
            }

            auto ts2 = get_ts();
            auto diff = ts2 - ts1;

            double expected_period = 1000000.0 / current_fps_;
            if (diff > expected_period * 1.1) {
                current_fps_ /= diff / expected_period;
                    if (current_fps_ < 1) {
                        current_fps_ = 1;
                    }
                } else {
                    if (current_fps_ < target_fps_) {
                        current_fps_ *= expected_period / diff;
                    }
                    if (current_fps_ > target_fps_) {
                        current_fps_ = target_fps_;
                    }
            }

            period = 1000.0 / current_fps_;
            timer_id = screen_update_timer.register_one_shot_timer(std::chrono::milliseconds(static_cast<int>(period)));
        }
    }

    screen_->close();
}

