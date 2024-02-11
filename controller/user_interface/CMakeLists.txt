cmake_minimum_required(VERSION 3.14)

project(hydrotopia_ui)

set(CMAKE_BUILD_TYPE Release)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(XLIB REQUIRED x11)
pkg_check_modules(RSVG REQUIRED librsvg-2.0)

include(CTest)
include(GNUInstallDirs)

add_executable(hydrotopia_ui
    src/common.cpp
    src/graphics_context/rendering_context.cpp
    src/graphics_context/surface_cache.cpp
    src/graphics_context/surface.cpp
    src/hydrotopia_ui.cpp
    src/main.cpp
    src/object/background_object.cpp
    src/object/dashed_line_object.cpp
    src/object/image_object.cpp
    src/object/navigate_object.cpp
    src/object/object.cpp
    src/object/splash_screen_object.cpp
    src/object/text_object.cpp
    src/scene/00_cache_generation_scene/cache_generation_scene.cpp
    src/scene/01_idle_screen/idle_scene.cpp
    src/scene/02_security/keypad_scene.cpp
    src/scene/03_settings/settings_scene.cpp
    src/scene/scene.cpp
    src/timer.cpp
    src/user_interface/button.cpp
    src/user_interface/screen.cpp
    src/user_interface/xlib_screen.cpp
)

target_include_directories(hydrotopia_ui
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(hydrotopia_ui
  ${CAIRO_LIBRARIES}
  ${XLIB_LIBRARIES}
  ${RSVG_LIBRARIES}
  pthread
  png
)

target_include_directories(hydrotopia_ui
  PUBLIC
  ${CAIRO_INCLUDE_DIRS}
  ${XLIB_INCLUDE_DIRS}
  ${RSVG_INCLUDE_DIRS}
  include
)

target_compile_options(hydrotopia_ui
  PUBLIC
  ${CAIRO_CFLAGS_OTHER})

install(TARGETS hydrotopia_ui)

install(
    FILES
      images/tree_01.png
      images/tree_02.png
      images/tree_03.png
      images/tree_04.png
      images/tree_05.png
      images/tree_06.png
      images/tree_07.png
      images/tree_08.png
      images/tree_09.png
      images/grass.png
      images/branch.png
      images/garden.png
      images/daily_channels.png
      images/hourly_channels.png
      images/Keypad_button_00.svg
      images/Keypad_button_01.svg
      images/Keypad_button_02.svg
      images/Keypad_button_03.svg
      images/Keypad_button_04.svg
      images/Keypad_button_05.svg
      images/Keypad_button_06.svg
      images/Keypad_button_07.svg
      images/Keypad_button_08.svg
      images/Keypad_button_09.svg
      images/Keypad_button_star.svg
      images/Keypad_button_hash.svg
      images/settings_button_off.svg
      images/settings_button_5m.svg
      images/settings_button_15m.svg
      images/settings_button_30m.svg
      images/settings_button_45m.svg
      images/settings_button_60m.svg
      images/settings_button_3h.svg
      images/settings_button_6h.svg
      images/settings_button_12h.svg
      images/settings_button_18h.svg
      images/settings_lock.svg

    DESTINATION
        /usr/share/hydrotopia_ui/images
)

