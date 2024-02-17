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

#include <common.hpp>
#include <object/background_object.hpp>
#include <scene/02_security/keypad_scene.hpp>

keypad_scene::keypad_scene(std::shared_ptr<rendering_context> ctx, std::shared_ptr<surface_cache> sur_cache)
  : scene(ctx, sur_cache)
{
  auto obj = std::shared_ptr<object>(new background_object(ctx_, sur_cache_, 0, 0,
         ctx->screen_width(), ctx->screen_height()));
  objects_.emplace_back(obj);

  // Preload all objects
  auto path = "/usr/share/hydrotopia_ui/images/tree_02.png";
  background_ = std::shared_ptr<image_object>(new image_object(ctx_, sur_cache_,
                                                                  1150,
                                                                  300,
                                                                  0,
                                                                  0,
                                                                  path,
                                                                  ""));

  keypad_object_01_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 100, 300, 320, 160, navigation_state::keypad_01));

  keypad_object_02_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 400, 300, 320, 160, navigation_state::keypad_02));

  keypad_object_03_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 700, 300, 320, 160, navigation_state::keypad_03));

  keypad_object_04_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 100, 550, 320, 160, navigation_state::keypad_04));

  keypad_object_05_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 400, 550, 320, 160, navigation_state::keypad_05));

  keypad_object_06_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 700, 550, 320, 160, navigation_state::keypad_06));

  keypad_object_07_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 100, 800, 320, 160, navigation_state::keypad_07));

  keypad_object_08_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 400, 800, 320, 160, navigation_state::keypad_08));

  keypad_object_09_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 700, 800, 320, 160, navigation_state::keypad_09));

  keypad_object_star_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 100, 1050, 320, 160, navigation_state::keypad_star));

  keypad_object_00_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 400, 1050, 320, 160, navigation_state::keypad_00));

  keypad_object_hash_ = std::shared_ptr<navigate_object>(new navigate_object(ctx_,
    sur_cache_, 700, 1050, 320, 160, navigation_state::keypad_hash));

  header_text_ = std::shared_ptr<text_object>(new text_object(ctx_, sur_cache_, 170, 140, 1300, 75, "Enter Code To Unlock", 75));
  header_text_->set_bg(1,1,1);

  status_text_ = std::shared_ptr<text_object>(new text_object(ctx_, sur_cache_, 170, 1340, 1300, 75, "", 100));
  status_text_->set_bg(1,1,1);
}

void keypad_scene::begin()
{

}


void keypad_scene::draw()
{
    for(auto&& object : objects_) {
        object->draw();
    }

    background_->draw();
    header_text_->draw();
    keypad_object_01_->draw();
    keypad_object_02_->draw();
    keypad_object_03_->draw();
    keypad_object_04_->draw();
    keypad_object_05_->draw();
    keypad_object_06_->draw();
    keypad_object_07_->draw();
    keypad_object_08_->draw();
    keypad_object_09_->draw();
    keypad_object_star_->draw();
    keypad_object_00_->draw();
    keypad_object_hash_->draw();

    if (started_ts_ == 0) {
        started_ts_ = get_ts();
    }

    bool reset = false;
    auto timeout = std::chrono::duration_cast<std::chrono::milliseconds>(active_screen_timeout);
    auto elapsed_time = (get_ts() - started_ts_) / 1000; // unit: ms

    if (started_ts_ != 0 && elapsed_time > timeout.count()) {
      idle_screen_ = true;
      reset = true;
      draw_counter_ = 0;
      started_ts_ = 0;
    }

    if (reset) {
      for(auto&& object : objects_) {
        object->invalidate();
      }

      background_->invalidate();
      header_text_->invalidate();
      keypad_object_01_->invalidate();
      keypad_object_02_->invalidate();
      keypad_object_03_->invalidate();
      keypad_object_04_->invalidate();
      keypad_object_05_->invalidate();
      keypad_object_06_->invalidate();
      keypad_object_07_->invalidate();
      keypad_object_08_->invalidate();
      keypad_object_09_->invalidate();
      keypad_object_star_->invalidate();
      keypad_object_00_->invalidate();
      keypad_object_hash_->invalidate();
    }
}

void keypad_scene::draw(ui_event ev)
{
  // User interactions resets screen timer
  started_ts_ = get_ts();

  for(auto&& object : objects_) {
      object->draw(ev);
  }

  background_->draw();

  header_text_->draw();

    bool pressed = false;
    bool evaluate = false;

    if (keypad_object_01_->update_ev_state(ev, false)) {
      keypad_object_01_->draw(ev);
      if (keypad_object_01_->is_selected()) {
        code_ += "1";
        pressed = true;
        keypad_object_01_->unselect();
      }
    }

    if (keypad_object_02_->update_ev_state(ev, false)) {
      keypad_object_02_->draw(ev);
      if (keypad_object_02_->is_selected()) {
        code_ += "2";
        pressed = true;
        keypad_object_02_->unselect();
      }
    }

    if (keypad_object_03_->update_ev_state(ev, false)) {
      keypad_object_03_->draw(ev);
      if (keypad_object_03_->is_selected()) {
        code_ += "3";
        pressed = true;
        keypad_object_03_->unselect();
      }
    }

    if (keypad_object_04_->update_ev_state(ev, false)) {
      keypad_object_04_->draw(ev);
      if (keypad_object_04_->is_selected()) {
        code_ += "4";
        pressed = true;
        keypad_object_04_->unselect();
      }
    }

    if (keypad_object_05_->update_ev_state(ev, false)) {
      keypad_object_05_->draw(ev);
      if (keypad_object_05_->is_selected()) {
        code_ += "5";
        pressed = true;
        keypad_object_05_->unselect();
      }
    }

    if (keypad_object_06_->update_ev_state(ev, false)) {
      keypad_object_06_->draw(ev);
      if (keypad_object_06_->is_selected()) {
        code_ += "6";
        pressed = true;
        keypad_object_06_->unselect();
      }
    }

    if (keypad_object_07_->update_ev_state(ev, false)) {
      keypad_object_07_->draw(ev);
      if (keypad_object_07_->is_selected()) {
        code_ += "7";
        pressed = true;
        keypad_object_07_->unselect();
      }
    }

    if (keypad_object_08_->update_ev_state(ev, false)) {
      keypad_object_08_->draw(ev);
      if (keypad_object_08_->is_selected()) {
        code_ += "8";
        pressed = true;
        keypad_object_08_->unselect();
      }
    }

    if (keypad_object_09_->update_ev_state(ev, false)) {
      keypad_object_09_->draw(ev);
      if (keypad_object_09_->is_selected()) {
        code_ += "9";
        pressed = true;
        keypad_object_09_->unselect();
      }
    }

    if (keypad_object_star_->update_ev_state(ev, false)) {
      keypad_object_star_->draw(ev);
      if (keypad_object_star_->is_selected()) {
        code_ = "";
        pressed = true;
        keypad_object_star_->unselect();
      }
    }

    if (keypad_object_00_->update_ev_state(ev, false)) {
      keypad_object_00_->draw(ev);
      if (keypad_object_00_->is_selected()) {
        code_ += "0";
        pressed = true;
        keypad_object_00_->unselect();
      }
    }

    if (keypad_object_hash_->update_ev_state(ev, false)) {
      keypad_object_hash_->draw(ev);
      if (keypad_object_hash_->is_selected()) {
        evaluate = true;
        keypad_object_hash_->unselect();
      }
    }

    while(code_.size() > 17) {
      code_.pop_back();
    }

    if (evaluate) {
      // Touch screen child safety
      if (code_ == "8090") {
        ended_ = true;

        for(auto&& object : objects_) {
          object->invalidate();
        }

        background_->invalidate();
        header_text_->invalidate();
        keypad_object_01_->invalidate();
        keypad_object_02_->invalidate();
        keypad_object_03_->invalidate();
        keypad_object_04_->invalidate();
        keypad_object_05_->invalidate();
        keypad_object_06_->invalidate();
        keypad_object_07_->invalidate();
        keypad_object_08_->invalidate();
        keypad_object_09_->invalidate();
        keypad_object_star_->invalidate();
        keypad_object_00_->invalidate();
        keypad_object_hash_->invalidate();
      } else {
        code_ = "";
      }
    }

    if (pressed || evaluate) {
      std::string hidden_code;
      for(int i=0; i < code_.size(); i++) {
        hidden_code += "*";
      }

      status_text_->set_text(hidden_code);
      status_text_->draw();
    }

  // safety
  if (draw_counter_ > 100000) {
      exit(0);
  }

  draw_counter_++;
}
