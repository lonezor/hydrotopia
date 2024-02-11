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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

#include <user_interface/xlib_screen.hpp>

xlib_screen::xlib_screen(int width, int height, int xpos, int ypos, std::string title, bool fullscreen)
    : screen(width, height)
{

    // Create window using default screen
    display_ = XOpenDisplay("");

    screen_ = DefaultScreen(display_);

    XSizeHints hints;
    memset(&hints, 0, sizeof(hints));
    hints.x = xpos;
    hints.y = ypos;
    hints.width = width;
    hints.height = height;
    hints.flags = PPosition|PSize;

    auto background = BlackPixel(display_, screen_);
    auto foreground = WhitePixel(display_, screen_);

    window_ = XCreateSimpleWindow(display_, DefaultRootWindow(display_),
                                 hints.x, hints.y,
                                 hints.width, hints.height,
                                 5, foreground, background);

    XSetStandardProperties(display_, window_, title.c_str(), title.c_str(),
                         None, NULL, 0, &hints);

    if (fullscreen) {
        XSizeHints* size_hints;
        long shints = 0;

        size_hints = XAllocSizeHints();

        if (XGetWMSizeHints(display_, window_, size_hints, &shints,
            XInternAtom(display_, "WM_SIZE_HINTS", False)) == 0) {
            puts("Failed.");
        } 

        XLowerWindow(display_, window_);
        XUnmapWindow(display_, window_);
        XSync(display_, False);

        XFree(size_hints);

        Atom atoms[2] = { XInternAtom(display_, "_NET_WM_STATE_FULLSCREEN", False), None };

        XChangeProperty(
            display_,
            window_,
            XInternAtom(display_, "_NET_WM_STATE", False),
            XA_ATOM, 32, PropModeReplace, (unsigned char*)atoms, 1);

        XMapWindow(display_, window_);
        XRaiseWindow(display_, window_);
    }

    // Connect cairo xlib surface to window
    cairo_surface_t* xlib_surface = cairo_xlib_surface_create(display_, window_, DefaultVisual(display_, screen_), width, height);
    cairo_xlib_surface_set_size(xlib_surface, width, height);
    cairo_t* xlib_cr = cairo_create(xlib_surface);


    root_surface_ = std::shared_ptr<surface>(new surface(xlib_surface, xlib_cr, static_cast<double>(width), static_cast<double>(height)));

    // Subscribe to input events
    long event_mask = ExposureMask;
    event_mask |= PointerMotionMask;
    event_mask |= ButtonPressMask;
    event_mask |= ButtonReleaseMask;
    event_mask |= KeyPressMask;
    event_mask |= KeyReleaseMask;
    event_mask |= FocusChangeMask;
    event_mask |= EnterWindowMask;
    event_mask |= LeaveWindowMask;
    XSelectInput(display_,window_,event_mask);

    // Raise window
    XMapRaised(display_, window_);
 
    // Subscribe to delete window event
    wm_delete_ = XInternAtom(display_, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(display_, window_, &wm_delete_, 1);

    // Disable default cursor
#if 0
    Cursor invisible_cursor;
    Pixmap bitmap;
    XColor black;
    memset(&black, 0, sizeof(black));
    static char no_data[] = { 0,0,0,0,0,0,0,0 };

    bitmap = XCreateBitmapFromData(display_, window_, no_data, 8, 8);
    invisible_cursor = XCreatePixmapCursor(display_, bitmap, bitmap, 
                                     &black, &black, 0, 0);
    XDefineCursor(display_,window_, invisible_cursor);
    XFreeCursor(display_, invisible_cursor);
    XFreePixmap(display_, bitmap);
#endif
}

xlib_screen::~xlib_screen()
{
    if (!closed_) {
        close();
    }
}

std::vector<std::shared_ptr<ui_event>> xlib_screen::poll_events()
{
    std::vector<std::shared_ptr<ui_event>> events;
  XEvent event;
  memset(&event, 0, sizeof(event));

  KeySym key;
  memset(&key, 0, sizeof(key));

  char text[10];
  memset(text, 0, sizeof(text));

    while(true) {
      auto event_in_queue = XCheckMaskEvent(display_, -1, &event);
      if (!event_in_queue) {
          break;
      }

      switch(event.type) {
        case Expose: {
            auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type::expose));
            events.emplace_back(e);
            break;
        }
        case MotionNotify: {
            auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type::pointer_motion,
                                  event.xbutton.x,
                                  event.xbutton.y));
            events.emplace_back(e);
            break;
        }
        case ButtonPress:
        case ButtonRelease: {
            bool pressed = (event.type == ButtonPress);
            switch (event.xbutton.button) {
                case 1:
                    button_event(button::left, pressed); 
                    break;
                case 2:
                    button_event(button::middle, pressed); // (scroll wheel pressed)
                    break;
                case 3:
                    button_event(button::right, pressed);
                    break;
                case 4:
                    button_event(button::scroll_up, pressed);
                    break;
                case 5:
                    button_event(button::scroll_down, pressed);
                    break;
                case 6:
                    button_event(button::scroll_left, pressed);
                    break;
                case 7:
                    button_event(button::scroll_right, pressed);
                    break;
                case 8:
                    button_event(button::nav_back, pressed);
                    break;
                case 9:
                    button_event(button::nav_forward, pressed);
                    break;
                default:
                    std::cerr << "Unknown xbutton.button" << std::endl;
                    continue;
            }
            ui_event_type ui_event_type = event.type == ButtonPress ? 
                ui_event_type::button_press : ui_event_type::button_release;
            auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type,
                                  event.xbutton.x,
                                  event.xbutton.y,
                                  button_state_));
            events.emplace_back(e);
            break;
        }
        case KeyPress: {
            auto i = XLookupString((XKeyEvent*)&event, text, sizeof(text), &key, 0);
            if (i == 1) {
                auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type::key_press,
                                      text[0]));
                events.emplace_back(e);
            }
            break;
        }
        case KeyRelease: {
            auto i = XLookupString((XKeyEvent*)&event, text, sizeof(text), &key, 0);
            if (i == 1) {
                auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type::key_release,
                                      text[0]));
                events.emplace_back(e);
            }
            break;
        }
        case FocusIn: {
            auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type::focus_in));
            events.emplace_back(e);
            break;
        }
        case FocusOut: {
            auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type::focus_out));
            events.emplace_back(e);
            break;
        }
        case EnterNotify: {
            auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type::enter));
            events.emplace_back(e);
            break;
        }
        case LeaveNotify: {
            auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type::leave));
            events.emplace_back(e);
            break;
        }
        case ClientMessage: // TODO: currently is acquired only with XNextEvent() but not XCheckWindowEvent()
            if (event.xclient.data.l[0] == wm_delete_) {
                auto e = std::shared_ptr<ui_event>(new ui_event(ui_event_type::close,
                                      text[0]));
                events.emplace_back(e);
            }
        default:
            break;
    }
  }
    return events;

}

void xlib_screen::button_event(button flag, bool pressed)
{
    if (pressed) {
        button_state_ |= flag;
    } else {
        button_state_ &= ~flag;
    }
}

void xlib_screen::close()
{
    if (!closed_) {
        root_surface_->destroy();
        XDestroyWindow(display_, window_);
        XCloseDisplay(display_);
        closed_ = true;
    }
}


